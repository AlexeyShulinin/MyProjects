#include "pch.h"
#include "Information.h"
#include <mutex>
#include <chrono>


Information::Information(const string file_name)
{
	this->file_name = file_name;
}


Information::~Information()
{
}

void Information::SearchPerson(const string name_of_person)
{
	this->lib_info.open(this->file_name);
	for (size_t i = 0; !lib_info.eof(); i++)
	{
		this_thread::sleep_for(chrono::milliseconds(1000));
		cout << ".";
		this->lib_info >> this->name >> this->group >> this->amount_of_books;
		if (this->name == name_of_person)
		{
			mutex mt;
			mt.lock();
			cout << "\n\tID потока:" << "\t" << this_thread::get_id() << endl;
			//this_thread::sleep_for(chrono::milliseconds(1000));
			cout << "Найдено совпадение" << endl;
			cout << this->name << "\t" << this->group << "\t" << this->amount_of_books << endl;
			mt.unlock();
		};
	}
	this->lib_info.close();
}