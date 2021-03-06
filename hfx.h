#ifndef HFX_H
#define HFX_H

// #pragma execution_character_set("utf-8")

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTableWidgetItem>
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
    // 收入
    void update_income();
    void set_price_income(const QString &TypeName);
    void set_table_income(Sheet NowSheet);
    // 支出
    void update_outcome();
    void set_table_outcome(Sheet NowSheet);

private slots:
    void closeEvent(QCloseEvent *event);

    /// 收入表格
    void on_ComboTypeIncome_currentIndexChanged(const QString &TypeName);

    void on_SpinPriceIncome_valueChanged();

    void on_SpinAmountIncome_valueChanged();

    void on_SpinDealIncome_valueChanged();

    void on_SpinPostIncome_valueChanged();

    void on_PushButtonSubmitIncome_clicked();

    void on_PushButtonSaveIncome_clicked();

    void on_SpinDateIncome_textChanged();

    void on_LineNameIncome_textChanged();

    void on_SpinDiscountIncome_valueChanged();

    void on_PushButtonReadIncome_clicked();

    void on_pushButtonDeletIncome_clicked();

    void on_tableWidgetIncome_itemDoubleClicked(QTableWidgetItem *item);

    void on_tableWidgetIncome_itemChanged(QTableWidgetItem *item);

    bool unsaved_in();

    /// 支出表格
    void on_SpinBoxDateOutcome_valueChanged();

    void on_ComboBoxTypeOutcome_currentTextChanged(const QString &arg1);

    void on_SpinBoxPriceOutcome_valueChanged(double arg1);

    void outcome_status_changed();

    void on_PushButtonSubmitOutcome_clicked();

    void on_LineEditOutcomeNote_textChanged(const QString &arg1);

    void on_PushButtonReadOutcome_clicked();

    void on_PushButtonSaveOutcome_clicked();

    void on_PushButtonDeletOutcome_clicked();

    void on_TableOutcome_itemDoubleClicked(QTableWidgetItem *item);

    void on_TableOutcome_itemChanged(QTableWidgetItem *item);

    bool unsaved_out();

    /// 总表头
    void on_PushButtonUpdate_clicked();

private:
    Ui::HFX *ui;
    /// 收入面板变量
    Sheet InNewSheet_;
    std::vector<Sheet> InAllSheet_;
    bool InLoadFlag_, InChanged_, InSaved_, InIsEdit_;

    /// 支出面板变量
    // 实时表单数据
    Sheet OutNewSheet_;
    // 所有表单数据
    std::vector<Sheet> OutAllSheet_;
    // 状态标志
    bool OutLoaded_, OutChanged_, OutSaved_, OutIsEdit_;

    /// 总表头变量
    // 总支出和总收入
    double TotalOutcome_, TotalIncome_;
};
#endif // HFX_H
