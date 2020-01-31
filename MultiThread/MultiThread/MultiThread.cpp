#include "pch.h"
#include <iostream>
#include "Information.h"
#include "Timer.h"

using namespace std;


/*
Example of multithreading
There are three threads, which works whith files for searching information about student(name,group,books)
*/

int main()
{
	setlocale(LC_ALL, "rus");

	Timer time;
	Information first_lib_info("FirstLib.txt"),second_lib_info("SecondLib.txt"),third_lib_info("ThirdLib.txt");

	//first_lib_info.SearchPerson("Aleksey");
	//second_lib_info.SearchPerson("Andrew");
	//third_lib_info.SearchPerson("Yulia");

	thread th1(&Information::SearchPerson, &first_lib_info, "Aleksey");
	thread th2(&Information::SearchPerson, &second_lib_info, "Andrew");
	thread th3(&Information::SearchPerson, &third_lib_info, "Yulia");


	th1.join();
	th2.join();
	th3.join();

	return 0;
}


