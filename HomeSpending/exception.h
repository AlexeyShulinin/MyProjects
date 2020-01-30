#ifndef EXCEPTION_H
#define EXCEPTION_H
#include<iostream>
#include<QFile>
#include "mainwindow.h"

using namespace std;

class Exception
{
private:
    static void isOpenCorrect(QFile&);
    static void isSaved(QFile&);
public:
    friend class MainWindow;
};



#endif // EXCEPTION_H
