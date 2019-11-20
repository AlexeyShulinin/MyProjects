#include "exception.h"
#include<QMessageBox>

using namespace std;

void Exception::isOpenCorrect(QFile &File)
{
    if(!File.open(QFile::ReadOnly | QFile::Text))
    {
        throw exception("Файл не открыт!");
    }
}

void Exception::isSaved(QFile &File)
{
    if(!File.open(QFile::WriteOnly | QFile::Text))
    {
        throw exception("Файл не открыт!\nЗапись невозможна!");
    }
}
