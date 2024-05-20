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
    tablesSelectLabel.setText("Текущая таблица");
    tablesSelectLabel.setFixedSize(140,20);

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
            QString tableName = "Учет рыболовных сессий";
            QStringList columnType;
            columnType << "int" << "int" << "str" << "str" << "int";;
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "Траулер")
        {
            QString tableName = "Траулер";
            QStringList columnType;
            columnType << "int" << "str" << "int" << "str";
            columnsDataTypes.append(std::pair(tableName, columnType));
        }
        else if(user->getAvailTables()[i] == "Рейс")
        {
            QString tableName = "Рейс";
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

    //this->selectLayout.addLayout(tableLabelAndCurrentTable,0,0);
    QHBoxLayout* newLayout = new QHBoxLayout;
    newLayout->addWidget(&tablesSelectLabel);
    newLayout->setSpacing(0);
    newLayout->addWidget(tables);

    newLayout->setAlignment(Qt::AlignLeft);
    // this->selectLayout.addWidget(&tablesSelectLabel,0,0);
    // this->selectLayout.addWidget(tables,0,0);
    this->selectLayout.addLayout(newLayout,0,0);
    qDebug()<<"created user";
    paramsQuery = new QSqlQuery(db);
    model = new QSqlQueryModel;

    reports = new QComboBox;
    setNewReports("Банка");
    recallQuery = new QPushButton;
    recallQuery->setIcon(QIcon("recallQuery.ico"));
    recallQuery->setText("R");
    recallQuery->setFixedSize(25,25);

    qDebug()<<"added reports";
    QHBoxLayout* reportsAndButton = new QHBoxLayout;
    QLabel* currentQuery = new QLabel("Текущий запрос");
    reportsAndButton->addWidget(currentQuery);
    reportsAndButton->addWidget(reports);
    reportsAndButton->addWidget(recallQuery);
    this->selectLayout.addLayout(reportsAndButton,1,0);

    //button for opening add/edit/delete window
    QPushButton* addButton = new QPushButton;
    addButton->setText("Добавить новую запись");
    this->selectLayout.addWidget(addButton,2,0);
    bdView = new QTableView;
    bdView->setModel(model);
    bdView->verticalHeader()->hide();
    bdView->setColumnWidth(0,150);

    this->selectLayout.addWidget(bdView,4,0);
    this->setLayout(&selectLayout);

    qDebug()<<"started connections";
    connect(tables, &QComboBox::currentTextChanged, this, &SelectTab::tableChanged);
    connect(reports, &QComboBox::currentIndexChanged, this , &SelectTab::reportChanged);
    connect(tables, &QComboBox::currentIndexChanged, this, &SelectTab::emitTableChanged);
    connect(recallQuery, &QPushButton::clicked, this, &SelectTab::emitQueryRecalled);
    connect(this, &SelectTab::queryRecalled, this, &SelectTab::reportChanged);
    connect(addButton, &QPushButton::clicked, this, &SelectTab::emitOpenAddWindow);
    connect(this, &SelectTab::addWindowButtonClicked, this, &SelectTab::openAddWindow);

    connect(this, &SelectTab::editWindowOpen, this, &SelectTab::openEditWindow);

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
        if (n == 0  && this->tables->currentText() != "Рейс")
        {
                connect(this->bdView, &QTableView::doubleClicked, this, &SelectTab::tableDoubleClicked);
        }
        else
        {
            disconnect(this->bdView, &QTableView::doubleClicked, this, &SelectTab::tableDoubleClicked);
        }
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
        qDebug() << reports[n]->getSql();
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
        qDebug() << this->model->lastError();
    }


    }
    else
    {
        disconnect(this->bdView, &QTableView::doubleClicked, this, &SelectTab::tableDoubleClicked);

    }
    update();
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
        columnNames << engColumnNameToRus(tempRecord.fieldName(i));
    }
    addWindow = new AddWindow(columnNames);
    connect(addWindow, &AddWindow::valuesEntered, this, &SelectTab::insertNewRow);
    addWindow->show();
}

void SelectTab::editRow(QStringList newValues)
{
    if (this->tables->currentText() == "Банка")
    {
        this->model->setQuery("update bank set BName = '" + newValues[1] + "', Cords = '" + newValues[2] + "' where BNum = " + newValues[0] + ";");
    }
    else if (this->tables->currentText() == "Рыба")
    {
        this->model->setQuery("update fish set Quantity = " + newValues[2] +  "where BNum = " + newValues[1] + ", FishName = '" + newValues[0] + "';");

    }
    else if (this->tables->currentText() == "Улов рыбы")
    {
        this->model->setQuery("update fishCatch set fishQuantity = " + newValues[2] + ",fishQuality = '" + newValues[3] + "', where FishName = '" + newValues[0] + "', resNum = '" + newValues[1] + "';");

    }
    else if (this->tables->currentText() == "Учет рыбаловных сессий")
    {
        this->model->setQuery("update fishingSessionRes set BNum = " + newValues[1] + ", DepDate = '" + newValues[2] + "', RetDate = " + newValues[3]+ ", VNum = '" + newValues[4] + "' where resNum = " + newValues[0]);

    }
    else if (this->tables->currentText() == "Рейс")
    {
        this->model->setQuery("update voyage set Tnum = " + newValues[1] + ", Vdate = '" + newValues[2]+ "', Retdate = '" + newValues[3] + "' where VNum = " + newValues[0]);

    }

    qDebug()<< this->model->lastError();
}

