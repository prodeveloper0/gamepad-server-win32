#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include <string>

#include <Windows.h>
#include <Ws2tcpip.h>
#include <WinSock2.h>
#include <iostream>

/*
	Written by pr0ximo(Samuel Lee)
	WinSock wrapper
*/

// SOCKADDR class
class Addr
{
public:
	virtual operator sockaddr*() { return NULL; }
	virtual int size() const { return -1; }
};

class InetAddr : public Addr
{
private:
	sockaddr_in _SockAddr;

public:
	InetAddr()
	{
		_SockAddr.sin_family = AF_INET;
		_SockAddr.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);
	}

	InetAddr(unsigned short port)
	{
		_SockAddr.sin_family = AF_INET;
		_SockAddr.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);
		_SockAddr.sin_port = ::htons(port);
	}

	InetAddr(const std::string &address, unsigned short port)
	{
		_SockAddr.sin_family = AF_INET;
		_SockAddr.sin_addr.S_un.S_addr = ::inet_addr(address.c_str());
		_SockAddr.sin_port = ::htons(port);
	}

	operator sockaddr*()
	{
		return (sockaddr*)(&_SockAddr);
	}

	int size() const
	{
		return sizeof(_SockAddr);
	}
};

// Socket
class Socket
{
protected:
	SOCKET _Socket;

public:
	static bool Startup()
	{
		WSADATA wsadata;
		if (::WSAStartup(MAKEWORD(2, 2), &wsadata) != NO_ERROR)
			return false;
		return true;
	}

	static void Cleanup()
	{
		WSACleanup();
	}

public:
	Socket(int af, int type, int protocol = 0)
	{
		_Socket = socket(af, type, protocol);
	}

	Socket(const SOCKET sock = INVALID_SOCKET)
	{
		_Socket = sock;
	}

public:
	int error()
	{
		int code = ::WSAGetLastError();

		return code;
	}

	int bind(Addr& myaddr)
	{
		return ::bind(_Socket, (sockaddr*)myaddr, myaddr.size());
	}

	int listen(int backlog = 5)
	{
		return ::listen(_Socket, backlog) == SOCKET_ERROR;
	}

	Socket accept(Addr& myaddr)
	{
		int len = myaddr.size();
		SOCKET sock = ::accept(_Socket, (sockaddr*)myaddr, &len);

		return Socket(sock);
	}

	int connect(Addr& myaddr)
	{
		return ::connect(_Socket, (sockaddr*)myaddr, myaddr.size());
	}

	int send(const char* buf, int len, int flags = 0)
	{
		return ::send(_Socket, buf, len, flags);
	}

	int send(const std::string buf, int flags = 0)
	{
		int code = ::WSAGetLastError();
		int result = send(buf.c_str(), buf.length(), flags);

		::WSASetLastError(code);

		return result;
	}

	int sendto(const char* buf, int len, Addr& myaddr, int flags = 0)
	{
		return ::sendto(_Socket, buf, len, flags, (sockaddr*)myaddr, myaddr.size());
	}

	int sendto(const std::string buf, Addr& myaddr, int flags = 0)
	{
		int code = ::WSAGetLastError();
		int result = sendto(buf.c_str(), buf.length(), myaddr, flags);

		::WSASetLastError(code);
		
		return result;
	}

	int recv(char* buf, int len, int flags = 0)
	{
		return ::recv(_Socket, buf, len, flags);
	}

	int recv(std::string& buf, int len, int flags = 0)
	{
		std::string recved;
		int result = SOCKET_ERROR;
		
		recved.resize(len);
		result = recv(&recved[0], len, flags);

		int code = ::WSAGetLastError();

		buf = std::string(&recved[0]);
		
		::WSASetLastError(code);

		return result;
	}

	int recvfrom(char* buf, int len, Addr& myaddr, int flags = 0)
	{
		return ::recvfrom(_Socket, buf, len, flags, (sockaddr*)myaddr, NULL);
	}

	int recvfrom(std::string& buf, int len, Addr& myaddr, int flags = 0)
	{
		std::string recved;
		int result = SOCKET_ERROR;

		recved.resize(len);
		result = recvfrom(recved, len, myaddr, flags);

		int code = ::WSAGetLastError();

		buf = std::string(&recved[0]);

		::WSASetLastError(code);

		return result;
	}

	int close()
	{
		SOCKET tmp = _Socket;
		_Socket = INVALID_SOCKET;

		return ::closesocket(tmp);
	}

	int setsockopt(int level, int optname, const char* optval, int optlen)
	{
		return ::setsockopt(_Socket, level, optname, optval, optlen);
	}

	int getsockopt(int level, int optname, char* optval, int* optlen)
	{
		return ::getsockopt(_Socket, level, optname, optval, optlen);
	}

	int ioctlsocket(long cmd, u_long* argp)
	{
		return ::ioctlsocket(_Socket, cmd, argp);
	}

	int setblocking(bool block)
	{
		u_long val = block ? 0 : 1;

		return ioctlsocket(FIONBIO, &val);
	}

	SOCKET native_socket()
	{
		return _Socket;
	}

	bool is_invalid() const
	{
		return _Socket == INVALID_SOCKET;
	}
};
