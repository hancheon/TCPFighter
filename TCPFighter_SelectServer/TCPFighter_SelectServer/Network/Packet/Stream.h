#pragma once
#include <Windows.h>

class Stream
{
	static constexpr int defaultPacketSize = 1400;
	int bufferSize_;
	char* stream_;
	int frontIdx_;
	int rearIdx_;

public:
	Stream();
	Stream(int size);
	virtual ~Stream();

	inline void Clear()
	{
		frontIdx_ = 0;
		rearIdx_ = 0;
	}

	int GetBufferSize() const;
	int GetDataSize() const;
	char* GetBufferPtr() const;

	int MoveRear(int size);
	int MoveFront(int size);


	Stream& operator=(const Stream& srcPacket);

	//---------------------------------------------//
	// write
	//---------------------------------------------//
	bool CheckWriteBound(size_t len);

	Stream& operator<<(UCHAR value);
	Stream& operator<<(CHAR value);
	Stream& operator<<(USHORT value);
	Stream& operator<<(SHORT value);
	Stream& operator<<(UINT32 value);
	Stream& operator<<(INT32 value);
	Stream& operator<<(ULONG value);
	Stream& operator<<(LONG value);
	Stream& operator<<(UINT64 value);
	Stream& operator<<(INT64 value);
	Stream& operator<<(float value);
	Stream& operator<<(double value);
	Stream& operator<<(bool value);

	inline int PutData(char* src, int size)
	{
		//------Exception Handling------//
		if (size <= 0 || rearIdx_ + size > bufferSize_)
			return 0;

		memcpy(stream_ + rearIdx_, src, size);
		rearIdx_ += size;

		return size;
	}

	//---------------------------------------------//
	// read
	//---------------------------------------------//
	bool CheckReadBound(size_t len);

	Stream& operator>>(UCHAR& value);
	Stream& operator>>(CHAR& value);
	Stream& operator>>(USHORT& value);
	Stream& operator>>(SHORT& value);
	Stream& operator>>(UINT32& value);
	Stream& operator>>(INT32& value);
	Stream& operator>>(ULONG& value);
	Stream& operator>>(LONG& value);
	Stream& operator>>(UINT64& value);
	Stream& operator>>(INT64& value);
	Stream& operator>>(float& value);
	Stream& operator>>(double& value);
	Stream& operator>>(bool& value);

	inline int GetData(char* dest, int size)
	{
		//------Exception Handling------//
		if (size <= 0 || frontIdx_ + size > rearIdx_)
		{
			return 0;
		}

		memcpy(dest, stream_ + frontIdx_, size);
		frontIdx_ += size;

		return size;
	}
};