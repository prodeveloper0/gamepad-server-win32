#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define MAX_KP 15
#define KPBUF_SIZE (MAX_KP + 1)

enum KEYPAD
{
	KP_L1,
	KP_R1,
	KP_SELECT,
	KP_START,
	KP_LEFT,
	KP_UP,
	KP_DOWN,
	KP_RIGHT,
	KP_Y,
	KP_X,
	KP_B,
	KP_A
};

int kp_process(char* local_bits, const char* remote_bits, const BYTE* vk_table);
void kp_default(char* bits);