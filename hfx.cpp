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

