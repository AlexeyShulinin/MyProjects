#pragma once
#include "mainwindow.h"
#include "instruction.h"
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
    QMessageBox::information(this,"О программе","Программа создана,для планирования и"
                                         "\n отслеживания доходов и расходов семьи"
                                         "\n      Powered by Shulinin Alexey");
    MainWindow::DownloadFromFile("./DataFiles/Current.txt");
    MainWindow::DownloadFromFuturePlans("./DataFiles/FuturePlans.txt");

    ui->line_Category->setValidator(new QRegExpValidator( QRegExp( "[А-Я][а-я]*\\s*[а-я]*\\s*[а-я]{1,9}") ) );
    QDoubleValidator *doubleValid = new QDoubleValidator(-999999.9,999999.99,2,ui->line_Price);
    doubleValid->setNotation(QDoubleValidator::StandardNotation);
    doubleValid->setLocale(QLocale::C);
    ui->line_Price->setValidator(doubleValid);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkSpending()
{
    if(ui->label_TotalWithFuture->text().toDouble() < 0)
    {
        ui->label_TotalWithFuture->setStyleSheet("color : red");
    }
    else ui->label_TotalWithFuture->setStyleSheet(("color : black"));
    if(ui->label_Total->text().toDouble() < 0)
    {
        ui->label_Total->setStyleSheet("color : red");
    }
    else ui->label_Total->setStyleSheet(("color : black"));
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
    ui->label_TotalWithFuture->setNum(0);


    for(int i=0;!stream.atEnd();i++)
    {

        buf[i] = stream.readLine();
        ui->listWidget->addItem(buf[i]);
        text = buf[i];
        price = text.split(QRegExp("\\s+"),QString::SkipEmptyParts);
        totalValue = price[price.count()-2].toDouble();

        if(totalValue < 0)
        {

            rest.getSumSpending(totalValue);
            ui->label_Spending->setNum(rest.TotalSpending);
        }
        else if(totalValue > 0)
        {

            rest.getSumIncome(totalValue);
            ui->label_Income->setNum(rest.TotalIncome);
        }
    }

    ui->label_Total->setNum(rest.SumRest());
    ui->label_TotalWithFuture->setNum(rest.SumRest());
    MainWindow::checkSpending();
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
        Spending.getSumSpending(price);
        ui->label_Spending->setNum(Spending.TotalSpending);
    }
    else if(price > 0)
    {
        Income.getSumIncome(price);
        ui->label_Income->setNum(Income.TotalIncome);
    }

    Total.TotalIncome = Income.TotalIncome; Total.TotalSpending = Spending.TotalSpending;
    Total.SumRest();
    ui->label_Total->setNum(Total.Rest);
    ui->label_TotalWithFuture->setNum(Total.Rest - ui->label_FutureSpending->text().toDouble());

    MainWindow::checkSpending();

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
    }
    else
    {
        ui->label_Income->setNum( income.TotalIncome - TotalPrice );
    }
    ui->label_Total->setNum( rest.Rest - TotalPrice );
    ui->label_TotalWithFuture->setNum(ui->label_Total->text().toDouble() + ui->label_FutureSpending->text().toDouble());

    MainWindow::checkSpending();

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
    MainWindow::DownloadFromFile("./DataFiles/Current.txt");
    ui->statusbar->showMessage("Загружен текущий месяц");
}

void MainWindow::on_DownoadPrevious_triggered()
{
    MainWindow::DownloadFromFile("./DataFiles/Previous.txt");
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
    MainWindow::SaveToFile("./DataFiles/Current.txt");
    ui->statusbar->showMessage("Ваши доходы,расходы за текущий месяц успешно сохранены!");
}


void MainWindow::on_Save_Previous_triggered()
{
    MainWindow::SaveToFile("./DataFiles/Previous.txt");
    ui->statusbar->showMessage("Ваши доходы,расходы за прошлый месяц успешно сохранены!");
}

void MainWindow::on_pushButton_addToPlans_clicked()
{
    QString out,priceText,category;
    double price = 0;

    FutureSpendingMoney Spending;

    if(!ui->line_Price->hasAcceptableInput() || !ui->line_Category->hasAcceptableInput())
    {
        QMessageBox::information(this,"Error","Ничего не вводилось");
        ui->line_Price->clear();ui->line_Category->clear();
        return;
    }
    price = ui->line_Price->text().toDouble();

    out = ui->line_Category->text() + " " +  ui->line_Price->text() + " руб";
    ui->listWidget_Plans->addItem(out);
    Spending.TotalFutureSpending = ui->label_FutureSpending->text().toDouble() + ui->line_Price->text().toDouble();
    ui->label_FutureSpending->setNum(Spending.TotalFutureSpending);

    ui->label_TotalWithFuture->setNum(ui->label_Total->text().toDouble() + Spending.TotalFutureSpending);

    MainWindow::checkSpending();
    ui->line_Price->clear(); ui->line_Category->clear();
    ui->statusbar->showMessage("Категория отложена в планы");
}


