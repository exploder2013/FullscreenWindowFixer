#pragma once

#include <string>
#include <fstream>

class FileLogger
{
public:
	FileLogger( std::string name );

	void WriteToLogFileA( char* format, ... );
	void WriteToLogFileW( wchar_t* format, ... );

	char* GetCurrentTimeA();
	wchar_t* GetCurrentTimeW();

private:
	void SetLogfile( std::string name );
	
private:
	std::string logfile;
};

