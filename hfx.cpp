#include "hfx.h"
#include "ui_hfx.h"
#include "Python.h"
MyMessageBox::MyMessageBox(QWidget *parent) :
    QMessageBox(parent), width_(0), length_(0)
{
}

void MyMessageBox::resizeEvent(QResizeEvent *event)
{
    setFixedSize(width_, length_);
}

void MyMessageBox::setMySize(int width, int length)
{
    width_ = width;
    length_ = length;
}

bool sort_Sheet_Post(const Sheet &p1, const Sheet &p2)
{
    return p1.Post > p2.Post;
}

bool sort_Sheet_Deal(const Sheet &p1, const Sheet &p2)
{
    if (p1.Deal == p2.Deal)
    {
        return sort_Sheet_Post(p1, p2);
    }
    return p1.Deal < p2.Deal;
}

bool sort_Sheet_Discount(const Sheet &p1, const Sheet &p2)
{
    if (p1.Discount == p2.Discount)
    {
        return sort_Sheet_Deal(p1, p2);
    }
    return p1.Discount > p2.Discount;
}

bool sort_Sheet_Amount(const Sheet &p1, const Sheet &p2)
{
    if (p1.Amount == p2.Amount)
    {
        return sort_Sheet_Discount(p1, p2);
    }
    return p1.Amount > p2.Amount;
}

bool sort_Sheet_Price(const Sheet &p1, const Sheet &p2)
{
    if(p1.Price == p2.Price)
    {
        return sort_Sheet_Amount(p1, p2);
    }
    return p1.Price > p2.Price;
}

bool sort_Sheet_Type(const Sheet &p1, const Sheet &p2)
{
    if (p1.Type.compare(p2.Type) == 0)
    {
        return sort_Sheet_Price(p1, p2);
    }
    else if (p1.Type.compare(p2.Type) > 0)
    {
        return false;
    }
    return true;
}

bool sort_Sheet_Name(const Sheet &p1, const Sheet &p2)
{
    if (p1.Name.compare(p2.Name) == 0)
    {
        return sort_Sheet_Type(p1, p2);
    }
    else if (p1.Name.compare(p2.Name) > 0)
    {
        return false;
    }
    return true;
}

bool sort_Sheet_Date(const Sheet &p1, const Sheet &p2)
{
    if (p1.Date == p2.Date)
    {
        return sort_Sheet_Name(p1, p2);
    }
    return p1.Date < p2.Date;
}

HFX::HFX(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HFX), InLoadFlag_(false), InChanged_(true), InSaved_(false), InIsEdit_(false),
      OutLoaded_{false}, OutSaved_{false}, OutIsEdit_{false}
{
    ui->setupUi(this);
    ui->tableWidgetIncome->setEditTriggers(QAbstractItemView::NoEditTriggers);
    const QString NowType = ui->ComboTypeIncome->currentText();
    set_price_income(NowType);
    ui->PushButtonReadIncome->setStyleSheet("color:red");
    update_income();
    ui->tableWidgetIncome->resizeColumnsToContents();
    ui->tableWidgetIncome->resizeRowsToContents();

    /// 支出表格
    ui->PushButtonSubmitOutcome->setStyleSheet("color:red");
    ui->LineEditOutcomeNote->setReadOnly(true);
    ui->LineEditOutcomeNote->setStyleSheet("background-color:rgb(125,125,125)");

    ui->TableOutcome->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->TableOutcome->resizeColumnsToContents();
    ui->TableOutcome->resizeRowsToContents();
    ui->PushButtonReadOutcome->setStyleSheet("color:red");

    update_outcome();
    ui->PushButtonSubmitOutcome->setStyleSheet("color:green");
    OutChanged_ = false;
}

HFX::~HFX()
{
    delete ui;
}

void HFX::set_price_income(const QString &TypeName)
{
    // 自动根据商品种类更改商品单价
    if (TypeName == QString("泡脚方"))
    {
        ui->SpinPriceIncome->setValue(6);
    }
    else if (TypeName == QString("定制茶"))
    {
        ui->SpinPriceIncome->setValue(10);
    }
    else if (TypeName == QString("气血茶"))
    {
        ui->SpinPriceIncome->setValue(5);
    }
    else if (TypeName == QString("益气茶"))
    {
        ui->SpinPriceIncome->setValue(5);
    }
    else if (TypeName == QString("消火茶"))
    {
        ui->SpinPriceIncome->setValue(5);
    }
    else if (TypeName == QString("润肺茶"))
    {
        ui->SpinPriceIncome->setValue(9);
    }
    else if (TypeName == QString("减脂茶"))
    {
        ui->SpinPriceIncome->setValue(9);
    }
    else if (TypeName == QString("安神茶"))
    {
        ui->SpinPriceIncome->setValue(9);
    }
}

