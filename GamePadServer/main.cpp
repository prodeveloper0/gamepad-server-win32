#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#include "socket.h"
#include "keypad.h"
#include "logger.h"
#include "settings.h"

int main(int argc, char **argv)
{
	char local_kb[KPBUF_SIZE + 1] = "";
	char remote_kb[KPBUF_SIZE + 1] = "";
	char msgbuf[128];

	logging("WSAStartup(): Initializing Winsock", LOG_INFO);
	if (!Socket::Startup())
	{
		logging("WStartup(): Failed to initializing Winsock", LOG_ERROR);
		return -1;
	}

	logging("Socket: Creating using (AF_INET, SOCK_DGRAM)", LOG_INFO);
	Socket udp_server(AF_INET, SOCK_DGRAM);
	if (udp_server.is_invalid())
	{
		logging("Socket: Socket is INVALID_SOCKET", LOG_ERROR);
		return -1;
	}

	logging("KeyPad: Loading settings from file", LOG_INFO);
	if (readfile("keypad.ini") == -1)
	{
		logging("KeyPad: Cannot load keybits and vktable from file", LOG_ERROR);
		return -1;
	}

	logging("KeyPad: Initializing keybits", LOG_INFO);
	kp_default(local_kb);
	kp_default(remote_kb);

	sprintf(msgbuf, "Socket: Binding server ::%d", _port);
	logging(msgbuf, LOG_INFO);
	if (udp_server.bind(InetAddr(_port)) == SOCKET_ERROR)
	{
		sprintf(msgbuf, "Socket: Cannot bind server ::%d, _error=%d", _port, udp_server.error());
		logging(msgbuf, LOG_ERROR);
		return -1;
	}
	udp_server.setblocking(false);

	logging("Socket: Ready to receive keybits", LOG_INFO);
	logging("KeyPad: Ready to receive keybits", LOG_INFO);
	while (true)
	{
		if (udp_server.recv(remote_kb, KPBUF_SIZE) == -1)
		{
			if (udp_server.error() != WSAEWOULDBLOCK)
			{
				sprintf(msgbuf, "Socket: Cannot received keybits from client, _error=%d", udp_server.error());
				logging(msgbuf, LOG_ERROR);
				break;
			}
		}
		else
		{
			sprintf(msgbuf, "Socket: Keybits are received from client, keybits=%s", remote_kb);
			logging(msgbuf, LOG_INFO);
		}

		kp_process(local_kb, remote_kb, _vktable);
		Sleep(_interval);
	}

	logging("Socket: Closing", LOG_INFO);
	udp_server.close();

	logging("WSAClenup(): Cleaning up Winsock", LOG_INFO);
	Socket::Cleanup();

	return 0;
}
