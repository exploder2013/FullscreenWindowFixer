#include "Logger.h"
#include <stdio.h>
#include <assert.h>

void Log::warn( const char * format, ... )
{
	va_list list;

	CONSOLE_SCREEN_BUFFER_INFO screenInfo = CONSOLE_SCREEN_BUFFER_INFO();
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

	// Save previous state
	assert( GetConsoleScreenBufferInfo( hConsole, &screenInfo ) != 0 );
	SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_GREEN );

	// Print tabs before processing.
	for( ; format != nullptr && *format == '\t'; format++ )
		printf( "\t" );

	printf( "[?] " );

	va_start( list, format );
	vprintf( format, list );
	va_end( list );

	SetConsoleTextAttribute( hConsole, screenInfo.wAttributes );
}

void Log::error( const char * format, ... )
{
	va_list list;

	CONSOLE_SCREEN_BUFFER_INFO screenInfo = CONSOLE_SCREEN_BUFFER_INFO();
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

	// Save previous state
	assert( GetConsoleScreenBufferInfo( hConsole, &screenInfo ) != 0 );
	SetConsoleTextAttribute( hConsole, FOREGROUND_RED );

	// Print tabs before processing.
	for( ; format != nullptr && *format == '\t'; format++ )
		printf( "\t" );

	printf( "[!] " );
	va_start( list, format );
	vprintf( format, list );
	va_end( list );

	SetConsoleTextAttribute( hConsole, screenInfo.wAttributes );
}

void Log::print( const char * format, ... )
{
	va_list list;

	CONSOLE_SCREEN_BUFFER_INFO screenInfo = CONSOLE_SCREEN_BUFFER_INFO();
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

	// Save previous state
	assert( GetConsoleScreenBufferInfo( hConsole, &screenInfo ) != 0 );
	SetConsoleTextAttribute( hConsole, FOREGROUND_GREEN );

	// Print tabs before processing.
	for( ; format != nullptr && *format == '\t'; format++ )
		printf( "\t" );

	printf( "[+] " );
	va_start( list, format );
	vprintf( format, list );
	va_end( list );

	SetConsoleTextAttribute( hConsole, screenInfo.wAttributes );
}

void Log::warn( const wchar_t * format, ... )
{
	va_list list;

	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

	// Save previous state
	assert( GetConsoleScreenBufferInfo( hConsole, &screenInfo ) != 0 );
	SetConsoleTextAttribute( hConsole, FOREGROUND_RED | FOREGROUND_GREEN );

	// Print tabs before processing.
	for( ; format != nullptr && *format == L'\t'; format++ )
		wprintf( L"\t" );

	printf( "[?] " );
	va_start( list, format );
	vwprintf( format, list );
	va_end( list );

	SetConsoleTextAttribute( hConsole, screenInfo.wAttributes );
}

void Log::error( const wchar_t * format, ... )
{
	va_list list;

	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

	// Save previous state
	assert( GetConsoleScreenBufferInfo( hConsole, &screenInfo ) != 0 );
	SetConsoleTextAttribute( hConsole, FOREGROUND_RED );

	// Print tabs before processing.
	for( ; format != nullptr && *format == L'\t'; format++ )
		wprintf( L"\t" );

	printf( "[!] " );
	va_start( list, format );
	vwprintf( format, list );
	va_end( list );

	SetConsoleTextAttribute( hConsole, screenInfo.wAttributes );
}

void Log::print( const wchar_t * format, ... )
{
	va_list list;

	CONSOLE_SCREEN_BUFFER_INFO screenInfo = CONSOLE_SCREEN_BUFFER_INFO();
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

	// Save previous state
	assert( GetConsoleScreenBufferInfo( hConsole, &screenInfo ) != 0 );
	SetConsoleTextAttribute( hConsole, FOREGROUND_GREEN );

	// Print tabs before processing.
	for( ; format != nullptr && *format == L'\t'; format++ )
		wprintf( L"\t" );

	printf( "[+] " );
	va_start( list, format );
	vwprintf( format, list );
	va_end( list );

	SetConsoleTextAttribute( hConsole, screenInfo.wAttributes );
}

void Log::warnn( const char * format, ... )
{
	warn( format );
	printf( "\n" );
}

void Log::errorn( const char * format, ... )
{
	error( format );
	printf( "\n" );
}

void Log::printn( const char * format, ... )
{
	print( format );
	printf( "\n" );
}

WORD Log::color( WORD color )
{
	CONSOLE_SCREEN_BUFFER_INFO screenInfo = CONSOLE_SCREEN_BUFFER_INFO();
	HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

	assert( GetConsoleScreenBufferInfo( hConsole, &screenInfo ) != 0 );
	SetConsoleTextAttribute( hConsole, color );

	return screenInfo.wAttributes;
}
