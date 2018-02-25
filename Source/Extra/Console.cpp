#include "Console.hpp"
#include "Logger.h"
#include <iostream>
#include <thread>

Console* Console::pInstance = nullptr;
std::vector< ConsoleExpression > consoleExpressions;

Console::Console( const std::string& console_title )
{
	AllocConsole();
	SetConsoleTitleA( console_title.c_str() );

	freopen_s( &stream_, "conin$", "r", stdin );
	freopen_s( &stream_, "conout$", "w", stdout );
	freopen_s( &stream_, "conout$", "w", stderr );

	SetConsoleCtrlHandler(
		(PHANDLER_ROUTINE)Console::ConsoleEventHandler,
		true
	);

	handlers.push_back( SelfInputHandler );
}

Console::~Console()
{
	terminate = true;
	Sleep( 50 );

	FreeConsole();
	fclose( stream_ );
}

bool Console::ConsoleEventHandler( DWORD dwEvent )
{
	switch( dwEvent )
	{
	case CTRL_CLOSE_EVENT:
	{
		FreeConsole();
		break;
	}
	}

	return true;
}

std::vector<InputHandler>& Console::GetInputHandlers()
{
	return handlers;
}

void Console::operator+=( const InputHandler & handler )
{
	handlers.push_back( handler );
}

void Console::CreateInputHandlerThread()
{
	std::thread( &Console::Listener, this ).detach();
}

void Console::Listener()
{
	char* input = new char[ 0x2000 ];
	while( !terminate )
	{
		WORD prevColor = Log::color( FOREGROUND_RED );

		memset( input, 0, 0x2000 ); // clearing buffer.
		printf( "\n>" );
		std::cin.getline( input, 0x2000 );

		Log::color( prevColor );

		for( InputHandler hanlder : handlers )
			hanlder( input );
	}
	delete[] input;
}

void Console::DisplayExpressions()
{
	Log::print( "Displaying all commands.\n" );
	// Loop through expressions and check if one matches.
	for( ConsoleExpression& expr : consoleExpressions )
		Log::print( "%s(%lld): %s\n", expr.expression.c_str(), (int64_t)expr.argCount, expr.helpMessage.c_str() );
}

void Console::SelfInputHandler( char* command )
{
	if( !command || strlen( command ) == 0 )
		return;

	char* context = NULL;
	char* token = NULL;

	std::vector< std::string > splitString;

	token = strtok_s( command, " ", &context );
	if( token == nullptr )
		return;

	splitString.push_back( token );
	// TODO: Optimize to check first token first and then decide on further action.
	while( ( token = strtok_s( nullptr, " ", &context ) ) != nullptr )
		splitString.push_back( token );

	if( splitString.size() == 0 || splitString.at( 0 ).empty() )
		return;

	/* Handle some default commands. */
	if( HandleDefaultCommands( splitString.at( 0 ) ) )
		return;

	// Loop through expressions and check if one matches.
	for( ConsoleExpression& expr : consoleExpressions )
	{
		if( splitString.at( 0 ) == expr.expression )
		{
			// Check how many arguments it expects.
			if( expr.argCount == 1 )
			{
				expr.callback( splitString );
				return;

			/* Allow for bigger argument lists. */
			} else if( splitString.size() >= expr.argCount )
			{
				expr.callback( splitString );
				return;
			}

			// Invalid argument count provided.
			Log::warn( "Usage: %s - %s. Got %lld, expected: %lld", expr.expression.c_str(), expr.helpMessage.c_str(), splitString.size(), (int64_t)expr.argCount );
			return;
		}
	}

	/* Expression not found, invalid command. */
	Log::warn( "Command: %s unrecognized!\n", splitString.at( 0 ).c_str() );
}

bool Console::HandleDefaultCommands( std::string command )
{
	if( command == "exit" )
	{
		Destroy();
		return true;
	} else if( command == "help" )
	{
		DisplayExpressions();
		return true;
	} else if( command == "clear" )
	{
		system( "cls" );
		return true;
	}

	return false;
}

void Console::Destroy()
{
	Log::warn( "Destroying console thread!\n" );
	Console::GetInstance()->terminate = true;
}