void HFX::on_ComboTypeIncome_currentIndexChanged(const QString &TypeName)
{
    set_price_income(TypeName);
    update_income();
}

void HFX::update_income()
{
    /// 读取表格信息
    InNewSheet_.Name = ui->LineNameIncome->text().toStdString();
    InNewSheet_.Date = ui->SpinDateIncome->text().toInt();
    InNewSheet_.Deal = ui->SpinDealIncome->text().toInt();
    InNewSheet_.Post = ui->SpinPostIncome->text().toFloat();
    InNewSheet_.Type = ui->ComboTypeIncome->currentText().toStdString();
    InNewSheet_.Amount = ui->SpinAmountIncome->text().toInt();
    InNewSheet_.Price = ui->SpinPriceIncome->text().toFloat();
    InNewSheet_.Discount = ui->SpinDiscountIncome->text().toFloat();

    InNewSheet_.Total = (InNewSheet_.Amount - InNewSheet_.Deal) * InNewSheet_.Price + InNewSheet_.Post - InNewSheet_.Discount;
    ui->LineTotalIncome->setText(QString("%1").arg(InNewSheet_.Total));

    /// 数据有变化后提交按钮的字体颜色变红
    /// 相应的变量变为false
    InChanged_ = true;
    ui->PushButtonSubmitIncome->setStyleSheet("color:red");
}

void HFX::on_SpinPriceIncome_valueChanged()
{
    update_income();
}

void HFX::on_SpinAmountIncome_valueChanged()
{
    update_income();
}

void HFX::on_SpinDealIncome_valueChanged()
{
    update_income();
}

void HFX::on_SpinPostIncome_valueChanged()
{
    update_income();
}

void HFX::on_SpinDateIncome_textChanged()
{
    InChanged_ = true;
    ui->PushButtonSubmitIncome->setStyleSheet("color:red");
}

void HFX::on_LineNameIncome_textChanged()
{
    InChanged_ = true;
    ui->PushButtonSubmitIncome->setStyleSheet("color:red");
}

void HFX::set_table_income(Sheet NowSheet)
{
    ui->tableWidgetIncome->insertRow(InAllSheet_.size());
    ui->tableWidgetIncome->setItem(InAllSheet_.size(), 0, new QTableWidgetItem(QString::number(NowSheet.Date)));
    ui->tableWidgetIncome->setItem(InAllSheet_.size(), 1, new QTableWidgetItem(NowSheet.Name.data()));
    ui->tableWidgetIncome->setItem(InAllSheet_.size(), 2, new QTableWidgetItem(NowSheet.Type.data()));
    ui->tableWidgetIncome->setItem(InAllSheet_.size(), 3, new QTableWidgetItem(QString::number(NowSheet.Price)));
    ui->tableWidgetIncome->setItem(InAllSheet_.size(), 4, new QTableWidgetItem(QString::number(NowSheet.Amount)));
    ui->tableWidgetIncome->setItem(InAllSheet_.size(), 5, new QTableWidgetItem(QString::number(NowSheet.Deal)));
    ui->tableWidgetIncome->setItem(InAllSheet_.size(), 6, new QTableWidgetItem(QString::number(NowSheet.Discount)));
    ui->tableWidgetIncome->setItem(InAllSheet_.size(), 7, new QTableWidgetItem(QString::number(NowSheet.Post)));
    ui->tableWidgetIncome->setItem(InAllSheet_.size(), 8, new QTableWidgetItem(QString::number(NowSheet.Total)));

    ui->tableWidgetIncome->resizeColumnsToContents();
    ui->tableWidgetIncome->resizeRowsToContents();
}

