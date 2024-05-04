#include "selecttab.h"
QSqlDatabase SelectTab::db;
SelectTab::SelectTab()
{
    //db = QSqlDatabase::addDatabase("QMYSQL");
    qDebug()<< "started creating new tab";
    if (!SelectTab::db.isOpen())
        initDatabase();
    tablesSelectLabel.setText("select table");
    this->selectLayout.addWidget(&tablesSelectLabel,0,0);

    user = new User("fish catch registrator");
    //getting columns data types for users' available types
    for (size_t i = 0; i < user->getAvailTables().size(); ++i)
    {
        if (user->getAvailTables()[i] == "bank")
        {
            QString tableName = "bank";
            QStringList columnType;
            columnType << "int" << "str" << "str";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "employee")
        {
            QString tableName = "employee";
            QStringList columnType;
            columnType << "int" << "str" << "str" << "str";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "fish")
        {
            QString tableName = "fish";
            QStringList columnType;
            columnType << "str" << "int" << "int";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "fishCatch")
        {
            QString tableName = "fishCatch";
            QStringList columnType;
            columnType << "str" << "int" << "int" << "str";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "fishingSessionRes")
        {
            QString tableName = "fishingSessionRes";
            QStringList columnType;
            columnType << "int" << "int" << "str" << "str" << "str" << "int";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "trawler")
        {
            QString tableName = "trawler";
            QStringList columnType;
            columnType << "int" << "str" << "int" << "str";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "voyage")
        {
            QString tableName = "voyage";
            QStringList columnType;
            columnType << "int" << "int" << "str" << "str";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "voyageCrew")
        {
            QString tableName = "voyageCrew";
            QStringList columnType;
            columnType << "int" << "int";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
    }
    paramsQuery = new QSqlQuery(db);
    tables = new QComboBox;
    tables->addItems(user->getAvailTables());
    this->selectLayout.addWidget(tables,1,0);
    qDebug()<<"created user";

    model = new QSqlQueryModel;

    reports = new QComboBox;
    setNewReports("bank");
    recallQuery = new QPushButton;
    recallQuery->setIcon(QIcon("recallQuery.ico"));
    recallQuery->setFixedSize(25,25);

    qDebug()<<"added reports";
    QHBoxLayout* reportsAndButton = new QHBoxLayout;
    reportsAndButton->addWidget(reports);
    reportsAndButton->addWidget(recallQuery);
    this->selectLayout.addLayout(reportsAndButton,2,0);

    //button for opening add/edit/delete window
    QPushButton* addButton = new QPushButton("add/delete/edit ");
    this->selectLayout.addWidget(addButton,3,0);


    bdView = new QTableView;
    bdView->setModel(model);
    bdView->verticalHeader()->hide();
    bdView->setColumnWidth(0,150);

    this->selectLayout.addWidget(bdView);
    this->setLayout(&selectLayout);

    qDebug()<<"started connections";
    connect(tables, &QComboBox::currentTextChanged, this, &SelectTab::tableChanged);
    connect(reports, &QComboBox::currentIndexChanged, this , &SelectTab::reportChanged);
    connect(tables, &QComboBox::currentIndexChanged, this, &SelectTab::emitTableChanged);
    connect(recallQuery, &QPushButton::clicked, this, &SelectTab::emitQueryRecalled);
    connect(this, &SelectTab::queryRecalled, this, &SelectTab::reportChanged);
    connect(addButton, &QPushButton::clicked, this, &SelectTab::emitOpenAddWindow);
    connect(this, &SelectTab::addWindowButtonClicked, this, &SelectTab::openAddWindow);

    qDebug()<<"tab created";
}
void SelectTab::tableChanged(QString name)
{
    qDebug()<<"slot";
    this->model->clear();
    setNewReports(name);
    this->update();
}

void SelectTab::reportChanged(int n)
{
    qDebug()<<"report changed";
    qDebug()<<n;

    //if it's real query
    if (n != -1)
    {

    QVector<Report*> reports;

    //looking throw all report groups and get current reports
    for (auto& e: this->user->getAvailReportGroups())
    {
        if (e->getTableName() == this->tables->currentText())
        {
            reports = e->getReports();
        }
    }

    //get reports data input guides
    QStringList newReportInputGuides = reports[n]->getGuide();

    //if there are some params in query - call params window
    if (newReportInputGuides.size() > 0)
    {
        paramsWindow = new ParamsEnterWidget(newReportInputGuides);
        bool c = connect (paramsWindow, &ParamsEnterWidget::paramsEntered, this, &SelectTab::executeQuery);
        qDebug()<< c;
        paramsWindow->show();
        queryBuffer = reports[n]->getSql();
    }

    //if there are no params in query - execute it instantly
    else
    {
        this->model->setQuery(reports[n]->getSql());
        qDebug()<<reports[n]->getSql();
    }

    update();
    }
}

void SelectTab::emitTableChanged(int n)
{
    emit newTablePicked(n, this->tables->currentText());
}

void SelectTab::executeQuery(QStringList paramsList)
{
    qDebug()<<"executing query";
    qDebug()<<queryBuffer;
    int bindValues = queryBuffer.count(":");
    qDebug()<<"Num of bind value - " << bindValues;
    QVector<Report*> reports;

    //looking throw all report groups and get current reports
    for (auto& e: this->user->getAvailReportGroups())
    {
        if (e->getTableName() == this->tables->currentText())
        {
            reports = e->getReports();
        }
    }

    QStringList params = reports[this->reports->currentIndex()]->getParamsNameList();
    qDebug()<< "preparing query";
    paramsQuery->prepare(queryBuffer);
    for (size_t i = 0; i < paramsList.size() ; ++i)
    {
        paramsQuery->bindValue(params[i], paramsList[i]);
    }

    paramsQuery->exec();

    //if query result is empty
    if( paramsQuery->size() == 0)
    {
        int n = QMessageBox::warning(0,
                                     "Warning",
                                     "Query result is empty",

                                     QMessageBox::Ok
                                     );
        paramsWindow->close();
        qDebug()<<paramsQuery->lastError();
        return;
    }


    QVariantList list = paramsQuery->boundValues();
    for (auto &e: list)
    {
        qDebug() << e.toString();
    }
    this->model->setQuery(*paramsQuery);
    qDebug()<< this->model->lastError();
}

void SelectTab::openAddWindow(QString currentTableName)
{
    this->model->setQuery("select * from " + this->tables->currentText());
    QSqlRecord tempRecord = this->model->record();
    QStringList columnNames;
    for (size_t i = 0; i < tempRecord.count(); ++i)
    {
        columnNames << tempRecord.fieldName(i);
    }
    addWindow = new AddWindow(columnNames);
    connect(addWindow, &AddWindow::valuesEntered, this, &SelectTab::insertNewRow);
    addWindow->show();
}

void SelectTab::insertNewRow(QStringList values)
{
    qDebug() << "inserting new row";
    QStringList columnsTypes;
    for (auto& e: columnsDataTypes)
    {
        if (e.first == tables->currentText())
        {
            columnsTypes = e.second;
            break;
        }
    }
    QString insertQueryText = "insert into " +
                              this->tables->currentText()
                               + " values (";
    //iterating throw all values
    for (size_t i = 0; i < values.size();++i)
    {
        if (columnsTypes[i] == "str")
        {
            insertQueryText.push_back("' " + values[i] + " '" + ",");
        }
        else if(columnsTypes[i] == "int")
        {
            insertQueryText.push_back(values[i] + + " , ");
        }
    }
    insertQueryText.removeLast();
    insertQueryText.push_back(");");
    qDebug()<<insertQueryText;
    this->model->setQuery(insertQueryText);
    qDebug()<<model->lastError().text();
    if(!model->lastError().text().isEmpty())
    {
        QMessageBox* sqlErrorMsgBox = new QMessageBox(this);
        sqlErrorMsgBox->setText(model->lastError().text());
        sqlErrorMsgBox->setInformativeText("Some error due to your row insert");
        sqlErrorMsgBox->setStandardButtons(QMessageBox::Ok);
        sqlErrorMsgBox->show();

    }
}

void SelectTab::emitQueryRecalled(int n)
{
    emit this->queryRecalled(this->reports->currentIndex());
}

void SelectTab::emitOpenAddWindow()
{
    emit this->addWindowButtonClicked(this->tables->currentText());
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
void SelectTab::setNewReports(QString name)
{


    QVector<Report*> newReports;
    //looking throw all report groups
    for (auto& e: this->user->getAvailReportGroups())
    {
        if (e->getTableName() == name)
        {
            newReports = e->getReports();
        }
    }

    reports->clear();
    for (auto& e: newReports)
    {
        reports->addItem(e->getText());
    }
    this->reports->setCurrentIndex(-1);
    this->reports->setPlaceholderText("Select report");
    this->update();

}
void SelectTab::setNewTable(QString name)
{
    // bdModel->setTable(tables->itemText(n));
    // bdModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    // bdModel->select();
}
