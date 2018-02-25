#pragma once
#include <stdio.h>
#include <Windows.h>
#include <functional>
#include <vector>

typedef std::function< void( char* ) > InputHandler;
class Console
{
public:
	static Console* GetInstance()
	{
		if( pInstance == nullptr )
			pInstance = new Console();

		return pInstance;
	}

	~Console();
	Console( const std::string& console_title = "Console" );

	static bool WINAPI ConsoleEventHandler( DWORD dwEvent );
	std::vector< InputHandler >& GetInputHandlers();
	void operator+=( const InputHandler& handler );


	void CreateInputHandlerThread();
private:
	void Listener();

	static void SelfInputHandler( char* command );
	static bool HandleDefaultCommands( std::string command );
	static void DisplayExpressions();
	static void Destroy();

private:
	static Console* pInstance;
	bool terminate = false;
	std::vector< InputHandler > handlers;
	FILE* stream_;
};

typedef std::function< void( std::vector< std::string >& ) > ConsoleCallback;
struct ConsoleExpression
{
	std::string expression;

	/* If invalid arguments are provided, display help message. */
	std::string helpMessage;
	ConsoleCallback callback;

	/* Includes the expression + rest of arguments */
	/* If argCount == 1, then unconditionally call the callback.*/
	int argCount;
};

extern std::vector< ConsoleExpression > consoleExpressions;