#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <vector>

#include "Session.h"
#include "RingBuffer.h"

#define PORT 5000 // TODO: 나중에 config 파일로 뺄 예정

class Server
{
private:
	SOCKET serverSocket_;
	FD_SET readSet_;
	std::list<Session*> clientSessions_; // 나중에 세션 풀로 대체

public:
	Server() : serverSocket_(INVALID_SOCKET), readSet_(NULL) {}

	~Server() {}

	bool Init();

	void Run();

	void NewSession();

	void WriteProc(Session*);

	void ReadProc(Session*);

	void PacketProc();
};