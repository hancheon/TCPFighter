#include "stdafx.h"
#include "RingBuffer.h"

RingBuffer::RingBuffer() : bufferSize_(defaultBufferSize), frontIdx_(0), rearIdx_(0)
{
	bufferPtr_ = new char[bufferSize_ + 1];
}

RingBuffer::RingBuffer(int size) : bufferSize_(size), frontIdx_(0), rearIdx_(0)
{
	bufferPtr_ = new char[bufferSize_ + 1];
}

RingBuffer::~RingBuffer()
{
	delete[] bufferPtr_;
}

unsigned int RingBuffer::GetBufferSize() const
{
	return bufferSize_;
}

unsigned int RingBuffer::GetUsedSize() const
{
	int diff = rearIdx_ - frontIdx_;

	return diff >= 0 ? diff : bufferSize_ + 1 + diff;
}

unsigned int RingBuffer::GetFreeSize() const
{
	return bufferSize_ - GetUsedSize();
}

unsigned int RingBuffer::GetDirectEnqueueSize() const
{
	int diff = rearIdx_ - frontIdx_;

	return diff >= 0 ? bufferSize_ - rearIdx_ : -1 - diff;
}

unsigned int RingBuffer::GetDirectDequeueSize() const
{
	int diff = rearIdx_ - frontIdx_;

	return diff >= 0 ? diff : bufferSize_ - frontIdx_;
}

unsigned int RingBuffer::Enqueue(const char* data, unsigned int reqSize)
{
#ifdef __SAFE_MODE__
	//------Exception Handling------//
	if (data == nullptr || reqSize > GetFreeSize() || (rearIdx + 1) % (bufferSize_ + 1) == frontIdx_)
		return 0;
#endif

	unsigned int directSize = GetDirectEnqueueSize();

	if (reqSize <= directSize) {
		//------Direct Enqueue------//
		memcpy(bufferPtr_ + rearIdx_ + 1, data, reqSize);
	}
	else {
		//------Split Enqueue------//
		memcpy(bufferPtr_ + rearIdx_ + 1, data, directSize);
		memcpy(bufferPtr_, data + directSize, reqSize - directSize);
	}

	//------Move Rear------//
	MoveRear(reqSize);

	return reqSize;
}

unsigned int RingBuffer::Dequeue(char* data, unsigned int reqSize)
{
#ifdef __SAFE_MODE__
	//------Exception Handling------//
	if (data == nullptr || reqSize > GetUsedSize() || rearIdx_ == frontIdx_)
		return 0;
#endif

	unsigned int directSize = GetDirectDequeueSize();

	//------Dequeue------//
	if (reqSize <= directSize) {
		memcpy(data, bufferPtr_ + frontIdx_ + 1, reqSize);
	}
	else {
		//------Split Dequeue------//
		memcpy(data, bufferPtr_ + frontIdx_ + 1, directSize);
		memcpy(data + directSize, bufferPtr_, reqSize - directSize);
	}

	//------Move Front------//
	MoveFront(reqSize);

	return reqSize;
}

unsigned int RingBuffer::Peek(char* data, unsigned int reqSize)
{
#ifdef __SAFE_MODE__
	//------Exception Handling------//
	if (data == nullptr || reqSize > GetUsedSize() || rearIdx_ == frontIdx_)
		return 0;
#endif

	unsigned int directSize = GetDirectDequeueSize();

	if (reqSize <= directSize) {
		//------Direct Peek------//
		memcpy(data, bufferPtr_ + frontIdx_ + 1, reqSize);
	}
	else {
		//------Split Peek------//
		memcpy(data, bufferPtr_ + frontIdx_ + 1, directSize);
		memcpy(data + directSize, bufferPtr_, reqSize - directSize);
	}

	return reqSize;
}

void RingBuffer::ClearBuffer()
{
	frontIdx_ = rearIdx_;
}

int RingBuffer::MoveFront(int size)
{
	frontIdx_ = (frontIdx_ + size) % (bufferSize_ + 1);
	return frontIdx_;
}

int RingBuffer::MoveRear(int size)
{
	rearIdx_ = (rearIdx_ + size) % (bufferSize_ + 1);
	return rearIdx_;
}

char* RingBuffer::GetBufferFrontPtr() const
{
	return bufferPtr_ + frontIdx_ + 1;
}

char* RingBuffer::GetBufferRearPtr() const
{
	return bufferPtr_ + rearIdx_ + 1;
}
