#include "stdafx.h"
#include <iostream>
#include "LogSystem.h"

LogSystem::LogSystem()
{
}


LogSystem::~LogSystem()
{
}

void LogSystem::log(std::string info)
{
	std::cout<<info<< std::endl;
}

void LogSystem::startUp()
{
}

void LogSystem::shutDown()
{
}