void SelectTab::deleteRow()
{
    QSqlRecord record = deletingRow;
    if (this->tables->currentText() == "Банка")
    {
        qDebug()<< "BNum from deleted row - "<< record.value(0).toString();
        this->model->setQuery("delete from fishCatch where FishName in (select FishName from fish where BNum = " + record.value(0).toString() + ");");
        qDebug()<< this->model->lastError();
        this->model->setQuery("delete from fishingSessionRes where BNum = " + record.value(0).toString() + ";");
        qDebug()<< this->model->lastError();
        this->model->setQuery("delete from fish where BNum = " + record.value(0).toString() + ";");
        qDebug()<< this->model->lastError();
        this->model->setQuery("delete from bank where BNum = " + record.value(0).toString() + ";");
        qDebug()<< this->model->lastError();
    }
    else if (this->tables->currentText() == "Рыба")
    {
        qDebug() << record.value(0).toString();
        this->model->setQuery("delete from fishCatch where fishCatch.FishName ='" + record.value(0).toString() + "'and fishCatch.resNum in (select fishingSessionRes.resNum from fishingSessionRes where fishingSessionRes.BNum = (select BNum from bank where BName = '" + record.value(2).toString()+  "'));");
        qDebug() << this->model->lastError();
        this->model->setQuery("delete from fish where BNum = (select BNum from bank where BName = '" + record.value(2).toString()+  "')" + " and FishName = '" + record.value(0).toString() + "';");
        qDebug() << this->model->lastError();
    }
    else if (this->tables->currentText() == "Улов рыбы")
    {
        this->model->setQuery("delete from fishCatch where FishName = '" + record.value(0).toString() + "'and resNum = '" +  record.value(1).toString() + "';");
    }
    else if (this->tables->currentText() == "Учет рыболовных сессий")
    {
        this->model->setQuery("delete from fishCatch where resNum = " + record.value(0).toString() + ";");
        this->model->setQuery("delete from fishingSessioNres where resNum = " + record.value(0).toString());
    }
    else if (this->tables->currentText() == "Рейс")
    {
        int n =QMessageBox::critical(0,
                                     "Вы не можете удалить эту запись",
                                     "Обратитесь к администратору базы данных",

                                     QMessageBox::Ok
                                     );
    }
    qDebug()<< "raw deleted"<<this->model->lastError();
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
        qDebug() << "quantity of columns" << columnsTypes.size();
        qDebug()<< "creating query";
        QString insertQueryText = "insert into " +
                                  rusTableNameToEng(this->tables->currentText())
                                  + " values (";
        qDebug() << "query created";
        //iterating throw all values
        for (size_t i = 0; i < values.size(); ++i)
        {
            if (columnsTypes[i] == "str")
            {
                insertQueryText.push_back("'" + values[i] + "'" + " ,");
            }
            else if(columnsTypes[i] == "int")
            {
                insertQueryText.push_back(values[i] + + " ,");
            }
        }
        qDebug() << "get values types";
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
    SelectTab::columnsNameDictionary.insert({"Tnum","Номер траулера"});
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
    if (tName == "Банка")
        return "bank";
    else if (tName == "Рыба")
        return "fish";
    else if (tName == "Улов рыбы")
        return "fishCatch";
    else if (tName == "Учет рыболовных сессий")
        return "fishingSessionRes";
    else if (tName == "Рейс")
        return "voyage";
    else if (tName == "Работник")
        return "employee";
    else if (tName == "Траулер")
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

void SelectTab::tableDoubleClicked(const QModelIndex& index)
{
    QStringList currentParams;
    int n = index.row();
    currentSelectedRow = n;
    qDebug() << n << "row clicked";
    QSqlRecord record = this->model->record(n);
    deletingRow = this->model->record(n);
    qDebug()<<"getting values from row";
    for (size_t i = 0; i < record.count(); ++i)
    {
        currentParams << record.value(i).toString();
    }
    qDebug() << "getting column names";
    QStringList columnNames;
    for (size_t i = 0; i < record.count(); ++i)
    {
        columnNames << record.fieldName(i);
    }
    qDebug()<<"emiting 'open window'";
    emit editWindowOpen(currentParams, columnNames);
}

void SelectTab::openEditWindow(QStringList values, QStringList columnsNames)
{
    if (this->tables->currentText() == "Банка")
    {
        this->model->setQuery("select * from fish where BNum = " + values[0] + ";");
        qDebug()<< this->model->lastError();
    }
    else if (this->tables->currentText() == "Рыба")
    {
        qDebug() << values[0] << values [1];
        this->model->setQuery("select * from fishCatch where fishCatch.FishName = '" + values[0] + "' AND fishCatch.resNum in (select fishingSessionRes.resNum from fishingSessionRes where BNum = (select BNum from bank where BName = '" + values[2] + "'));");
        qDebug()<< this->model->lastError();

    }
    else if (this->tables->currentText() == "Улов рыбы")
    {
    }
    else if (this->tables->currentText() == "Учет рыболовных сессий")
    {
        this->model->setQuery("select * from fishCatch where resNum = " + values[0] + ";");

    }
    else if (this->tables->currentText() == "Рейс")
    {
        int n =QMessageBox::critical(0,
                                      "Вы не можете изменять эту запись",
                                      "Обратитесь к администратору базы данных",

                                      QMessageBox::Ok
                                      );
    }
    for (size_t i = 0; i < this->model->columnCount(); ++i)
    {

        this->model->setHeaderData(i,Qt::Orientation::Horizontal,
                                   engColumnNameToRus(this->model->headerData(i,Qt::Orientation::Horizontal).toString()));
    }
    EditWindow* newEditWindow = new EditWindow(values, columnsNames, this->model);
    connect(newEditWindow, &EditWindow::changesSubmited, this, &SelectTab::editRow);
    connect(newEditWindow, &EditWindow::callDeleteRow, this,&SelectTab::deleteRow);

    newEditWindow->show();
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
