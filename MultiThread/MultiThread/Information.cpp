#include "pch.h"
#include "Information.h"
#include <mutex>
#include <chrono>


Information::Information(const string file_name)
{
	this->file_name = file_name;
	this->amount_of_books = 0;
	this->group = 0;
	this->name = "0";
}


Information::~Information()
{
}

void Information::SearchPerson(const string name_of_person)
{
	if (!CheckFile()) return;
	bool isFound = false;
	for (size_t i = 0; !lib_info.eof() && !isFound; i++)
	{
		this_thread::sleep_for(chrono::milliseconds(1000));
		cout << ".";
		this->lib_info >> this->name >> this->group >> this->amount_of_books;
		if (this->name == name_of_person)
		{
			mutex mt;
			mt.lock();
			cout << "\n\tID thread:" << "\t" << this_thread::get_id() << endl;
			//this_thread::sleep_for(chrono::milliseconds(1000));
			cout << "Student was founded" << endl;
			cout << this->name << "\t" << this->group << "\t" << this->amount_of_books << endl;
			isFound = true;
			mt.unlock();
		};
	}
	if (!isFound) cout << "Student not found" << endl;
	this->lib_info.close();
}

void Information::FileOpened()
{
	this->lib_info.open(this->file_name);
	if (!this->lib_info.is_open()) throw exception("File doesn't opened");
}

bool Information::CheckFile()
{
	try
	{
		this->FileOpened();
	}
	catch (exception &ex)
	{
		cout << ex.what() << endl;
		return false;
	}
	return true;
}

