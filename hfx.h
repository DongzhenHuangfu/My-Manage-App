#ifndef HFX_H
#define HFX_H

// #pragma execution_character_set("utf-8")

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
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
    float Price, Post, Discount;
    long Total;
}Sheet;

/// 定义排序规则：
/// 日期从小到大，
/// 若日期一样则名字从小到大，
/// 若名字一样则类别从小到大，
/// 若类别一样则价格从大到小，
/// 若价格一样则数量从大到小，
/// 若数量一样则赠送从小到大，
/// 若赠送一样则折扣从小到大
/// 若折扣一样则邮费从大到小
bool sort_Sheet_Date(const Sheet &p1, const Sheet &p2);
bool sort_Sheet_Name(const Sheet &p1, const Sheet &p2);
bool sort_Sheet_Type(const Sheet &p1, const Sheet &p2);
bool sort_Sheet_Price(const Sheet &p1, const Sheet &p2);
bool sort_Sheet_Amount(const Sheet &p1, const Sheet &p2);
bool sort_Sheet_Deal(const Sheet &p1, const Sheet &p2);
bool sort_Sheet_Discount(const Sheet &p1, const Sheet &p2);
bool sort_Sheet_Post(const Sheet &p1, const Sheet &p2);

class MyMessageBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit MyMessageBox(QWidget *parent = 0);
    void setMySize(int width, int length);
signals:
public slots:

protected:
    void resizeEvent(QResizeEvent *event);
private:
    int width_, length_;
};

class HFX : public QMainWindow
{
    Q_OBJECT

public:
    HFX(QWidget *parent = nullptr);
    ~HFX();

    /// 按照表格更新数据，计算总价数额
    void update();
    void set_price(const QString &TypeName);

private slots:
    void on_ComboType_currentIndexChanged(const QString &TypeName);

    void on_SpinPrice_valueChanged();

    void on_SpinAmount_valueChanged();

    void on_SpinDeal_valueChanged();

    void on_SpinPost_valueChanged();

    void on_PushButtonSubmit_clicked();

    void on_PushButtonSave_clicked();

    void on_PushButtonSavePath_clicked();

    void on_SpinDate_textChanged();

    void on_LineName_textChanged();

    void on_SpinDiscount_valueChanged();

    void on_PushButtonRead_clicked();

private:
    Ui::HFX *ui;
    /// 收入面板变量
    // 实时的表单数据
    Sheet NewSheet;
    std::vector<Sheet> AllSheet;
    bool LoadFlag, Changed;

    /// 支出面板变量
    // 实时表单数据
    Sheet OutNewSheet_;
    // 所有表单数据
    std::vector<Sheet> OutAllSheet_;
    // 状态标志
    bool OutLoaded_, OutChanged_;
};
#endif // HFX_H
