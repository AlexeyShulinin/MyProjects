#pragma once
#ifndef MONEYCLASS_H
#define MONEYCLASS_H
#include <iostream>
#include<string>


#endif // MONEYCLASS_H

using namespace std;

class MoneyClass//базовый класс
{
public:
    double price;
    MoneyClass(){price = 0;}
    MoneyClass(double price) { this->price = price ;}
};

class IncomeMoney : public MoneyClass //класс-наслденик для прибыли
{
public:
    string IncomeCategory;
    IncomeMoney(){}
    IncomeMoney(string Category,double cost)
    {
        (MoneyClass(cost));
        this->IncomeCategory = Category;
    }

};

class SpendingMoney : public MoneyClass //класс-наследник для затрат
{
public:
    string CostCategory;

    SpendingMoney(){}
    SpendingMoney(string Category,double cost)
    {
        (MoneyClass(cost));
        this->CostCategory = Category;
    }
};

class FutureSpendingMoney : public SpendingMoney //класс-наследник для подсчёта будущих затрат
{
public:
    double TotalFutureSpending;

    FutureSpendingMoney(){TotalFutureSpending = 0;}
};

class TotalRevenue : public MoneyClass //класс-наследник для подсчёта прибыли
{
public:
    double TotalIncome;

    TotalRevenue()
    {
        TotalIncome = 0;
    }
    void getSumIncome(double income);

};

class TotalCosts : public SpendingMoney //класс-наследник для подсчёта затрат
{
public:
    double TotalSpending;

    TotalCosts()
    {
        TotalSpending = 0;
    }
    void getSumSpending(double spending);
};

//класс-наследник для подсчёта остатка
class RestOfMoney : virtual public TotalCosts,virtual public TotalRevenue,virtual public FutureSpendingMoney
{
public:
    double Rest;

    RestOfMoney(){Rest = 0;}
    RestOfMoney(double income,double spend,double futureSpending)
    {
        this->TotalIncome = income;
        this->TotalSpending = spend;
        this->TotalFutureSpending = futureSpending;
    }

    double SumRest();

};
