#pragma once

class RingBuffer
{
	friend class Server;
	static constexpr int defaultBufferSize = 10000;

public:
	//------Constructor & Destructor------//
	RingBuffer();
	RingBuffer(int size); // 사용자가 버퍼 크기를 직접 정의함
	~RingBuffer();

	//------버퍼 정보 제공 함수------//
	unsigned int GetBufferSize() const;
	unsigned int GetUsedSize() const;
	unsigned int GetFreeSize() const;
	unsigned int GetDirectEnqueueSize() const; // 현재 rear에서 연속적으로 삽입할 수 있는 데이터 크기 반환
	unsigned int GetDirectDequeueSize() const; // 현재 front에서 연속적으로 추출할 수 있는 데이터 크기 반환

	//------일반적인 데이터 입출력 함수------//
	unsigned int Enqueue(const char* data, unsigned int reqSize);
	unsigned int Dequeue(char* data, unsigned int reqSize);
	unsigned int Peek(char* data, unsigned int reqSize); // 데이터 추출하지 않고 복사

	//------버퍼 초기화 (데이터 보존을 위해 front, rear 위치만 이동)------//
	void ClearBuffer();

	//------front, rear 이동 함수------//
	int MoveFront(int size);
	int MoveRear(int size);

private:
	//------Recv()와 Send()에서 직접 버퍼에 접근하기 위해 필요한 함수------//
	char* GetBufferFrontPtr() const;
	char* GetBufferRearPtr() const;

private:
	int bufferSize_;
	char* bufferPtr_;
	int frontIdx_; // 데이터 추출 인덱스
	int rearIdx_; // 데이터 삽입 인덱스
};