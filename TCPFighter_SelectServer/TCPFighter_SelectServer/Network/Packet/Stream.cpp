#include "stdafx.h"
#include "Stream.h"

Stream::Stream() : bufferSize_(defaultPacketSize), frontIdx_(0), rearIdx_(0)
{
	stream_ = new char[bufferSize_];
}

Stream::Stream(int size) : bufferSize_(size), frontIdx_(0), rearIdx_(0)
{
	stream_ = new char[bufferSize_];
}

Stream::~Stream()
{
	delete[] stream_;
}

int Stream::GetBufferSize() const
{
	return bufferSize_;
}

int Stream::GetDataSize() const
{
	return rearIdx_ - frontIdx_;
}

char* Stream::GetBufferPtr() const
{
	return stream_;
}

int Stream::MoveRear(int size)
{
	//------Exception Handling------//
	if (size <= 0 || rearIdx_ + size > bufferSize_)
		return 0;

	rearIdx_ += size;

	return size;
}

int Stream::MoveFront(int size)
{
	//------Exception Handling------//
	if (size <= 0 || frontIdx_ + size > rearIdx_)
		return 0;

	frontIdx_ += size;

	return size;
}

Stream& Stream::operator=(const Stream& srcPacket)
{
	if (this == &srcPacket)
		return *this;

	assert(srcPacket.GetBufferSize() > bufferSize_);

	Clear();
	PutData(srcPacket.GetBufferPtr() + frontIdx_, srcPacket.GetDataSize());

	return *this;
}

//------------------------------------------------------------//
// write
//------------------------------------------------------------//
bool Stream::CheckWriteBound(size_t len)
{
	if (offset_ + len > sizeof(stream_)) {
		ASSERT(FALSE);
	}
}

Stream& Stream::operator<<(UCHAR value)
{
	PutData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator<<(CHAR value)
{
	PutData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator<<(USHORT value)
{
	PutData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator<<(SHORT value)
{
	PutData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator<<(UINT32 value)
{
	PutData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator<<(INT32 value)
{
	PutData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator<<(ULONG value)
{
	PutData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator<<(LONG value)
{
	PutData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator<<(UINT64 value)
{
	PutData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator<<(INT64 value)
{
	PutData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator<<(float value)
{
	PutData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator<<(double value)
{
	PutData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

//------------------------------------------------------------//
// read
//------------------------------------------------------------//
bool Stream::CheckReadBound(size_t len)
{
	return false;
}

Stream& Stream::operator>>(UCHAR& value)
{
	GetData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator>>(CHAR& value)
{
	GetData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator>>(USHORT& value)
{
	GetData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator>>(SHORT& value)
{
	GetData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator>>(UINT32& value)
{
	GetData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator>>(INT32& value)
{
	GetData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator>>(ULONG& value)
{
	GetData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator>>(LONG& value)
{
	GetData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator>>(UINT64& value)
{
	GetData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator>>(INT64& value)
{
	GetData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator>>(float& value)
{
	GetData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

Stream& Stream::operator>>(double& value)
{
	GetData(reinterpret_cast<PCHAR>(&value), sizeof(value));
	return *this;
}

