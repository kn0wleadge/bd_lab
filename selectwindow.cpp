#include "selectwindow.h"



// void SelectWindow::setNewHeaders(int n)
// {
//     QSqlRecord record = db.record(tables->itemText(n));
//     int fieldsCount = record.count();
//     QStringList columnsNames;
//     for (size_t i = 0; i < fieldsCount; ++i)
//     {
//         columnsNames.append(record.fieldName(i));
//     }
//     for (size_t i = 0;i < columnsNames.size();++i)
//     {
//         bdModel->setHeaderData(i,Qt::Horizontal,columnsNames[i]);
//     }
//     headers = new QComboBox;
//     headers->addItems(columnsNames);
// }
// void SelectWindow::initTab(int n)
// {
//     this->widget(n)->
// }
// void SelectWindow::setNewTable(int n)
// {
//     bdModel->setTable(tables->itemText(n));
//     bdModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
//     bdModel->select();
// }

// void SelectWindow::initDatabase()

// {
//     db = QSqlDatabase::addDatabase("QMYSQL");
//     db.setDatabaseName("mydb");
//     db.setHostName("localhost");
//     db.setUserName("root");
//     db.setPassword("Kapibara228");
//     if (!db.open())
//     {
//         qDebug()<<"db not opened";
//         qDebug()<<db.lastError();
//     }
// }
void SelectWindow::createNewTab(int n)
{
    qDebug()<<"Enter new tab create";
    qDebug()<<this->count();
    if (n == this->count() - 1)
    {
        SelectTab* newTab = new SelectTab;
        connect(newTab, &SelectTab::newTablePicked, this, &SelectWindow::tablesNameChanged);

        this->addTab(newTab," Bank ");
        disconnect(this, &SelectWindow::currentChanged, this,&SelectWindow::createNewTab);
        this->tabBar()->moveTab(this->count()-1, this->count() - 2);
        this->setCurrentIndex(this->count() - 2);
        connect(this, &SelectWindow::currentChanged, this,&SelectWindow::createNewTab);
        connect(this->tabBar(),&QTabBar::tabCloseRequested,this, &SelectWindow::closeTab);
        //connect(this->ta  bBar(),&QTabBar::tabCloseRequested,this, &QTabWidget::setCurrentIndex);
    }
}

void SelectWindow::closeTab(int n)
{
    qDebug()<<"Closing tab" << n;
    if (n !=0 && n != this->tabBar()->count() - 1)
    {
        this->setCurrentIndex(n -1);
        this->tabBar()->removeTab(n);
    }
}

void SelectWindow::tablesNameChanged(int n,QString newTableName)
{
    qDebug()<< "new name picked";
    qDebug() << n << newTableName;
    this->tabBar()->setTabText(this->tabBar()->currentIndex(),newTableName);
    this->update();
}

SelectWindow::SelectWindow(User* user,QWidget* parent):QTabWidget(parent)
{
    qDebug()<<"cons start";
    this->setGeometry(0,0,800,600);
    this->setWindowTitle("Select section");
    this->setTabShape(QTabWidget::Rounded);
    QIcon icon("V2w4_Uv0_C8.ico");
    this->setWindowIcon(icon);
    SelectTab* newTab = new SelectTab();
    connect(newTab, &SelectTab::newTablePicked, this, &SelectWindow::tablesNameChanged);
    this->addTab(newTab, " Bank ");
    this->addTab(new QWidget, " + ");
    this->setMovable(1);
    this->setTabsClosable(1);
    this->tabBar()->setTabButton(1,QTabBar::RightSide, nullptr);
    this->tabBar()->setTabButton(this->count()- 1,QTabBar::LeftSide,nullptr);

    this->user = user;
    connect(this, &SelectWindow::currentChanged, this,&SelectWindow::createNewTab);
    connect(this->tabBar(),&QTabBar::tabCloseRequested,this->tabBar(), &QTabBar::removeTab);
    //connect(this->tabBar(),&QTabBar::tabCloseRequested,this, &QTabWidget::setCurrentIndex);
    //connect(newTab, &SelectTab::tableChangedSignal,this, &SelectTab::tableChanged);

    qDebug()<<"cons end";
}
