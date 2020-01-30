#pragma once
#include"moneyclass.h"
#include<iostream>

using namespace std;

void TotalRevenue::getSumIncome(double income)
{
    this->TotalIncome += income;
}

void TotalCosts::getSumSpending(double spending)
{
    this->TotalSpending += spending;
}

double RestOfMoney::SumRest()
{
    this->Rest =this->TotalIncome+this->TotalSpending+this->TotalFutureSpending;
    return this->Rest;
}