void HFX::on_PushButtonSubmitIncome_clicked()
{
    if (!InChanged_)
    {
        auto reply = QMessageBox::question(this, "亲爱的稍等！", "这个数据已经存过一次了！\n 要再存一次吗？", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
    }
    update_income();
    set_table_income(InNewSheet_);
    InAllSheet_.push_back(InNewSheet_);
    /// 相应的变量变为true
    InChanged_ = false;
    InSaved_ = false;
    ui->PushButtonSubmitIncome->setStyleSheet("color:green");
}

void HFX::on_PushButtonSaveIncome_clicked()
{
    if (!InLoadFlag_)
    {
        auto reply = QMessageBox::question(this, "亲爱的稍等！", "旧文件还没有读取呢！\n 要读取之前的数据吗？", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            on_PushButtonReadIncome_clicked();
            return;
        }
    }

    // 检查现有数据向量
    if (InAllSheet_.size() == 0)
    {
        MyMessageBox msg;
        msg.setWindowTitle("懒虫！！！");
        msg.setText("亲爱的,你没有输入数据呢还！ \n快填！！");
        msg.setMySize(400, 180);
        msg.addButton("好哒",QMessageBox::ActionRole);
        msg.exec();
        return;
    }

    QString FileName;
    QWidget *qwidget = new QWidget();
    FileName = QFileDialog::getSaveFileName(qwidget, "亲爱的想存在哪呀", "", "CSV文件(*.csv)");
    if (FileName == NULL)
    {
        return;
    }

    // 读取存储路径
    std::string SavePath = FileName.toStdString();

    // 讲数据按照日期先后排序
    auto AllSheetSave = InAllSheet_;
    sort(AllSheetSave.begin(), AllSheetSave.end(), sort_Sheet_Date);
    // 初始化Python
    Py_SetPythonHome((const wchar_t *)(L"C:/Python38"));
    Py_Initialize();

    if (!Py_IsInitialized())
    {
        ui->PushButtonSaveIncome->setStyleSheet("color:blue");
        return;
    }
    PyObject * PModule = NULL;
    PyObject * PFunc = NULL;
    PModule =PyImport_ImportModule("handle_data");
    PFunc= PyObject_GetAttrString(PModule, "save");

    // 将数据转换成Python的输入
    PyObject *NameList = PyList_New(0);
    PyObject *TypeList = PyList_New(0);
    PyObject *DateList = PyList_New(0);
    PyObject *AmountList = PyList_New(0);
    PyObject *DealList = PyList_New(0);
    PyObject *DiscountList = PyList_New(0);
    PyObject *PriceList = PyList_New(0);
    PyObject *PostList = PyList_New(0);
    PyObject *TotalList = PyList_New(0);

    PyObject *PArgs = PyTuple_New(2);
    PyObject *PDict = PyDict_New();

    for (unsigned int i = 0; i < InAllSheet_.size(); i++)
    {
        PyList_Append(NameList, Py_BuildValue("s", InAllSheet_[i].Name.data()));
        PyList_Append(TypeList, Py_BuildValue("s", InAllSheet_[i].Type.data()));
        PyList_Append(DateList, Py_BuildValue("i", InAllSheet_[i].Date));
        PyList_Append(AmountList, Py_BuildValue("i", InAllSheet_[i].Amount));
        PyList_Append(DealList, Py_BuildValue("i", InAllSheet_[i].Deal));
        PyList_Append(DiscountList, Py_BuildValue("f", InAllSheet_[i].Discount));
        PyList_Append(PriceList, Py_BuildValue("f", InAllSheet_[i].Price));
        PyList_Append(PostList, Py_BuildValue("f", InAllSheet_[i].Post));
        PyList_Append(TotalList, Py_BuildValue("l", InAllSheet_[i].Total));
    }

    PyDict_SetItemString(PDict, "Name", NameList);
    PyDict_SetItemString(PDict, "Type", TypeList);
    PyDict_SetItemString(PDict, "Date", DateList);
    PyDict_SetItemString(PDict, "Amount", AmountList);
    PyDict_SetItemString(PDict, "Deal", DealList);
    PyDict_SetItemString(PDict, "Discount", DiscountList);
    PyDict_SetItemString(PDict, "Price", PriceList);
    PyDict_SetItemString(PDict, "Post", PostList);
    PyDict_SetItemString(PDict, "Total", TotalList);

    PyTuple_SetItem(PArgs, 0, Py_BuildValue("s", SavePath.data()));
    PyTuple_SetItem(PArgs, 1, PDict);

    PyEval_CallObject(PFunc, PArgs);

    Py_Finalize();

    MyMessageBox msg;
    msg.setWindowTitle("亲爱的辛苦了！");
    msg.setText("保存成功！");
    msg.setMySize(400, 180);
    msg.addButton("好哒",QMessageBox::ActionRole);
    msg.exec();

    InSaved_ = true;
}

void HFX::on_SpinDiscountIncome_valueChanged()
{
    update_income();
}

void HFX::on_PushButtonReadIncome_clicked()
{
    std::string FileName;
    QWidget *qwidget = new QWidget();
    QString QFileName = NULL;
    QFileName = QFileDialog::getOpenFileName(qwidget, "亲爱的把上次的文件找出来好不好呀", "", "CSV文件(*.csv)");
    if (QFileName == NULL)
    {
        return;
    }
    FileName = QFileName.toStdString();
    Py_SetPythonHome((const wchar_t *)(L"C:/Python38"));
    Py_Initialize();

    if (!Py_IsInitialized())
    {
        ui->PushButtonReadIncome->setStyleSheet("color:blue");
        return;
    }

    PyObject *PModule = NULL;
    PyObject *PFunc = NULL;
    PyObject *PReturn = NULL;

    PModule =PyImport_ImportModule("handle_data");
    PFunc= PyObject_GetAttrString(PModule, "read");

    PyObject *PArgs = PyTuple_New(1);
    PyTuple_SetItem(PArgs, 0, Py_BuildValue("s", FileName.data()));

    PReturn = PyEval_CallObject(PFunc, PArgs);

    PyObject *NameList = PyDict_GetItemString(PReturn, "Name");
    PyObject *TypeList = PyDict_GetItemString(PReturn, "Type");
    PyObject *DateList = PyDict_GetItemString(PReturn, "Date");
    PyObject *AmountList = PyDict_GetItemString(PReturn, "Amount");
    PyObject *DealList = PyDict_GetItemString(PReturn, "Deal");
    PyObject *DiscountList = PyDict_GetItemString(PReturn, "Discount");
    PyObject *PriceList = PyDict_GetItemString(PReturn, "Price");
    PyObject *PostList = PyDict_GetItemString(PReturn, "Post");
    PyObject *TotalList = PyDict_GetItemString(PReturn, "Total");

    for (unsigned int i = 0; i < PyList_Size(NameList); i++)
    {
        PyArg_Parse(PyList_GetItem(NameList, i), "s", &InNewSheet_.Name);
        PyArg_Parse(PyList_GetItem(TypeList, i), "s", &InNewSheet_.Type);
        PyArg_Parse(PyList_GetItem(DateList, i), "i", &InNewSheet_.Date);
        PyArg_Parse(PyList_GetItem(AmountList, i), "i", &InNewSheet_.Amount);
        PyArg_Parse(PyList_GetItem(DealList, i), "i", &InNewSheet_.Deal);
        PyArg_Parse(PyList_GetItem(DiscountList, i), "f", &InNewSheet_.Discount);
        PyArg_Parse(PyList_GetItem(PriceList, i), "f", &InNewSheet_.Price);
        PyArg_Parse(PyList_GetItem(PostList, i), "f", &InNewSheet_.Post);
        PyArg_Parse(PyList_GetItem(TotalList, i), "l", &InNewSheet_.Total);

        set_table_income(InNewSheet_);
        InAllSheet_.push_back(InNewSheet_);
    }

    Py_Finalize();

    InLoadFlag_ = true;
    ui->PushButtonReadIncome->setStyleSheet("color:green");

    MyMessageBox msg;
    msg.setWindowTitle("亲爱的真棒！");
    msg.setText("读取成功！");
    msg.setMySize(400, 180);
    msg.addButton("好哒",QMessageBox::ActionRole);
    msg.exec();
}

void HFX::on_pushButtonDeletIncome_clicked()
{
    QList<QTableWidgetItem*> items = ui->tableWidgetIncome->selectedItems();
    // 获取选取的行号
    std::vector<int> SelectedRows;
    for (int i = 0; i < items.count(); i++)
    {
        int row = ui->tableWidgetIncome->row(items.at(i));
        if (i == 0)
        {
            SelectedRows.push_back(row);
            continue;
        }

        else if (row != *(SelectedRows.end()-1))
        {
            SelectedRows.push_back(row);
        }
    }
    // 删去被选中的行
    int Count = 0;
    for (const auto row : SelectedRows)
    {
        ui->tableWidgetIncome->removeRow(row - Count);
        InAllSheet_.erase(InAllSheet_.begin() + row - Count);
        Count ++;
    }

    InSaved_ = false;
}

void HFX::closeEvent(QCloseEvent *event)
{
    if (InSaved_)
    {
        event->accept();
        return;
    }

    auto reply = QMessageBox::question(this, "亲爱的稍等！", "更改的表格还没有存储呢！\n 要存储后再退出吗？", QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        on_PushButtonSaveIncome_clicked();
    }
    else if (reply == QMessageBox::Cancel)
    {
        event->ignore();
        return;
    }

    event->accept();
}

void HFX::on_tableWidgetIncome_itemDoubleClicked(QTableWidgetItem *item)
{
    InIsEdit_ = true;
    ui->tableWidgetIncome->editItem(item);
}

void HFX::on_tableWidgetIncome_itemChanged(QTableWidgetItem *item)
{
    if (!InIsEdit_)
    {
        return;
    }
    int row = ui->tableWidgetIncome->row(item);
    int column = ui->tableWidgetIncome->column(item);
    switch (column)
    {
    case 0:
        InAllSheet_[row].Date = item->data(Qt::DisplayRole).toInt();
        break;
    case 1:
        InAllSheet_[row].Name = item->data(Qt::DisplayRole).toString().toStdString();
        break;
    case 2:
        InAllSheet_[row].Type = item->data(Qt::DisplayRole).toString().toStdString();
        break;
    case 3:
        InAllSheet_[row].Price = item->data(Qt::DisplayRole).toFloat();
        break;
    case 4:
        InAllSheet_[row].Amount = item->data(Qt::DisplayRole).toInt();
        break;
    case 5:
        InAllSheet_[row].Deal = item->data(Qt::DisplayRole).toInt();
        break;
    case 6:
        InAllSheet_[row].Discount = item->data(Qt::DisplayRole).toFloat();
        break;
    case 7:
        InAllSheet_[row].Post = item->data(Qt::DisplayRole).toFloat();
        break;
    case 8:
        InAllSheet_[row].Total = item->data(Qt::DisplayRole).toFloat();
    }

    InSaved_ = false;
    InIsEdit_ = false;
}

/// 支出表格

void HFX::update_outcome()
{
    /// 读取表格信息
    OutNewSheet_.Date = ui->SpinBoxDateOutcome->text().toInt();
    OutNewSheet_.Price = ui->SpinBoxPriceOutcome->text().toFloat();

    QString TypeOutcome = ui->ComboBoxTypeOutcome->currentText();
    if (TypeOutcome == QString("其他"))
    {
        OutNewSheet_.Type = ui->LineEditOutcomeNote->text().toStdString();
    }
    else
    {
        OutNewSheet_.Type = TypeOutcome.toStdString();
    }

    /// 数据有变化后提交按钮的字体颜色变红
    /// 改变相应的状态变量
    outcome_status_changed();
}

void HFX::outcome_status_changed()
{
    OutChanged_ = true;
    ui->PushButtonSubmitOutcome->setStyleSheet("color:red");
}

void HFX::on_SpinBoxDateOutcome_valueChanged()
{
    OutNewSheet_.Date = ui->SpinBoxDateOutcome->text().toInt();

    outcome_status_changed();
}

void HFX::on_ComboBoxTypeOutcome_currentTextChanged(const QString &arg1)
{
    if (arg1 == QString("其他"))
    {
        ui->LineEditOutcomeNote->setReadOnly(false);
        ui->LineEditOutcomeNote->setStyleSheet("background-color:rgb(255,255,255)");
        OutNewSheet_.Type = ui->LineEditOutcomeNote->text().toStdString();
    }
    else
    {
        ui->LineEditOutcomeNote->setReadOnly(true);
        ui->LineEditOutcomeNote->setStyleSheet("background-color:rgb(125,125,125)");
        OutNewSheet_.Type = arg1.toStdString();
    }

    outcome_status_changed();
}

void HFX::on_SpinBoxPriceOutcome_valueChanged(double arg1)
{
    OutNewSheet_.Price = arg1;

    outcome_status_changed();
}

void HFX::set_table_outcome(Sheet NowSheet)
{
    ui->TableOutcome->insertRow(OutAllSheet_.size());

    ui->TableOutcome->setItem(OutAllSheet_.size(), 0, new QTableWidgetItem(QString::number(NowSheet.Date)));
    ui->TableOutcome->setItem(OutAllSheet_.size(), 1, new QTableWidgetItem(NowSheet.Type.data()));
    ui->TableOutcome->setItem(OutAllSheet_.size(), 2, new QTableWidgetItem(QString::number(NowSheet.Price)));

    ui->TableOutcome->resizeColumnsToContents();
    ui->TableOutcome->resizeRowsToContents();
}

void HFX::on_PushButtonSubmitOutcome_clicked()
{
    if (!OutChanged_)
    {
        auto reply = QMessageBox::question(this, "亲爱的稍等！", "这个数据已经存过一次了！\n 要再存一次吗？", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
    }

    ui->TableOutcome->insertRow(OutAllSheet_.size());
    ui->TableOutcome->setItem(OutAllSheet_.size(), 0, new QTableWidgetItem(QString::number(OutNewSheet_.Date)));
    ui->TableOutcome->setItem(OutAllSheet_.size(), 1, new QTableWidgetItem(QString(OutNewSheet_.Type.data())));
    ui->TableOutcome->setItem(OutAllSheet_.size(), 2, new QTableWidgetItem(QString::number(OutNewSheet_.Price)));

    ui->TableOutcome->resizeColumnsToContents();
    ui->TableOutcome->resizeRowsToContents();

    OutAllSheet_.push_back(OutNewSheet_);
    /// 相应的变量变为true
    OutChanged_ = false;
    ui->PushButtonSubmitOutcome->setStyleSheet("color:green");
}

void HFX::on_LineEditOutcomeNote_textChanged(const QString &arg1)
{
    OutNewSheet_.Type = arg1.toStdString();

    outcome_status_changed();
}

void HFX::on_PushButtonReadOutcome_clicked()
{
    std::string FileName;
    QWidget *qwidget = new QWidget();
    QString QFileName = NULL;
    QFileName = QFileDialog::getOpenFileName(qwidget, "亲爱的把上次的文件找出来好不好呀", "", "CSV文件(*.csv)");
    if (QFileName == NULL)
    {
        return;
    }
    FileName = QFileName.toStdString();
    Py_SetPythonHome((const wchar_t *)(L"C:/Python38"));
    Py_Initialize();

    if (!Py_IsInitialized())
    {
        ui->PushButtonReadOutcome->setStyleSheet("color:blue");
        return;
    }

    PyObject *PModule = NULL;
    PyObject *PFunc = NULL;
    PyObject *PReturn = NULL;

    PModule =PyImport_ImportModule("handle_data");
    PFunc= PyObject_GetAttrString(PModule, "read_outcome");

    PyObject *PArgs = PyTuple_New(1);
    PyTuple_SetItem(PArgs, 0, Py_BuildValue("s", FileName.data()));

    PReturn = PyEval_CallObject(PFunc, PArgs);

    PyObject *TypeList = PyDict_GetItemString(PReturn, "Type");
    PyObject *DateList = PyDict_GetItemString(PReturn, "Date");
    PyObject *PriceList = PyDict_GetItemString(PReturn, "Price");

    for (unsigned int i = 0; i < PyList_Size(TypeList); i++)
    {
        PyArg_Parse(PyList_GetItem(TypeList, i), "s", &OutNewSheet_.Type);
        PyArg_Parse(PyList_GetItem(DateList, i), "i", &OutNewSheet_.Date);
        PyArg_Parse(PyList_GetItem(PriceList, i), "f", &OutNewSheet_.Price);

        set_table_outcome(OutNewSheet_);
        OutAllSheet_.push_back(OutNewSheet_);
    }

    Py_Finalize();

    OutLoaded_ = true;
    ui->PushButtonReadOutcome->setStyleSheet("color:green");

    if (OutChanged_)
    {
        update_outcome();
    }
    else
    {
        update_outcome();
        OutChanged_ = false;
        ui->PushButtonSubmitOutcome->setStyleSheet("color:green");
    }

    MyMessageBox msg;
    msg.setWindowTitle("亲爱的真棒！");
    msg.setText("读取成功！");
    msg.setMySize(400, 180);
    msg.addButton("好哒",QMessageBox::ActionRole);
    msg.exec();
}

void HFX::on_PushButtonSaveOutcome_clicked()
{
    if (!OutLoaded_)
    {
        auto reply = QMessageBox::question(this, "亲爱的稍等！", "旧文件还没有读取呢！\n 要读取之前的数据吗？", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            on_PushButtonReadOutcome_clicked();
            return;
        }
    }

    // 检查现有数据向量
    if (OutAllSheet_.size() == 0)
    {
        MyMessageBox msg;
        msg.setWindowTitle("懒虫！！！");
        msg.setText("亲爱的,你没有输入数据呢还！ \n快填！！");
        msg.setMySize(400, 180);
        msg.addButton("好哒",QMessageBox::ActionRole);
        msg.exec();
        return;
    }

    QString FileName;
    QWidget *qwidget = new QWidget();
    FileName = QFileDialog::getSaveFileName(qwidget, "亲爱的想存在哪呀", "", "CSV文件(*.csv)");
    if (FileName == NULL)
    {
        return;
    }

    // 读取存储路径
    std::string SavePath = FileName.toStdString();

    // 讲数据按照日期先后排序
    auto AllSheetSave = OutAllSheet_;
    sort(AllSheetSave.begin(), AllSheetSave.end(), sort_Sheet_Date);
    // 初始化Python
    Py_SetPythonHome((const wchar_t *)(L"C:/Python38"));
    Py_Initialize();

    if (!Py_IsInitialized())
    {
        ui->PushButtonSaveOutcome->setStyleSheet("color:blue");
        return;
    }
    PyObject * PModule = NULL;
    PyObject * PFunc = NULL;
    PModule =PyImport_ImportModule("handle_data");
    PFunc= PyObject_GetAttrString(PModule, "save_outcome");

    // 将数据转换成Python的输入
    PyObject *TypeList = PyList_New(0);
    PyObject *DateList = PyList_New(0);
    PyObject *PriceList = PyList_New(0);

    PyObject *PArgs = PyTuple_New(2);
    PyObject *PDict = PyDict_New();

    for (unsigned int i = 0; i < OutAllSheet_.size(); i++)
    {
        PyList_Append(TypeList, Py_BuildValue("s", OutAllSheet_[i].Type.data()));
        PyList_Append(DateList, Py_BuildValue("i", OutAllSheet_[i].Date));
        PyList_Append(PriceList, Py_BuildValue("f", OutAllSheet_[i].Price));
    }

    PyDict_SetItemString(PDict, "Type", TypeList);
    PyDict_SetItemString(PDict, "Date", DateList);
    PyDict_SetItemString(PDict, "Price", PriceList);

    PyTuple_SetItem(PArgs, 0, Py_BuildValue("s", SavePath.data()));
    PyTuple_SetItem(PArgs, 1, PDict);

    PyEval_CallObject(PFunc, PArgs);

    Py_Finalize();

    MyMessageBox msg;
    msg.setWindowTitle("亲爱的辛苦了！");
    msg.setText("保存成功！");
    msg.setMySize(400, 180);
    msg.addButton("好哒",QMessageBox::ActionRole);
    msg.exec();

    OutSaved_ = true;
}

void HFX::on_PushButtonDeletOutcome_clicked()
{
    QList<QTableWidgetItem*> items = ui->TableOutcome->selectedItems();
    // 获取选取的行号
    std::vector<int> SelectedRows;
    for (int i = 0; i < items.count(); i++)
    {
        int row = ui->TableOutcome->row(items.at(i));
        if (i == 0)
        {
            SelectedRows.push_back(row);
            continue;
        }

        else if (row != *(SelectedRows.end()-1))
        {
            SelectedRows.push_back(row);
        }
    }
    // 删去被选中的行
    int Count = 0;
    for (const auto row : SelectedRows)
    {
        ui->TableOutcome->removeRow(row - Count);
        OutAllSheet_.erase(OutAllSheet_.begin() + row - Count);
        Count ++;
    }

    OutSaved_ = false;
}

void HFX::on_TableOutcome_itemDoubleClicked(QTableWidgetItem *item)
{
    OutIsEdit_ = true;
    ui->TableOutcome->editItem(item);
}

void HFX::on_TableOutcome_itemChanged(QTableWidgetItem *item)
{
    if (!OutIsEdit_)
    {
        return;
    }
    int row = ui->TableOutcome->row(item);
    int column = ui->TableOutcome->column(item);
    switch (column)
    {
    case 0:
        OutAllSheet_[row].Date = item->data(Qt::DisplayRole).toInt();
        break;
    case 1:
        OutAllSheet_[row].Type = item->data(Qt::DisplayRole).toString().toStdString();
        break;
    case 2:
        OutAllSheet_[row].Price = item->data(Qt::DisplayRole).toFloat();
    }

    OutSaved_ = false;
    OutIsEdit_ = false;
}
