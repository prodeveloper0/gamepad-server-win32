#define _CRT_SECURE_NO_WARNINGS
#include "settings.h"
#include "keypad.h"

#include <iostream>
#include <fstream>
#include <string>
#include <map>

const char* trim_space = " \t\n\v";

int _interval = 33;
unsigned short _port = 3300;
BYTE _vktable[MAX_KP] = { 0, };

char *_vkname[] = {"KP_L1", "KP_R1", "KP_SELECT", "KP_START", "KP_LEFT", "KP_UP", "KP_DOWN", "KP_RIGHT", "KP_Y", "KP_X", "KP_B", "KP_A", "", "", "" };

inline std::string trim(const std::string& s, const std::string& drop = trim_space)
{
	std::string ss = s;
	std::string r = ss.erase(s.find_last_not_of(drop) + 1);
	return r.erase(0, r.find_first_not_of(drop));
}

inline bool is_digits(const std::string& str)
{
	if (trim(str) == "")
		return false;

	return str.find_first_not_of("0123456789abcdefABCDEFx") == std::string::npos;
}

int readfile(const char* filename)
{
	std::map<std::string, std::string> vk;
	std::ifstream input(filename);
	char line[64];

	if (input.fail())
		return -1;

	while (!input.eof())
	{
		input.getline(line, 64);

		if (trim(line) == "")
			continue;

		std::cout << trim(line) << std::endl;

		std::string key, value;
		key = trim(strtok(line, "="));
		value = trim(strtok(NULL, "="));

		vk[key] = value;
	}

	input.close();

	for (int i = 0; i < MAX_KP; ++i)
	{
		if (strlen(_vkname[i]))
			_vktable[i] = is_digits(vk[_vkname[i]]) ? (BYTE)strtol(vk[_vkname[i]].c_str(), NULL, 16) : 0;
	}

	_interval = is_digits(vk["INTERVAL"]) ? std::stoi(vk["INTERVAL"]) : 33;
	_port = is_digits(vk["BIND_PORT"]) ? (unsigned short)std::stoul(vk["BIND_PORT"]) : 3300;

	return 0;
}
