#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>

#define LOG_ERROR "ERROR"
#define LOG_INFO "INFO"
#define LOG_WARN "WARN"

void logging(const char* message, const char* type, FILE* out = stdout);
