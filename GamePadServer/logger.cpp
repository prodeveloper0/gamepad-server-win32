#include "logger.h"

void logging(const char* message, const char* type, FILE* out)
{
	time_t now = time(NULL);
	struct tm *tm_now = localtime(&now);
	char timestamp[100] = { 0, };

	strftime(timestamp, 100, "%Y/%m/%d %H:%M:%S", tm_now);

	fprintf(out, "%s %s %s\n", timestamp, type, message);
}
