#include "stdafx.h"
#include "Network.h"
#include "Packet.h"

int uniqueID = 0;

bool Server::Init()
{
	int retval;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << std::format("WSAStartup() failed: {}\n", WSAGetLastError());
		return false;
	}

	serverSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket_ == INVALID_SOCKET) {
		std::cerr << std::format("socket() failed: {}\n", WSAGetLastError());
		WSACleanup();
		return false;
	}

	u_long nonblock = 1;
	retval = ioctlsocket(serverSocket_, FIONBIO, &nonblock);
	if (retval == SOCKET_ERROR) {
		std::cerr << std::format("ioctlsocket() failed: {}\n", WSAGetLastError());
		closesocket(serverSocket_);
		WSACleanup();
		return false;
	}

	LINGER lin;
	lin.l_onoff = 1;
	lin.l_linger = 0;
	retval = setsockopt(serverSocket_, SOL_SOCKET, SO_LINGER, reinterpret_cast<char*>(&lin), sizeof(lin));
	if (retval == SOCKET_ERROR) {
		std::cerr << std::format("setsockopt() failed: {}\n", WSAGetLastError());
		closesocket(serverSocket_);
		WSACleanup();
		return false;
	}

	SOCKADDR_IN servAddr;
	ZeroMemory(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	retval = bind(serverSocket_, reinterpret_cast<SOCKADDR*>(&servAddr), sizeof(servAddr));
	if (retval == SOCKET_ERROR) {
		std::cerr << std::format("bind() failed: {}\n", WSAGetLastError());
		closesocket(serverSocket_);
		WSACleanup();
		return false;
	}

	std::cout << "# 서버 초기화 완료\n";

	retval = listen(serverSocket_, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		std::cerr << std::format("listen() failed: {}\n", WSAGetLastError());
		closesocket(serverSocket_);
		WSACleanup();
		return false;
	}

	FD_ZERO(&readSet_);
	FD_SET(serverSocket_, &readSet_);

	std::cout << "# 서버가 시작되었습니다. 클라이언트 연결 대기중...\n";

	return true;
}

void Server::Run()
{
	int retval;

	while (true) {
		// set 초기화
		FD_SET writeSet;
		FD_ZERO(&writeSet);

		// 보낼 데이터 있는 소켓 조사
		for (Session* s : clientSessions_) {
			if (s->sendBuffer->GetUsedSize() > 0) {
				FD_SET(s->socket, &writeSet);
			}
		}

		timeval time;
		time.tv_sec = 0;
		time.tv_usec = 0;

		// set 조사
		retval = select(0, &readSet_, &writeSet, nullptr, &time);
		if (retval > 0) { // 처리할 데이터 존재
			// Accept
			if (FD_ISSET(serverSocket_, &readSet_)) {
				NewSession();
			}

			for (Session* s : clientSessions_) {
				// Recv
				if (FD_ISSET(s, &readSet_)) {
					ReadProc(s);
				}

				// Send
				if (FD_ISSET(s, &writeSet)) {
					WriteProc(s);
				}
			}
		}
		else if (retval == SOCKET_ERROR) {
			std::cerr << std::format("select() failed: {}\n", WSAGetLastError());
			break;
		}
	}
}

void Server::NewSession()
{
	SOCKADDR_IN clientAddr;
	int clientAddrLen = sizeof(clientAddr);

	SOCKET socket = accept(serverSocket_, reinterpret_cast<SOCKADDR*>(&clientAddr), &clientAddrLen);
	if (socket == INVALID_SOCKET) {
		std::cerr << std::format("select() failed: {}\n", WSAGetLastError());
		return;
	}

	Session* newSession = new Session;
	newSession->socket = socket;
	newSession->sessionID = uniqueID;
	newSession->sendBuffer = new RingBuffer;
	newSession->recvBuffer = new RingBuffer;
	newSession->action = 0; // TODO: 임시 상태 (가만히 상태가 있던가, 아마 0이면 행동X 상태?로 지정할듯)
	newSession->direction = PT_MOVE_DIR_RR; // 기본 방향: 오른쪽 방향
	newSession->x = 100; // TODO: 임시 위치 (나중에 화면 랜덤?)
	newSession->y = 100; // TODO: 임시 위치 (나중에 화면 랜덤?)
	newSession->HP = 100;

	FD_SET(socket, &readSet_);
	clientSessions_.push_back(newSession);
	uniqueID++; // TODO: 유니크 아이디 제공하는 알고리즘 필요 (고민해봐야할듯)

	std::cout << std::format("# 새 클라이언트 연결 : {}\n", newSession->sessionID);
}

void Server::WriteProc(Session* s)
{
	int retval;

	while (s->sendBuffer->GetUsedSize()) {
		retval = send(s->socket, s->sendBuffer->GetBufferFrontPtr(), s->sendBuffer->GetDirectDequeueSize(), 0);
		if (retval == SOCKET_ERROR) {
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			std::cerr << std::format("send() failed: {}\n", WSAGetLastError());
			return;
		}
		s->sendBuffer->MoveFront(retval);
	}
}

void Server::ReadProc(Session* s)
{
	int retval;

	while (s->recvBuffer->GetFreeSize() > 0) {
		retval = recv(s->socket, s->recvBuffer->GetBufferRearPtr(), s->recvBuffer->GetDirectEnqueueSize(), 0);
		if (retval <= 0) {
			if (retval == 0) { // 클라이언트 정상 종료
				FD_CLR(s->socket, &readSet_);
				clientSessions_.remove(s);
				std::cout << std::format("# 클라이언트 {} 연결 종료\n", s->sessionID);
				delete s;
				return;
			}
			else {
				if (WSAGetLastError() != WSAEWOULDBLOCK) {
					std::cerr << std::format("recv() failed: {}\n", WSAGetLastError());
					return; // TODO: recv() 실패는 그 때 recv를 다시 시도해야할까? 아닐거 같음 out? 그건 잘 모르겠네
				}
			}
		}
		else {
			s->recvBuffer->MoveRear(retval);

			PACKET_HEADER header;
			if (s->recvBuffer->Peek((char*)&header, sizeof(header)) != sizeof(header)) continue;
			if (header.CODE != 0x89) return; // TODO: 아마 부정 사용유저? 패킷 손댐 log 따고 강종
			if (s->recvBuffer->GetUsedSize() < (sizeof(header) + header.SIZE)) continue;
			// TODO: PacketStream에서 언마샬링하고 처리
		}
	}
}

void Server::PacketProc()
{

}
