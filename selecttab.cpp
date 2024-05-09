#include "selecttab.h"
QSqlDatabase SelectTab::db;
std::unordered_map<QString,QString> SelectTab::columnsNameDictionary;
SelectTab::SelectTab()
{
    //db = QSqlDatabase::addDatabase("QMYSQL");
    qDebug()<< "started creating new tab";
    if (!SelectTab::db.isOpen())
        initDatabase();
    if (SelectTab::columnsNameDictionary.empty())
        initColumnsNameDictionary();
    tablesSelectLabel.setText("Выберите таблицу");
    this->selectLayout.addWidget(&tablesSelectLabel,0,0);

    user = new User("fish catch registrator");
    //getting columns data types for users' available types
    for (size_t i = 0; i < user->getAvailTables().size(); ++i)
    {
        if (user->getAvailTables()[i] == "Банки")
        {
            QString tableName = "Банки";
            QStringList columnType;
            columnType << "int" << "str" << "str";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "Работники")
        {
            QString tableName = "Работники";
            QStringList columnType;
            columnType << "int" << "str" << "str" << "str";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "Рыба")
        {
            QString tableName = "Рыба";
            QStringList columnType;
            columnType << "str" << "int" << "int";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "Улов рыбы")
        {
            QString tableName = "Улов рыбы";
            QStringList columnType;
            columnType << "str" << "int" << "int" << "str";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "Учет рыболовных сессий")
        {
            QString tableName = "Учет рыболовный сессий";
            QStringList columnType;
            columnType << "int" << "int" << "str" << "str" << "str" << "int";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "Траулеры")
        {
            QString tableName = "Траулеры";
            QStringList columnType;
            columnType << "int" << "str" << "int" << "str";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "Рейсы")
        {
            QString tableName = "Рейсы";
            QStringList columnType;
            columnType << "int" << "int" << "str" << "str";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "Экипаж рейса")
        {
            QString tableName = "Экипаж рейса";
            QStringList columnType;
            columnType << "int" << "int";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
    }

    tables = new QComboBox;
    tables->addItems(user->getAvailTables());
    this->selectLayout.addWidget(tables,1,0);
    qDebug()<<"created user";
    paramsQuery = new QSqlQuery(db);
    model = new QSqlQueryModel;

    reports = new QComboBox;
    setNewReports("Банки");
    recallQuery = new QPushButton;
    recallQuery->setIcon(QIcon("recallQuery.ico"));
    recallQuery->setText("R");
    recallQuery->setFixedSize(25,25);

    qDebug()<<"added reports";
    QHBoxLayout* reportsAndButton = new QHBoxLayout;
    reportsAndButton->addWidget(reports);
    reportsAndButton->addWidget(recallQuery);
    this->selectLayout.addLayout(reportsAndButton,2,0);

    //button for opening add/edit/delete window
    QPushButton* addButton = new QPushButton("Добавить новую запись");
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
    qDebug()<<"query num" << n;

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

    //get report's data-input guides
    QStringList newReportInputGuides = reports[n]->getGuide();

    //if there are some params in query - call params window
    if (newReportInputGuides.size() > 0)
    {
        paramsWindow = new ParamsEnterWidget(newReportInputGuides);
        bool c = connect (paramsWindow, &ParamsEnterWidget::paramsEntered, this, &SelectTab::executeQuery);
        qDebug()<<"connecting new window to slot:" << c;
        paramsWindow->show();
        queryBuffer = reports[n]->getSql();
    }

    //if there are no params in query - execute it instantly
    else
    {
        this->model->setQuery(reports[n]->getSql());
        //getting first row in model to get quantity of columns
        // QSqlRecord tempRecord = this->model->record(0);
        // QStringList strList;
        // for (size_t i = 0; i < tempRecord.count(); ++i)
        // {
        //     strList<<this->model->headerData(i,Qt::Horizontal).toString();
        //     this->model->setHeaderData(i,Qt::Horizontal,
        //                                engColumnNameToRus(strList[i]));
        // }
        this->bdView->resizeColumnsToContents();
        qDebug()<<reports[n]->getSql();
       // qDebug() << "headers" << strList;
        qDebug()<<"0 params query executed";
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

    //looking throw all report groups and get current queries(reports)
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

    this->bdView->resizeColumnsToContents();

    //if query result is empty - display message box
    if( paramsQuery->size() == 0)
    {
        int n =QMessageBox::warning(0,
                                     "Warning",
                                     "Query result is empty",

                                     QMessageBox::Ok
                                     );
        paramsWindow->close();
        qDebug()<<paramsQuery->lastError();
        return;
        paramsWindow->close();
        qDebug()<<paramsQuery->lastError();
        return;
    }


    QVariantList list = paramsQuery->boundValues();
    for (auto &e: list)
    {
        qDebug() << e.toString();
    }
   // QThread::msleep(5000);
    this->model->setQuery(*paramsQuery);
    qDebug() << "parametrised query executed";


    // QSqlRecord tempRecord = this->model->record(0);
    // QStringList strList;
    // for (size_t i = 0; i < tempRecord.count(); ++i)
    // {
    //     strList<<this->model->headerData(i,Qt::Horizontal).toString();

    // }
    // qDebug() << "headers" << strList;

    this->bdView->resizeColumnsToContents();
    qDebug()<< this->model->lastError();
   // QThread::msleep(5000);
    update();
}

void SelectTab::openAddWindow(QString currentTableName)
{

    this->model->setQuery("select * from " + rusTableNameToEng(this->tables->currentText()));
    QSqlRecord tempRecord = this->model->record();
    QStringList columnNames;
    for (size_t i = 0; i < tempRecord.count(); ++i)
    {
       // columnNames << engColumnNameToRus(tempRecord.fieldName(i));
         columnNames << tempRecord.fieldName(i);
    }
    addWindow = new AddWindow(columnNames);
    connect(addWindow, &AddWindow::valuesEntered, this, &SelectTab::insertNewRow);
    addWindow->show();
}

void SelectTab::insertNewRow(QStringList values)
{
    qDebug() << "inserting new row";
    qDebug() << "quantity of values:" << values.size();
    //if some value is empty
    if (!values.contains(""))
    {
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
                                  rusTableNameToEng(this->tables->currentText())
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
            sqlErrorMsgBox->setInformativeText("Попытка добавить новую строку вызвала ошибку");
            sqlErrorMsgBox->setStandardButtons(QMessageBox::Ok);
            sqlErrorMsgBox->show();

        }
    }
    else
    {
        QMessageBox* valueEnterError = new QMessageBox(this);
        valueEnterError->setInformativeText("Какие-то данные не были введены");
        valueEnterError->setStandardButtons(QMessageBox::Ok);
        valueEnterError->setIcon(QMessageBox::Warning);
        valueEnterError->show();
    }
}

void SelectTab::initColumnsNameDictionary()
{
    /*
     * @function inits dictionary this pairs {mysqlColumnName,rus equivalent}
     */
    // bank table
    SelectTab::columnsNameDictionary.insert({"BNum","Номер банки"});
    SelectTab::columnsNameDictionary.insert({"BName","Название банки"});
    SelectTab::columnsNameDictionary.insert({"Cords","Координаты расположения"});

    // employee table
    SelectTab::columnsNameDictionary.insert({"EmNum","Номер сотрудника"});
    SelectTab::columnsNameDictionary.insert({"CName","Имя сотрудника"});
    SelectTab::columnsNameDictionary.insert({"CAdress","Адрес сотрудника"});
    SelectTab::columnsNameDictionary.insert({"CPosition","Должность сотрудника"});

    // fish table
    columnsNameDictionary.insert({"FishName","Название рыбы"});
    columnsNameDictionary.insert({"BNum","Номер банки"});
    columnsNameDictionary.insert({"Quantity","Количество рыбы"});

    //fishCatch table
    SelectTab::columnsNameDictionary.insert({"FishName","Номер рыбного улова"});
    SelectTab::columnsNameDictionary.insert({"resNum","Номер учетной сессии"});
    SelectTab::columnsNameDictionary.insert({"fishQuantity","Количество пойманной рыбы"});
    SelectTab::columnsNameDictionary.insert({"fishQuality","Качество пойманной рыбы"});

    //fishingSessionRes table
    SelectTab::columnsNameDictionary.insert({"resNum","Номер учетной сессии"});
    SelectTab::columnsNameDictionary.insert({"BNum","Номер банки"});
    SelectTab::columnsNameDictionary.insert({"DepDate","Дата прибытия на банку"});
    SelectTab::columnsNameDictionary.insert({"RetDate","Дата убытия с банки"});
    SelectTab::columnsNameDictionary.insert({"VNum","Номер рейса"});

    //trawler table
    SelectTab::columnsNameDictionary.insert({"TNum","Номер траулера"});
    SelectTab::columnsNameDictionary.insert({"TName","Название траулера"});
    SelectTab::columnsNameDictionary.insert({"Displacement","Водоизмещение"});
    SelectTab::columnsNameDictionary.insert({"Bdate","Дата ввода в строй"});

    //voyage table
    SelectTab::columnsNameDictionary.insert({"VNum","Номер рейса"});
    SelectTab::columnsNameDictionary.insert({"TNum","Номер траулера"});
    SelectTab::columnsNameDictionary.insert({"Vdate","Дата выхода в рейс"});
    SelectTab::columnsNameDictionary.insert({"Retdate","Дата прибытия с рейса"});

    //voyageCrew
    SelectTab::columnsNameDictionary.insert({"VNum","Номер рейса"});
    SelectTab::columnsNameDictionary.insert({"EmNum","Номер сотрудника"});
}

QString SelectTab::rusTableNameToEng(QString tName)
{
    /*
     * @this func converts russian table names to english(db type) names
     *
     * @return string
     */
    if (tName == "Банки")
        return "bank";
    else if (tName == "Рыба")
        return "fish";
    else if (tName == "Уловы рыбы")
        return "fishCatch";
    else if (tName == "Учет рыболовных сессий")
        return "fishingSessionRes";
    else if (tName == "Рейсы")
        return "voyage";
    else if (tName == "Работники")
        return "employee";
    else if (tName == "Траулеры")
        return "trawler";
    else if (tName == "Экипаж рейса")
        return "voyageCrew";
    return "error table";
}

QString SelectTab::engColumnNameToRus(QString cName)
{
    qDebug() << "translating" << cName;
    return this->columnsNameDictionary[cName];
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
    this->reports->setPlaceholderText("Выберите отчет");
    this->update();

}
void SelectTab::setNewTable(QString name)
{
    // bdModel->setTable(tables->itemText(n));
    // bdModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    // bdModel->select();
}
