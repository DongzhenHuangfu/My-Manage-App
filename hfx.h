#ifndef HFX_H
#define HFX_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class HFX; }
QT_END_NAMESPACE

class HFX : public QMainWindow
{
    Q_OBJECT

public:
    HFX(QWidget *parent = nullptr);
    ~HFX();

private:
    Ui::HFX *ui;
};
#endif // HFX_H
