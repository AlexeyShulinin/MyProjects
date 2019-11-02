#pragma once
#ifndef MONEYCLASS_H
#define MONEYCLASS_H
#include <iostream>
#include<string>

#endif // MONEYCLASS_H

using namespace std;

class MoneyClass //������� �����
{
public:
    double price;
    MoneyClass(){price = 0;}
    MoneyClass(double price) { this->price = price ;}
};

class IncomeMoney : public MoneyClass //�����-��������� �������� �� ����������� �����
{
public:
    string IncomeCategory;

    IncomeMoney(){}
    IncomeMoney(string Category,double cost)
    {
        (MoneyClass(cost));
        this->IncomeCategory = Category;
    }
    void ShowIncomeMoney();
};

class SpendingMoney : public MoneyClass //�����-��������� �������� �� �������
{
public:
    string CostCategory;

    SpendingMoney(){}
    SpendingMoney(string Category,double cost)
    {
        (MoneyClass(cost));
        this->CostCategory = Category;
    }
    void ShowSpendingMoney();
};

class FutureSpendingMoney : public SpendingMoney
{
public:
    int amount;
    double TotalFutureSpending;

    FutureSpendingMoney(){TotalFutureSpending = 0; amount = 0;}
    int SumFutureSpending();
};

class TotalRevenue : public IncomeMoney //����� ����� �����
{
public:
    double TotalIncome;

    TotalRevenue()
    {
        TotalIncome = 0;
    }
    int SumIncomeMoney();
    void ShowListIncome(IncomeMoney *arr,int size);
};

class TotalCosts : public SpendingMoney //����� ����� ��������
{
public:
    double TotalSpending;

    TotalCosts()
    {
        TotalSpending = 0;
    }
    int SumSpendingMoney();
};

class RestOfMoney : virtual public TotalCosts,virtual public TotalRevenue
{
public:
    double Rest;
    double income;
    double spend;
    double futureSpend;

    RestOfMoney(){Rest = 0;income = 0;spend = 0; futureSpend = 0;}
    RestOfMoney(double income,double spend,double futureSpending)
    {
        this->income = income;
        this->spend = spend;
        this->futureSpend = futureSpending;
    }

    double SumRest();



};
