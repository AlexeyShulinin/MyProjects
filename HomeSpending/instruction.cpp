#include "instruction.h"
#include "ui_instruction.h"
#include <QMessageBox>

Instruction::Instruction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Instruction)
{
    ui->setupUi(this);
    Instruction::DownloadFile("./DataFiles/Instruction.txt");
}

Instruction::~Instruction()
{
    delete ui;
}

void Instruction::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Instruction::DownloadFile(QString Path)
{
     QFile File;
     QString buf;
     File.setFileName(Path);

     if(!File.open(QFile::ReadOnly | QFile::Text))
     {
         QMessageBox::information(this,"Error","Файл не открыт!");
         return;
     }
     QTextStream stream(&File);
     buf = stream.readAll();

     ui->textEdit->setText(buf);

     File.close();
}

