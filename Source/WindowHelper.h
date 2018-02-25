#pragma once
#include <windows.h>
#include <string>

namespace WindowHelper
{
	struct ProcessInfo {
		std::string exeName;
		int procId;
	};

	bool IsProcessRunning( const std::string& procName );
	bool GetProcessInformation( const std::string& procName, ProcessInfo& procInfo );

	bool IsWindowTopmost( HWND hWnd );
	bool IsWindowFullscreen( HWND hWnd );
}