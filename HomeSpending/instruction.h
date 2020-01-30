#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QFile>
#include <QDialog>
#include <QTextStream>

namespace Ui {
class Instruction;
}

class Instruction : public QDialog
{
    Q_OBJECT

public:
    explicit Instruction(QWidget *parent = nullptr);
    ~Instruction();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Instruction *ui;
    void DownloadFile(QString Path);
};

#endif // INSTRUCTION_H
