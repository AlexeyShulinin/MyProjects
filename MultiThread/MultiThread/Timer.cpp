#include "pch.h"
#include "Timer.h"


Timer::Timer()
{
	start = chrono::high_resolution_clock::now();
}


Timer::~Timer()
{
	end = chrono::high_resolution_clock::now();
	chrono::duration<float> duration = end - start;
	cout << "Duration " << duration.count() << " s" << endl;
}
