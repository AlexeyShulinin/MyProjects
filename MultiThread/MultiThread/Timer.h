#pragma once
#include <chrono>
#include <iostream>

using namespace std;

class Timer
{
public:
	Timer();
	~Timer();
private:
	chrono::time_point<chrono::steady_clock> start, end;
};

