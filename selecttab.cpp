#include "selecttab.h"
QSqlDatabase SelectTab::db;
SelectTab::SelectTab()
{
    //db = QSqlDatabase::addDatabase("QMYSQL");
    qDebug()<< "started creating new tab";
    if (!SelectTab::db.isOpen())
        initDatabase();

    tablesSelectLabel.setText("Select table");
    this->selectLayout.addWidget(&tablesSelectLabel);

    tables = new QComboBox;
    tables->addItems(db.tables());
    this->selectLayout.addWidget(tables);

    headers = new QComboBox;

    bdModel = new QSqlTableModel(nullptr,db);

    setNewTable(0);

    setNewHeaders(0);

    searchByLabel.setText("Search by");
    this->selectLayout.addWidget(&searchByLabel);

    headers = new QComboBox;
    headers->addItems(columnsNames);
    this->selectLayout.addWidget(headers);

    searchValue = new QLineEdit;
    this->selectLayout.addWidget(searchValue);

    bdView = new QTableView;
    bdView->setModel(bdModel);
    bdView->verticalHeader()->hide();
    bdView->setColumnWidth(0,150);

    this->selectLayout.addWidget(bdView);
    this->setLayout(&selectLayout);

    connect(tables, &QComboBox::currentIndexChanged, this, &SelectTab::tableChanged);
    connect(tables, &QComboBox::currentIndexChanged, this, &SelectTab::emitTableChanged);
}
void SelectTab::tableChanged(int n)
{
    qDebug()<<"slot";
    setNewHeaders(n);
    setNewTable(n);
    this->update();
}

void SelectTab::emitTableChanged(int n)
{
    emit newTablePicked(n, this->tables->currentText());
}
void SelectTab::initDatabase()
{
    SelectTab::db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName("mydb");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("Kapibara228");
    if (!db.open())
    {
        qDebug()<<"db not opened";
        qDebug()<<db.lastError();
    }
}
void SelectTab::setNewHeaders(int n)
{
    QSqlRecord record = db.record(tables->itemText(n));
    int fieldsCount = record.count();
    columnsNames.clear();
    for (size_t i = 0; i < fieldsCount; ++i)
    {
        columnsNames.append(record.fieldName(i));
    }
    for (size_t i = 0;i < columnsNames.size();++i)
    {
        bdModel->setHeaderData(i,Qt::Horizontal,columnsNames[i]);
    }
    headers->clear();

    headers->addItems(columnsNames);
    // for (auto e: columnsNames)
    // {
    //     qDebug()<< e;
    //     qDebug()<<headers->
    // }
}
void SelectTab::setNewTable(int n)
{
    bdModel->setTable(tables->itemText(n));
    bdModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    bdModel->select();
}
