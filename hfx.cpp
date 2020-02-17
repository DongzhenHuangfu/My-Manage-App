#include "hfx.h"
#include "ui_hfx.h"
#include "Python.h"

HFX::HFX(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HFX)
{
    ui->setupUi(this);
    update();
}

HFX::~HFX()
{
    delete ui;
}


void HFX::on_ComboType_currentIndexChanged(const QString &TypeName)
{
    // 自动根据商品种类更改商品单价
    if (TypeName == QString("泡脚方"))
    {
        ui->SpinPrice->setValue(6);
    }
    else if (TypeName == QString("定制茶"))
    {
        ui->SpinPrice->setValue(10);
    }
    else if (TypeName == QString("气血茶"))
    {
        ui->SpinPrice->setValue(5);
    }
    else if (TypeName == QString("益气茶"))
    {
        ui->SpinPrice->setValue(5);
    }
    else if (TypeName == QString("消火茶"))
    {
        ui->SpinPrice->setValue(5);
    }
    else if (TypeName == QString("润肺茶"))
    {
        ui->SpinPrice->setValue(9);
    }
    else if (TypeName == QString("减脂茶"))
    {
        ui->SpinPrice->setValue(9);
    }
    else if (TypeName == QString("安神茶"))
    {
        ui->SpinPrice->setValue(9);
    }
}

void HFX::update()
{
    /// 读取表格信息
    NewSheet.Name = ui->LineName->text().toStdString();
    NewSheet.Date = ui->SpinDate->text().toInt();
    NewSheet.Deal = ui->SpinDeal->text().toInt();
    NewSheet.Post = ui->SpinPost->text().toFloat();
    NewSheet.Type = ui->ComboType->currentText().toStdString();
    NewSheet.Amount = ui->SpinAmount->text().toInt();
    NewSheet.Price = ui->SpinPrice->text().toFloat();

    NewSheet.Total = (NewSheet.Amount - NewSheet.Deal) * NewSheet.Price + NewSheet.Post;
    ui->LineTotal->setText(QString("%1").arg(NewSheet.Total));

    /// 数据有变化后提交按钮的字体颜色变红
    ui->PushButtonSubmit->setStyleSheet("color:red");
}

void HFX::on_SpinPrice_valueChanged()
{
    update();
}

void HFX::on_SpinAmount_valueChanged()
{
    update();
}

void HFX::on_SpinDeal_valueChanged()
{
    update();
}

void HFX::on_SpinPost_valueChanged()
{
    update();
}

void HFX::on_PushButtonSubmit_clicked()
{
    update();
    AllSheet.push_back(NewSheet);
}

void HFX::save_data()
{
    Py_Initialize();

    PyObject * pModule = NULL;
    PyObject * pFunc = NULL;

    if (!Py_IsInitialized())
    {
        ui->PushButtonSubmit->setStyleSheet("color:blue");
    }
    else
    {
        ui->PushButtonSubmit->setStyleSheet("color:green");
    }

    Py_Finalize();
}
