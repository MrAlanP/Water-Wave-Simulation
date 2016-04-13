#pragma once

#include <d3d11.h>
#include <sstream>
#include <string>

class Console
{
public:
	static void Log(std::string output) {
		HANDLE myConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD Written;

		output += "\n";

		char* charOutput = &output[0u];
		
		WriteConsoleA(myConsoleHandle, charOutput, strlen(charOutput), &Written, NULL);
	};

	static void Log(int output) {
		std::stringstream stringStream;
		stringStream << output;
		std::string str = stringStream.str();
		Log(str);
	};

	static void Log(float output) {
		std::stringstream stringStream;
		stringStream << output;
		std::string str = stringStream.str();
		Log(str);
	};

};