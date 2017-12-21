#ifndef UTILS_H_
#define UTILS_H_

#include <time.h>
#include <string>

std::string getCurrentDateTime()
{
	time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return std::string(buf);
}

#endif