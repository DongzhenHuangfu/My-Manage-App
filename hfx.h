#ifndef HFX_H
#define HFX_H

// #pragma execution_character_set("utf-8")

#include <QMainWindow>
#include <vector>
#include <string>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class HFX; }
QT_END_NAMESPACE


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
    float Price, Post;
    long Total;
}Sheet;

class HFX : public QMainWindow
{
    Q_OBJECT

public:
    HFX(QWidget *parent = nullptr);
    ~HFX();

    /// 按照表格更新数据，计算总价数额
    void update();
    /// 存下表单数据
    void save_data();

private slots:
    void on_ComboType_currentIndexChanged(const QString &TypeName);

    void on_SpinPrice_valueChanged();

    void on_SpinAmount_valueChanged();

    void on_SpinDeal_valueChanged();

    void on_SpinPost_valueChanged();

    void on_PushButtonSubmit_clicked();

private:
    Ui::HFX *ui;
    /// 实时的表单数据
    Sheet NewSheet;
    std::vector<Sheet> AllSheet;
};
#endif // HFX_H
