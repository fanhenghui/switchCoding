#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFileDialog>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //连接信号和槽
    connect(ui->chooseBtn, SIGNAL(clicked()), this, SLOT(on_chooseBtn_slot()));
    connect(ui->sureBtn, SIGNAL(clicked()), this, SLOT(on_sureBtn_slot()));
}

void Widget::on_chooseBtn_slot()
{
//qDebug() << "打开文件按钮!";
    //TODO 打开文件窗口，选择要转换的文件夹
//    filePath = "test.txt";
    filePath = QFileDialog::getExistingDirectory(this, tr("open director"));
//qDebug() << "path:" << filePath;
}

/*
 * 递归遍历文件夹里面的文件，然后改变编码
 */
void Widget::recursion(QString filePath)
{
    QDir dir(filePath);
    //文件列表
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    for(int i = 0; i < file_list.size(); i++)
    {
        QString path = file_list.at(i).absoluteFilePath();
        //转换编码
        switchCode(path);
    }
    //文件夹列表
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < folder_list.size(); i++)
    {
        QString path = folder_list.at(i).absoluteFilePath();
        //递归遍历文件
        recursion(path);
    }

}

void Widget::on_sureBtn_slot()
{
//qDebug() << "确定按钮!";
//    switchCode();
    oldCode = ui->oldEdit->text();
    newCode = ui->newEdit->text();
    if( oldCode.isEmpty() || newCode.isEmpty() )
    {
        QMessageBox::information(this, tr("提示"), tr("请填写编码!"));
        return;
    }
    if( filePath.isEmpty() )
    {
        QMessageBox::information(this, tr("提示"), tr("请选择待转换编码的文件夹!"));
        return;
    }
    recursion(filePath);
    //TODO 提示转换成功
//qDebug() << "转换成功!";
    QMessageBox::information(this, tr("恭喜"), tr("转码成功!"));
}

/*
 * 转换文件的编码
 */
void Widget::switchCode(QString filePath)
{
//qDebug() << "转化开始";
    //获取编码
//    QString oldCode = ui->oldEdit->text();
//    QString newCode = ui->newEdit->text();
    QFile file(filePath);
    if( !file.open(QIODevice::ReadOnly | QIODevice::Truncate))
    {
        return;
    }
    QTextStream in(&file);
    in.setCodec(oldCode.toUtf8().data());
    QString content = in.readAll();
//qDebug() << "content:" << content;
    file.resize(0);
    file.remove();
    file.close();
    QFile outFile(filePath);
    if( !outFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return;
    }
    QTextStream out(&outFile);
    out.setCodec(newCode.toUtf8().data());
    out << content;
    outFile.close();
}

Widget::~Widget()
{
    delete ui;
}
