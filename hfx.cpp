#include "hfx.h"
#include "ui_hfx.h"

HFX::HFX(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HFX)
{
    ui->setupUi(this);
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
