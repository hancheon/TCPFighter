#pragma once

#include "RingBuffer.h"

struct Session {
	SOCKET			socket;
	DWORD			sessionID;
	RingBuffer*		sendBuffer;
	RingBuffer*		recvBuffer;

	DWORD			action;
	BYTE			direction;

	short			x;
	short			y;

	char			HP;
};