void MainWindow::DownloadFromFuturePlans(QString Path)
{
    QFile File;
    QString text;
    QStringList price;
    FutureSpendingMoney plan;
    File.setFileName(Path);

    if(!File.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::information(this,"Error","Файл не открыт!");
        return;
    }

    QTextStream stream(&File); //Направляем поток на данный файл

    QVector<QString> buf(30);


    ui->listWidget_Plans->clear();
    ui->label_FutureSpending->setNum(0);


    for(int i=0;!stream.atEnd();i++)
    {
        buf[i] = stream.readLine();
        ui->listWidget_Plans->addItem(buf[i]);
        text = buf[i];
        price = text.split(QRegExp("\\s+"),QString::SkipEmptyParts);
        plan.TotalFutureSpending =  price[price.count()-2].toDouble();
        ui->label_FutureSpending->setNum(ui->label_FutureSpending->text().toDouble()+plan.TotalFutureSpending);
        ui->label_TotalWithFuture->setNum(ui->label_Total->text().toDouble() + ui->label_FutureSpending->text().toDouble());
    }
    MainWindow::checkSpending();
    File.close();

}


void MainWindow::on_DownloadFuturePlans_triggered()
{
    MainWindow::DownloadFromFuturePlans("./DataFiles/FuturePlans.txt");
    ui->statusbar->showMessage("Ваши запланированные затраты успешно загружены!");
}

void MainWindow::on_SaveFuturePlans_triggered()
{
    QFile File;
    File.setFileName("./DataFiles/FuturePlans.txt");

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
        spending.getSumSpending(TotalPrice);
        ui->label_Spending->setNum(spending.TotalSpending);
        //ui->label_Spending->setNum( spending.TotalSpending + TotalPrice);
    }
    else
    {
        income.getSumIncome(TotalPrice);
        ui->label_Income->setNum(income.TotalIncome);
        //ui->label_Income->setNum( income.TotalIncome + TotalPrice );
    }
    ui->label_Total->setNum( rest.Rest + TotalPrice );
    ui->label_FutureSpending->setNum(ui->label_FutureSpending->text().toDouble()-TotalPrice);
    ui->label_TotalWithFuture->setNum(ui->label_Total->text().toDouble()+ui->label_FutureSpending->text().toDouble());
    MainWindow::checkSpending();

}
void MainWindow::on_add_To_MainList_clicked()
{
    QVector<QString> list(ui->listWidget_Plans->count());
    QString text;

    if(!ui->listWidget_Plans->currentItem()) return;
    for(int i=0,j=0;i<ui->listWidget_Plans->count();i++)
    {
        if(ui->listWidget_Plans->item(i)->text() == ui->listWidget_Plans->currentItem()->text())
        {
            text = ui->listWidget_Plans->item(i)->text();
            MainWindow::transferToMainList(text);
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

    if(ui->listWidget_Plans->count() == 0) return;
    for(int i=0;i<ui->listWidget_Plans->count();i++)
    {
        text = list[i] = ui->listWidget_Plans->item(i)->text();
        MainWindow::transferToMainList(text);
    }
    ui->listWidget_Plans->clear();
    ui->statusbar->showMessage("Все категории перенесены");
}

void MainWindow::on_Exit_clicked()
{
    QApplication::quit();
}

void MainWindow::on_Instruction_triggered()
{
    Instruction instruction;
    instruction.setWindowTitle("Manual");
    instruction.open();
    while(1){if(!instruction.exec()) break;}
}

void MainWindow::on_DeletePlan_clicked()
{
    QVector<QString> list(ui->listWidget_Plans->count()-1);
    QString text;
    QStringList price;
    double TotalPrice;

    int i = 0,j=0;

    if(ui->listWidget_Plans->count()==0) return;
    for(;i <= list.count();i++)
    {
        if(ui->listWidget_Plans->item(i)->text() == ui->listWidget_Plans->currentItem()->text()){continue;}
        list[j] = ui->listWidget_Plans->item(i)->text();
        j++;
    }
    text = ui->listWidget_Plans->currentItem()->text();
    price = text.split(QRegExp("\\s+"),QString::SkipEmptyParts);// делим на части;
    TotalPrice = price[price.count()-2].toDouble();//Берём среднюю часть - это наша сумма

    ui->label_TotalWithFuture->setNum(ui->label_TotalWithFuture->text().toDouble() - TotalPrice);
    ui->label_FutureSpending->setNum(ui->label_FutureSpending->text().toDouble() - TotalPrice);
    MainWindow::checkSpending();

    ui->listWidget_Plans->clear();
    j = 0;
    while(j < list.count())
    {
        ui->listWidget_Plans->addItem(list[j]);
        j++;
    }
    ui->statusbar->showMessage("Из плана удалена категория");
}
