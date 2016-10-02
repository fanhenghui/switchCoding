#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

    void switchCode(QString);

    void recursion(QString);

    ~Widget();

public slots:
    void on_chooseBtn_slot();

    void on_sureBtn_slot();

private:
    Ui::Widget *ui;
    QString filePath;    //保存打开的文件路径
    QString oldCode;
    QString newCode;
};

#endif // WIDGET_H
