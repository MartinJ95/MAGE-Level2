#include "StackDebugging.h"

StackFunctionLog::StackFunctionLog(std::string functionName) : m_beginTime(clock()), m_functionName(functionName)
{
}

StackDebugging::StackDebugging() : m_LogFile("Logger.txt", std::fstream::in | std::fstream::out), m_loggedFunctions()
{
	if (!m_LogFile.is_open())
	{
		std::cout << "debbugger failed" << std::endl;
	}
}

void StackDebugging::LogFunction(const std::string &functionName)
{
	StackFunctionLog log{ functionName };
	m_loggedFunctions.emplace(std::move(log));
}

void StackDebugging::PopFunction()
{
	if (m_LogFile.is_open())
	{
		m_LogFile << m_loggedFunctions.top().m_functionName << " time elapsed: " << std::to_string(clock() - m_loggedFunctions.top().m_beginTime) << "ms" << "\n";
	}
	m_loggedFunctions.pop();
}

StackDebugging* StackDebugging::GetInstance()
{
	if (instance != nullptr)
	{
		return instance;
	}
	instance = new StackDebugging();
	return instance;
}

void StackDebugging::DestroyInstance()
{
	delete instance;
}

StackDebugging* StackDebugging::instance = nullptr;