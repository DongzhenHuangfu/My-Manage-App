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
    , ui(new Ui::HFX), LoadFlag(false), Changed(true)
{
    ui->setupUi(this);
    const QString NowType = ui->ComboType->currentText();
    set_price(NowType);
    ui->PushButtonRead->setStyleSheet("color:red");
    update();
}

HFX::~HFX()
{
    delete ui;
}

void HFX::set_price(const QString &TypeName)
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

void HFX::on_ComboType_currentIndexChanged(const QString &TypeName)
{
    set_price(TypeName);
    update();
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
    NewSheet.Discount = ui->SpinDiscount->text().toFloat();

    NewSheet.Total = (NewSheet.Amount - NewSheet.Deal) * NewSheet.Price + NewSheet.Post - NewSheet.Discount;
    ui->LineTotal->setText(QString("%1").arg(NewSheet.Total));

    /// 数据有变化后提交按钮的字体颜色变红
    /// 相应的变量变为false
    Changed = true;
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

void HFX::on_SpinDate_textChanged()
{
    Changed = true;
    ui->PushButtonSubmit->setStyleSheet("color:red");
}

void HFX::on_LineName_textChanged()
{
    Changed = true;
    ui->PushButtonSubmit->setStyleSheet("color:red");
}

void HFX::on_PushButtonSubmit_clicked()
{
    if (!Changed)
    {
        auto reply = QMessageBox::question(this, "亲爱的稍等！", "这个数据已经存过一次了！\n 要再存一次吗？", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            return;
        }
    }
    update();
    AllSheet.push_back(NewSheet);
    /// 相应的变量变为true
    Changed = false;
    ui->PushButtonSubmit->setStyleSheet("color:green");
}

void HFX::on_PushButtonSave_clicked()
{
    if (!LoadFlag)
    {
        auto reply = QMessageBox::question(this, "亲爱的稍等！", "旧文件还没有读取呢！\n 要读取之前的数据吗？", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            on_PushButtonRead_clicked();
            return;
        }
    }

    // 读取存储路径
    std::string SavePath = ui->LineSavePath->text().toStdString();
    if (SavePath.size() < 4)
    {
        MyMessageBox msg;
        msg.setWindowTitle("亲爱的打扰一下哦！");
        msg.setText("亲爱的,文件的路径是不是没选呀 \n选一下呗");
        msg.setMySize(400, 180);
        msg.addButton("好哒",QMessageBox::ActionRole);
        msg.exec();
        return;
    }
    else if (SavePath.substr(SavePath.size()-4, SavePath.size()) != ".csv")
    {
        MyMessageBox msg;
        msg.setWindowTitle("亲爱的打扰一下哦！");
        msg.setText("亲爱的,文件名是不是错啦 \n检查一下呗");
        msg.setMySize(400, 180);
        msg.addButton("好哒",QMessageBox::ActionRole);
        msg.exec();
        return;
    }

    // 检查现有数据向量
    if (AllSheet.size() == 0)
    {
        MyMessageBox msg;
        msg.setWindowTitle("懒虫！！！");
        msg.setText("亲爱的,你没有输入数据呢还！ \n快填！！");
        msg.setMySize(400, 180);
        msg.addButton("好哒",QMessageBox::ActionRole);
        msg.exec();
        return;
    }

    // 讲数据按照日期先后排序
    sort(AllSheet.begin(), AllSheet.end(), sort_Sheet_Date);
    // 初始化Python
    Py_SetPythonHome((const wchar_t *)(L"C:/Python38"));
    Py_Initialize();

    if (!Py_IsInitialized())
    {
        ui->PushButtonSave->setStyleSheet("color:blue");
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

    for (unsigned int i = 0; i < AllSheet.size(); i++)
    {
        PyList_Append(NameList, Py_BuildValue("s", AllSheet[i].Name.data()));
        PyList_Append(TypeList, Py_BuildValue("s", AllSheet[i].Type.data()));
        PyList_Append(DateList, Py_BuildValue("i", AllSheet[i].Date));
        PyList_Append(AmountList, Py_BuildValue("i", AllSheet[i].Amount));
        PyList_Append(DealList, Py_BuildValue("i", AllSheet[i].Deal));
        PyList_Append(DiscountList, Py_BuildValue("f", AllSheet[i].Discount));
        PyList_Append(PriceList, Py_BuildValue("f", AllSheet[i].Price));
        PyList_Append(PostList, Py_BuildValue("f", AllSheet[i].Post));
        PyList_Append(TotalList, Py_BuildValue("l", AllSheet[i].Total));
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
}

void HFX::on_PushButtonSavePath_clicked()
{
    QString FileName;
    QWidget *qwidget = new QWidget();
    FileName = QFileDialog::getSaveFileName(qwidget, "亲爱的想存在哪呀", "", "CSV文件(*.csv)");
    if (FileName == NULL)
    {
        return;
    }
    ui->LineSavePath->setText(FileName);
}

void HFX::on_SpinDiscount_valueChanged()
{
    update();
}

void HFX::on_PushButtonRead_clicked()
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
        ui->PushButtonRead->setStyleSheet("color:blue");
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
        PyArg_Parse(PyList_GetItem(NameList, i), "s", &NewSheet.Name);
        PyArg_Parse(PyList_GetItem(TypeList, i), "s", &NewSheet.Type);
        PyArg_Parse(PyList_GetItem(DateList, i), "i", &NewSheet.Date);
        PyArg_Parse(PyList_GetItem(AmountList, i), "i", &NewSheet.Amount);
        PyArg_Parse(PyList_GetItem(DealList, i), "i", &NewSheet.Deal);
        PyArg_Parse(PyList_GetItem(DiscountList, i), "f", &NewSheet.Discount);
        PyArg_Parse(PyList_GetItem(PriceList, i), "f", &NewSheet.Price);
        PyArg_Parse(PyList_GetItem(PostList, i), "f", &NewSheet.Post);
        PyArg_Parse(PyList_GetItem(TotalList, i), "l", &NewSheet.Total);
        AllSheet.push_back(NewSheet);
    }

    Py_Finalize();

    LoadFlag = true;
    ui->PushButtonRead->setStyleSheet("color:green");

    MyMessageBox msg;
    msg.setWindowTitle("亲爱的真棒！");
    msg.setText("读取成功！");
    msg.setMySize(400, 180);
    msg.addButton("好哒",QMessageBox::ActionRole);
    msg.exec();
}
