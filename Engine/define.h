#pragma once

#define ENGINE_DEBUG
//#define ENGINE_RELEASE

#define ScreenWidth (1920)
#define ScreenHeight (1080)

#define LOG_ENGINE(message) \
{\
	std::string file = __FILE__;\
	file = file.substr(file.find_last_of("/\\") + 1);\
	printf("[Engine] FILE : %s, LINE : %d\n[Engine] MESSAGE : %s\n\n", file.c_str(), __LINE__, message);\
}