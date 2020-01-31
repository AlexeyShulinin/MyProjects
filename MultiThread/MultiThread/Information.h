#pragma once
#include <iostream>
#include <thread>
#include <fstream>
#include <string>

using namespace std;

class Information
{
private:

	string name;
	int group, amount_of_books;

public:
	Information(const string file_name);
	~Information();


	ifstream lib_info;
	string file_name;

	void SearchPerson(const string name_of_person);
	void FileOpened();
	bool CheckFile();
	
};

