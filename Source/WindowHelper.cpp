#include "WindowHelper.h"

#include <windows.h>
#include <TlHelp32.h>

bool WindowHelper::IsProcessRunning( const std::string & procName )
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof( PROCESSENTRY32 );

	HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
	if( Process32First( snapshot, &entry ) == TRUE )
	{
		while( Process32Next( snapshot, &entry ) == TRUE )
		{
			if( procName.find( entry.szExeFile ) != std::string::npos )
			{
				CloseHandle( snapshot );
				return true;
			}
		}
	}

	CloseHandle( snapshot );
	return false;
}

bool WindowHelper::GetProcessInformation( const std::string & procName, ProcessInfo& procInfo )
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof( PROCESSENTRY32 );

	HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
	if( Process32First( snapshot, &entry ) == TRUE )
	{
		while( Process32Next( snapshot, &entry ) == TRUE )
		{
			if( procName.find( entry.szExeFile ) != std::string::npos )
			{
				procInfo.exeName = entry.szExeFile;
				procInfo.procId = entry.th32ProcessID;

				CloseHandle( snapshot );
				return true;
			}
		}
	}

	CloseHandle( snapshot );
	return false;
}

bool WindowHelper::IsWindowTopmost( HWND hWnd )
{
	WINDOWINFO wInfo;
	GetWindowInfo( hWnd, &wInfo );

	return (wInfo.dwExStyle & WS_EX_TOPMOST) ? true : false;
}

bool WindowHelper::IsWindowFullscreen( HWND hWnd )
{
	RECT size;

	const int cX = GetSystemMetrics( SM_CXSCREEN );
	const int cY = GetSystemMetrics( SM_CYSCREEN );

	GetWindowRect( hWnd, &size );
	return size.right + size.left == cX && size.bottom + size.top == cY;
}
