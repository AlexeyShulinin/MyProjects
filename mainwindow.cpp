#pragma once
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "moneyclass.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMessageBox::information(this,"Ввод","Ввод категории начинается с большой буквы\nПри вводе дробного числа ставьте точку");
    MainWindow::DownloadFromFile("D:/QtProjects/Kursach/Current.txt");
    MainWindow::DownloadFromFuturePlans("D:/QtProjects/Kursach/FuturePlans.txt");

    ui->line_Category->setValidator(new QRegExpValidator( QRegExp( "[А-Я][а-я]*\\s*[а-я]*\\s*[а-я]{1,9}") ) );
    QDoubleValidator *doubleValid = new QDoubleValidator(-9999.9,9999.99,2,ui->line_Price);
    doubleValid->setNotation(QDoubleValidator::StandardNotation);
    doubleValid->setLocale(QLocale::C);
    ui->line_Price->setValidator(doubleValid);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DownloadFromFile(QString Path)
{
    QFile File;
    File.setFileName(Path);

    if(!File.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(this,"Error","Файл не открыт!");
        return;
    }

    QTextStream stream(&File); //Направляем поток на данный файл

    QVector<QString> buf(30);
    QString text;
    QStringList price;
    RestOfMoney rest;

    double totalValue = 0;

    ui->listWidget->clear();
    ui->label_Total->setNum(0);
    ui->label_Income->setNum(0);
    ui->label_Spending->setNum(0);


    for(int i=0;!stream.atEnd();i++)
    {

        buf[i] = stream.readLine();
        ui->listWidget->addItem(buf[i]);
        text = buf[i];
        price = text.split(QRegExp("\\s+"),QString::SkipEmptyParts);
        totalValue = price[price.count()-2].toDouble();

        if(totalValue < 0)
        {
            rest.spend += totalValue;
            ui->label_Spending->setNum(rest.spend);
        }
        else if(totalValue > 0)
        {
            rest.income += totalValue;
            ui->label_Income->setNum(rest.income);
        }
    }

    ui->label_Total->setNum(rest.SumRest());

    File.close();

}

void MainWindow::on_pushButton_Add_clicked()
{
    QString out,priceText,category;
    double price = 0;

    TotalCosts Spending;
    TotalRevenue Income;
    RestOfMoney  Total;

    if(!ui->line_Price->hasAcceptableInput() | !ui->line_Category->hasAcceptableInput())
    {
        QMessageBox::information(this,"Error","Некорректное значение\nПопробуйте ввести снова");
        ui->line_Price->clear(); ui->line_Category->clear();
        return;
    }
    price = ui->line_Price->text().toDouble();


    out = ui->line_Category->text() + " " +  ui->line_Price->text() + " руб";
    ui->listWidget->addItem(out);

    Spending.TotalSpending = ui->label_Spending->text().toDouble();
    Income.TotalIncome = ui->label_Income->text().toDouble();

    if(price < 0)
    {
        Spending.TotalSpending += price;
        ui->label_Spending->setNum(Spending.TotalSpending);
    }
    else if(price > 0)
    {
        Income.TotalIncome += price;
        ui->label_Income->setNum(Income.TotalIncome);
    }

    Total.Rest = Income.TotalIncome + Spending.TotalSpending;
    ui->label_Total->setNum(Total.Rest);

    ui->line_Price->clear(); ui->line_Category->clear();
    ui->statusbar->showMessage("Добавлен новый элемент");
}



void MainWindow::on_pushButton_Cansel_clicked()
{
    QVector<QString> list(ui->listWidget->count());
    QString text;
    QStringList price;
    double TotalPrice;

    int i = 0,j=0;

    if(ui->listWidget->count()==0) return;
    for(;i < ui->listWidget->count() ;i++)
    {
        list[i] = ui->listWidget->item(i)->text();
    }
    text = list[i-1];
    price = text.split(QRegExp("\\s+"),QString::SkipEmptyParts);// делим на части;
    TotalPrice = price[price.count()-2].toDouble();//Берём среднюю часть - это наша сумма

    TotalRevenue income;
    RestOfMoney rest;
    TotalCosts spending;

    income.TotalIncome = ui->label_Income->text().toDouble();
    spending.TotalSpending = ui->label_Spending->text().toDouble();
    rest.Rest = ui->label_Total->text().toDouble();

    if(TotalPrice < 0)
    {

        ui->label_Spending->setNum( spending.TotalSpending - TotalPrice);
        ui->label_Total->setNum( rest.Rest - TotalPrice);
    }
    else
    {
        ui->label_Income->setNum( income.TotalIncome - TotalPrice );
        ui->label_Total->setNum( rest.Rest - TotalPrice );
    }
    list.pop_back();
    ui->listWidget->clear();
    while(j < (i-1))
    {
        ui->listWidget->addItem(list[j]);
        j++;
    }
    ui->statusbar->showMessage("Отмена действия");
}

void MainWindow::on_action_Exit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_DownloadCurrent_triggered()
{
    MainWindow::DownloadFromFile("D:/QtProjects/Kursach/Current.txt");
    ui->statusbar->showMessage("Загружен текущий месяц");
}

void MainWindow::on_DownoadPrevious_triggered()
{
    MainWindow::DownloadFromFile("D:/QtProjects/Kursach/Previous.txt");
    ui->statusbar->showMessage("Загружен прошлый месяц");
}

