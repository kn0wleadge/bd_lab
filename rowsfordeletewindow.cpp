#include "rowsfordeletewindow.h"

void RowsForDeleteWindow::emitCallDeleteRow()
{
    emit this->callDeleteRow();
}

RowsForDeleteWindow::RowsForDeleteWindow(QWidget* parent, QSqlQueryModel* model):QWidget(parent)
{
    qDebug()<< "creating 'RowsForDeleteWindow ";
    this->label = new QLabel();
    label->setText("Внимание, удаление этой записи приведет к удалению следующих записей");
    layout.addWidget(label,0,0);
    okButton = new QPushButton();
    okButton->setText("Ok");
    okButton->setFixedSize(120,30);
    cancelButton = new QPushButton();
    cancelButton->setText("Cancel");
    table = new QTableWidget();
    qDebug()<<"creating output table";
    for (size_t i = 0; i < model->rowCount(); ++i)
    {
        qDebug()<<i <<"row";

        QSqlRecord record = model->record(i);
        qDebug()<<"taked row";
        table->setColumnCount(record.count());

        table->insertRow(i);
        qDebug()<<"inserted new row";

        for (size_t j = 0; j < record.count() ; ++j)
        {
            qDebug()<<record.value(j).toString();
            QTableWidgetItem* newItem = new QTableWidgetItem(record.value(j).toString());
            table->setItem(i,j,newItem);
            qDebug()<<"item setted";
        }
    }
    table->verticalHeader()->hide();

    for (size_t i = 0; i < model->columnCount(); ++i )
    {
        table->setHorizontalHeaderItem(i,new QTableWidgetItem(model->headerData(i,Qt::Orientation::Horizontal).toString()));
    }
        table->resizeColumnsToContents();
    qDebug()<<"created output table";
    layout.addWidget(okButton,2,0);
    layout.addWidget(cancelButton,2,1);
    connect (okButton, &QPushButton::clicked, this, &RowsForDeleteWindow::emitCallDeleteRow);
    connect(okButton, &QPushButton::clicked, this , &QWidget::close);
    connect(cancelButton, &QPushButton::clicked, this , &QWidget::close);
    layout.addWidget(table,1,0);
    this->setLayout(&layout);
    qDebug()<<"finished creating 'RowsForDeleteWindow'";
}
