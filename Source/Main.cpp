#include <windows.h>
#include <string>
#include <ctime>
#include <thread>
#include <map>
#include <array>

#include "Extra/Logger.h"
#include "WindowHelper.h"

#define VERSION "1.0"
HWND g_hWnd = nullptr;

std::map< HWND, RECT > windowSizes;
BOOL CALLBACK EnumWindowCallback_SaveSizes( HWND hWnd, LPARAM lParam )
{
	DWORD lpdwProcessId;

	if( hWnd == GetDesktopWindow() || hWnd == GetShellWindow() )
		return TRUE;

	GetWindowThreadProcessId( hWnd, &lpdwProcessId );
	if( lpdwProcessId != lParam )
	{
		RECT rect;
		
		if( GetWindowRect( hWnd, &rect ) > 0 ) 
			windowSizes.insert_or_assign( hWnd, rect );
	}

	return TRUE;
}

BOOL CALLBACK EnumWindowCallback_GetWindow( HWND hWnd, LPARAM lParam )
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId( hWnd, &lpdwProcessId );
	if( lpdwProcessId == lParam )
	{
		g_hWnd = hWnd;
		return FALSE;
	}

	return TRUE;
}

bool IsFullscreenAppRunning()
{
	QUERY_USER_NOTIFICATION_STATE state;
	HRESULT result = SHQueryUserNotificationState( &state );

	if( result == S_OK )
		return state == QUNS_RUNNING_D3D_FULL_SCREEN;

	return false;
}

int main( int argc, char** argv )
{
	std::array< std::string, 4 > defaultProcessList{ "RainbowSix.exe", "witcher3.exe", "bf4.exe", "csgo.exe" };
	bool useDefaultProcessList = false;

	Log::printn( "Fullscreen Window Fixer. Version: %s", VERSION );
	if( argc < 2 ) {
		Log::errorn( "Invalid arguments passed" );
		Log::errorn( "Usage: fwf [exeName]" );

		// Append default arguments to the list.
		// TODO: Add config file where this could be defined without recompiling.
		std::string processListStr;

		for( auto s : defaultProcessList )
			processListStr += s + ", ";

		// Pop last two characters ( ", " )
		processListStr.erase( processListStr.length() - 2, 2 );

		Log::printn( "Using default process list: %s", processListStr.c_str() );

		useDefaultProcessList = true;
		//return 1337;
	}

	std::string procName;

	if( !useDefaultProcessList ) {
		std::string procName( argv[ 1 ] );
		Log::printn( "Target Window is %s", procName.c_str() );
		Log::printn( "This program will exit when the process exits or with F1 keystroke." );
	}

	Log::printn( "Looking for the target window..." );

	bool windowFound = false, isFullscreen = true;
	WindowHelper::ProcessInfo procInfo;
	auto clock = std::clock();

	while( !GetAsyncKeyState( VK_F1 ) & 1 )
	{
		EnumWindows( EnumWindowCallback_SaveSizes, 0 );
		while( !windowFound )
		{
			// If not arguments passed, iterate default list until process is found.
			if( useDefaultProcessList )
			{
				for( auto& proc : defaultProcessList ) {
					if( WindowHelper::IsProcessRunning( proc ) )
					{
						// A quick fix to not check this if statement after the process have been found.
						useDefaultProcessList = false;
						procName = proc;

						break;
					}
				}
			}

			if( WindowHelper::IsProcessRunning( procName ) ) {
				
				Log::printn( "Process window found(%s). Saving current positions.", procName.c_str() );
				WindowHelper::GetProcessInformation( procName, procInfo );

				EnumWindows( EnumWindowCallback_GetWindow, procInfo.procId );
				windowFound = true;
			}
			
			std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
		}


		if( !IsFullscreenAppRunning() )
		{
			// Some games can have problems, so add a delay.
			std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
			if( IsFullscreenAppRunning() )
				continue;

			// Save positions every 5 seconds.
			if( std::clock() - clock > 5000 ) {
				EnumWindows( EnumWindowCallback_SaveSizes, 0 );

				if( !WindowHelper::IsProcessRunning( procName ) )
				{
					Log::warnn( "Process no longer detected running! Exiting." );
					std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
					return 0;
				}

				clock = std::clock();
			}

			if( !isFullscreen )
			{
				Log::printn( "Game minimize detected! Restoring window positions!" );
				for( auto obj : windowSizes )
				{
					RECT rect = obj.second;
					MoveWindow( obj.first, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE );
				}

				isFullscreen = true;
			}
		} else {

			if( isFullscreen ) {
				Log::printn( "Game maximize detected! Saving positions!" );
				isFullscreen = false;
			}
		}

		std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
	}

	Log::printn( "Exiting out of process!" );
	return 0;
}