void MainWindow::SaveToFile(QString Path)
{
    QFile File;
    File.setFileName(Path);

    if(!File.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::information(this,"Error","Файл не открыт!");
        return;
    }

    QTextStream stream(&File); //Направляем поток на данный файл
    QVector<QString> buf(30);

    for(int i=0; i < ui->listWidget->count() ;i++)
    {
        buf[i] = ui->listWidget->item(i)->text();
        stream.operator<<(buf[i])<<endl;
    }
    File.close();
}

void MainWindow::on_Save_Current_triggered()
{
    MainWindow::SaveToFile("D:/QtProjects/Kursach/Current.txt");
    ui->statusbar->showMessage("Ваши доходы,расходы за текущий месяц успешно сохранены!");
}


void MainWindow::on_Save_Previous_triggered()
{
    MainWindow::SaveToFile("D:/QtProjects/Kursach/Previous.txt");
    ui->statusbar->showMessage("Ваши доходы,расходы за прошлый месяц успешно сохранены!");
}

void MainWindow::on_pushButton_addToPlans_clicked()
{
    QString out,priceText,category;
    double price = 0;

    TotalCosts Spending;
    TotalRevenue Income;
    RestOfMoney  Total;

    if(!ui->line_Price->hasAcceptableInput() || !ui->line_Category->hasAcceptableInput())
    {
        QMessageBox::information(this,"Error","Ничего не вводилось");
        ui->line_Price->clear();ui->line_Category->clear();
        return;
    }
    price = ui->line_Price->text().toDouble();

    out = ui->line_Category->text() + " " +  ui->line_Price->text() + " руб";
    ui->listWidget_Plans->addItem(out);

    Spending.TotalSpending = ui->label_Spending->text().toDouble();
    Income.TotalIncome = ui->label_Income->text().toDouble();

    ui->line_Price->clear(); ui->line_Category->clear();
    ui->statusbar->showMessage("Категория отложена в планы");
}

void MainWindow::DownloadFromFuturePlans(QString Path)
{
    QFile File;
    File.setFileName(Path);

    if(!File.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(this,"Error","Файл не открыт!");
        return;
    }

    QTextStream stream(&File); //Направляем поток на данный файл

    QVector<QString> buf(30);


    ui->listWidget_Plans->clear();


    for(int i=0;!stream.atEnd();i++)
    {

        buf[i] = stream.readLine();
        ui->listWidget_Plans->addItem(buf[i]);
    }

    File.close();

}

void MainWindow::on_DownloadFuturePlans_triggered()
{
    MainWindow::DownloadFromFuturePlans("D:/QtProjects/Kursach/FuturePlans.txt");
    ui->statusbar->showMessage("Ваши запланированные затраты успешно загружены!");
}

void MainWindow::on_SaveFuturePlans_triggered()
{
    QFile File;
    File.setFileName("D:/QtProjects/Kursach/FuturePlans.txt");

    if(!File.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::information(this,"Error","Файл не открыт!");
        return;
    }

    QTextStream stream(&File); //Направляем поток на данный файл
    QVector<QString> buf(30);

    for(int i=0; i < ui->listWidget_Plans->count() ;i++)
    {
        buf[i] = ui->listWidget_Plans->item(i)->text();
        stream.operator<<(buf[i])<<endl;
    }
    File.close();
    ui->statusbar->showMessage("Ваши запланированные затраты успешно сохранены!");
}

void MainWindow::transferToMainList(QString text)
{
    QStringList price;
    double TotalPrice;

    ui->listWidget->addItem(text);
    price = text.split(QRegExp("\\s+"),QString::SkipEmptyParts);// делим на части;
    TotalPrice = price[price.count()-2].toDouble();//Берём среднюю часть - это наша сумма

    TotalRevenue income;
    RestOfMoney rest;
    TotalCosts spending;

    income.TotalIncome = ui->label_Income->text().toDouble();
    spending.TotalSpending = ui->label_Spending->text().toDouble();
    rest.Rest = ui->label_Total->text().toDouble();

    if(TotalPrice < 0)
    {

        ui->label_Spending->setNum( spending.TotalSpending + TotalPrice);
        ui->label_Total->setNum( rest.Rest + TotalPrice);
    }
    else
    {
        ui->label_Income->setNum( income.TotalIncome + TotalPrice );
        ui->label_Total->setNum( rest.Rest + TotalPrice );
    }


}
void MainWindow::on_add_To_MainList_clicked()
{
    QVector<QString> list(ui->listWidget_Plans->count());
    QString text;

    for(int i=0,j=0;i<=ui->listWidget_Plans->count();i++)
    {
        if(ui->listWidget_Plans->item(i)->text() == ui->listWidget_Plans->currentItem()->text())
        {
            text = ui->listWidget_Plans->item(i)->text();
            MainWindow::transferToMainList(text);
            i++;
            continue;
        }

        list[j] = ui->listWidget_Plans->item(i)->text();
        j++;
    }
    ui->listWidget_Plans->clear();
    for(int i = 0;i <= (list.count()-2);i++)
    {
        ui->listWidget_Plans->addItem(list[i]);
    }
    ui->statusbar->showMessage("Категория перенесена");
}

void MainWindow::on_transferAll_clicked()
{
    QVector<QString> list(ui->listWidget_Plans->count());
    QString text;

    for(int i=0;i<ui->listWidget_Plans->count();i++)
    {
        text = list[i] = ui->listWidget_Plans->item(i)->text();
        MainWindow::transferToMainList(text);
    }
    ui->listWidget_Plans->clear();
    ui->statusbar->showMessage("Все категории перенесены");
}
