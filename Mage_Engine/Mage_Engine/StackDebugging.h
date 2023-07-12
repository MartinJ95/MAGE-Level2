#pragma once
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <stack>

struct StackFunctionLog
{
friend class StackDebugging;
private:
	StackFunctionLog(std::string functionName);
public:
	const std::clock_t m_beginTime;
	const std::string m_functionName;
};

class StackDebugging
{
public:
	StackDebugging();
	void LogFunction(const std::string &functionName);
	void PopFunction();
	static StackDebugging* GetInstance();
	static void DestroyInstance();

protected:
	std::fstream m_LogFile;
	std::stack<StackFunctionLog> m_loggedFunctions;
	static StackDebugging* instance;
};

