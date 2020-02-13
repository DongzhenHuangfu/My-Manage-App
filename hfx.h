#ifndef HFX_H
#define HFX_H

#pragma execution_character_set("utf-8")

#include <QMainWindow>
#include <vector>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class HFX; }
QT_END_NAMESPACE

class HFX : public QMainWindow
{
    Q_OBJECT

public:
    HFX(QWidget *parent = nullptr);
    ~HFX();

private slots:
    void on_ComboType_currentIndexChanged(const QString &TypeName);

private:
    Ui::HFX *ui;
};

/// 保存账单信息的结构
///   Name: 购买人名字
///   Type: 购买茶的种类
///   Date: 购买日期
/// Amount: 购买数量
///   Deal: 赠送数量
///  Price: 购买单价
///   Post: 邮费
///  Total：总价
typedef struct
{
    std::string Name, Type;
    int Date, Amount, Deal;
    float Price, Post, Total;
}Sheet;

#endif // HFX_H
