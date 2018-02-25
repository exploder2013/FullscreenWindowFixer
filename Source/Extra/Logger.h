#pragma once
#include <windows.h>

namespace Log
{
	void warn( const char* format, ... );
	void error( const char* format, ... );
	void print( const char* format, ... );

	void warn( const wchar_t* format, ... );
	void error( const wchar_t* format, ... );
	void print( const wchar_t* format, ... );

	// newline versions
	void warnn( const char* format, ... );
	void errorn( const char* format, ... );
	void printn( const char* format, ... );

	WORD color( WORD color );
}