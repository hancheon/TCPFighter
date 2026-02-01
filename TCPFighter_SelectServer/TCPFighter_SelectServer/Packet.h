#pragma once
#pragma pack(push, 1)

struct PACKET_HEADER
{
	unsigned char CODE;
	unsigned char SIZE;
	unsigned char TYPE;
};

enum MOVE_DIR
{
	PT_MOVE_DIR_LL,
	PT_MOVE_DIR_LU,
	PT_MOVE_DIR_UU,
	PT_MOVE_DIR_RU,
	PT_MOVE_DIR_RR,
	PT_MOVE_DIR_RD,
	PT_MOVE_DIR_DD,
	PT_MOVE_DIR_LD
};

enum PACKET_ID
{
	PT_SC_CREATE_MY_CHARACTER,
	PT_SC_CREATE_OTHER_CHARACTER,
	PT_SC_DELETE_CHARACTER,
	PT_CS_MOVE_START,
	PT_SC_MOVE_START,
	PT_CS_MOVE_STOP,
	PT_SC_MOVE_STOP,
	PT_CS_ATTACK1,
	PT_SC_ATTACK1,
	PT_CS_ATTACK2,
	PT_SC_ATTACK2,
	PT_CS_ATTACK3,
	PT_SC_ATTACK3,
	PT_SC_DAMAGE,
};

struct SC_CREATE_MY_CHARACTER
{
	unsigned int USER_ID;
	unsigned char DIR;
	short X;
	short Y;
	unsigned char HP;
};

struct SC_CREATE_OTHER_CHARACTER
{
	unsigned int USER_ID;
	unsigned char DIR;
	short X;
	short Y;
	unsigned char HP;
};

struct SC_DELETE_CHARACTER
{
	unsigned int USER_ID;
};

struct CS_MOVE_START
{
	unsigned char DIR;
	short X;
	short Y;
};

struct SC_MOVE_START
{
	unsigned int USER_ID;
	unsigned char DIR;
	short X;
	short Y;
};

struct CS_MOVE_STOP
{
	unsigned char DIR;
	short X;
	short Y;
};

struct SC_MOVE_STOP
{
	unsigned int USER_ID;
	unsigned char DIR;
	short X;
	short Y;
};

struct CS_ATTACK1
{
	unsigned char DIR;
	short X;
	short Y;
};

struct SC_ATTACK1
{
	unsigned int USER_ID;
	unsigned char DIR;
	short X;
	short Y;
};

struct CS_ATTACK2
{
	unsigned char DIR;
	short X;
	short Y;
};

struct SC_ATTACK2
{
	unsigned int USER_ID;
	unsigned char DIR;
	short X;
	short Y;
};

struct CS_ATTACK3
{
	unsigned char DIR;
	short X;
	short Y;
};

struct SC_ATTACK3
{
	unsigned int USER_ID;
	unsigned char DIR;
	short X;
	short Y;
};

struct SC_DAMAGE
{
	unsigned int ATTACKER_ID;
	unsigned int DAMAGE_ID;
	unsigned char DAMAGE_HP;
};

#pragma pack(pop)