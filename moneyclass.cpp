#pragma once
#include"moneyclass.h"
#include<iostream>

using namespace std;


void IncomeMoney::ShowIncomeMoney()
{
    cout << this->IncomeCategory << " " << this->price << " ���"<< endl;
}

void TotalRevenue::ShowListIncome(IncomeMoney *arr, int size)
{
    for(int i = 0;i < size; i++)
    {
        cout << arr[i].IncomeCategory << " " << arr[i].price <<" ���"<< endl;
    }
}

void SpendingMoney::ShowSpendingMoney()
{
    cout << this->CostCategory << " " << this->price <<" ���" << endl;
}

double RestOfMoney::SumRest()
{
    this->Rest = income + spend + futureSpend;
    return this->Rest;
}
