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

    bdView = new QTableView;
    bdView->setModel(model);
    bdView->verticalHeader()->hide();
    bdView->setColumnWidth(0,150);

    this->selectLayout.addWidget(bdView);
    this->setLayout(&selectLayout);

    connect(tables, &QComboBox::currentTextChanged, this, &SelectTab::tableChanged);
    connect(reports, &QComboBox::currentIndexChanged, this , &SelectTab::reportChanged);
    connect(tables, &QComboBox::currentIndexChanged, this, &SelectTab::emitTableChanged);
    connect(recallQuery, &QPushButton::clicked, this, &SelectTab::emitQueryRecalled);
    connect(this, &SelectTab::queryRecalled, this, &SelectTab::reportChanged);
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
    //if it's true query
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

void SelectTab::emitQueryRecalled(int n)
{
    emit this->queryRecalled(this->reports->currentIndex());
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
