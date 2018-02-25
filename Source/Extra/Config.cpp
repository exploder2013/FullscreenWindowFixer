#include "Config.h"
#include <stdarg.h>
#include <ctime>
#include <windows.h>
#include <stringapiset.h>
#include <time.h>

FileLogger::FileLogger( std::string name )
	: logfile( name )
{
	SetLogfile( name );
	WriteToLogFileA( "-----------[%s]----------", GetCurrentTimeA() );
}

void FileLogger::SetLogfile( std::string name )
{
	std::ifstream file( name, std::ifstream::app );
	if( file.is_open() )
	{
		file.close();
		return;
	}

	// Create file as it doesn't exist.
	std::ofstream file2( name, std::ofstream::out );
	file2.close();
}

void FileLogger::WriteToLogFileA( char * format, ... )
{
	va_list va_alist;
	char buf[ 1024 ] = { 0 };
	std::ofstream file;

	va_start( va_alist, format );

	_vsnprintf_s( buf, 
		_countof( buf ),
		_TRUNCATE,
		format, va_alist );

	va_end( va_alist );

	char timebuf[ 100 ] = { 0 };
	if( ( file = std::ofstream( logfile, std::ofstream::app ) ).is_open() )
	{
		sprintf_s( timebuf, sizeof( timebuf ), "[%s]: ", GetCurrentTimeA() );

		file << timebuf << buf << "\n";
		file.close();
	}
}

void FileLogger::WriteToLogFileW( wchar_t * format, ... )
{
	va_list va_alist;
	wchar_t buf[ 1024 ] = { 0 };
	std::wofstream file;

	va_start( va_alist, format );

	_vsnwprintf_s( buf,
		_countof( buf ),
		_TRUNCATE,
		format, va_alist );

	va_end( va_alist );

	if( ( file = std::wofstream( logfile, std::wofstream::app ) ).is_open() )
	{
		wchar_t timebuf[ 100 ] = { 0 };
		swprintf_s( timebuf, sizeof( timebuf ) / 2, L"[%s]: ", GetCurrentTimeW() );

		file << timebuf << buf << L"\n";
		file.close();
	}
}

char* FileLogger::GetCurrentTimeA()
{
	static char timebuf[ 100 ] = { 0 };
	std::time_t time = std::time( nullptr );

	ctime_s( timebuf, sizeof( timebuf ), &time );
	timebuf[ strlen( timebuf ) - 1 ] = 0;

	return timebuf;
}

wchar_t * FileLogger::GetCurrentTimeW()
{
	static wchar_t timebuf[ 100 ] = { 0 };
	
	MultiByteToWideChar( CP_UTF8, NULL, GetCurrentTimeA(),
		-1, timebuf, sizeof( timebuf ) );

	return timebuf;
}
