#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "exception.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    friend class Exception;
private slots:

    void on_pushButton_Add_clicked();

    void on_pushButton_Cansel_clicked();

    void on_action_Exit_triggered();

    void on_DownloadCurrent_triggered();

    void DownloadFromFile(QString Path);

    void on_DownoadPrevious_triggered();

    void SaveToFile(QString Path);

    void on_Save_Current_triggered();

    void on_Save_Previous_triggered();

    void on_pushButton_addToPlans_clicked();

    void DownloadFromFuturePlans(QString path);

    void on_DownloadFuturePlans_triggered();

    void on_SaveFuturePlans_triggered();

    void transferToMainList(QString text);

    void on_add_To_MainList_clicked();

    void on_transferAll_clicked();

    void on_Exit_clicked();

    void checkSpending();

    void on_Instruction_triggered();

    void on_DeletePlan_clicked();

    void on_clear_ListWirdget_clicked();

    void on_clear_list_Plans_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
