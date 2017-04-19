#include "keypad.h"

int kp_process(char* local_bits, const char* remote_bits, const BYTE* vk_table)
{
	if (local_bits[MAX_KP] != '$' || remote_bits[MAX_KP] != '$')
		return -1;

	for (size_t i = 0; i < MAX_KP; ++i)
	{
		ULONG extra_info = 0;

		if (local_bits[i] == '0' && remote_bits[i] == '0')
			continue;

		local_bits[i] = remote_bits[i];
		keybd_event(vk_table[i], 0, (local_bits[i] == '0') ? KEYEVENTF_KEYUP : 0, (ULONG_PTR)&extra_info);
	}

	return 0;
}

void kp_default(char* bits)
{
	for (size_t i = 0; i < MAX_KP; ++i)
		bits[i] = '0';
	bits[MAX_KP] = '$';
}
