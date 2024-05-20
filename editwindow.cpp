#include "editwindow.h"

void EditWindow::someValueChanged()
{
    isValueChanged = true;
    qDebug() << "some value changed";
}

void EditWindow::cancelChanges()
{
    this->close();
}

void EditWindow::submitChanges()
{
    QStringList finalValues;
    for (auto& e:valuesList)
    {
        finalValues << e->text();
    }
    emit changesSubmited(finalValues);
    this->close();
}

void EditWindow::deleteRow()
{
    emit rowDeleteCalled();
    this->close();
}

void EditWindow::callDeleteWindow()
{
    RowsForDeleteWindow* window = new RowsForDeleteWindow(nullptr, this->model);
    connect(window, &RowsForDeleteWindow::callDeleteRow, this, &EditWindow::emitCallDeleteRow);
    connect(window, &RowsForDeleteWindow::callDeleteRow, this, &QWidget::close);
    window->show();
}

void EditWindow::emitCallDeleteRow()
{
    emit this->callDeleteRow();
}
EditWindow::EditWindow(QStringList values, QStringList columnsNames,  QSqlQueryModel* bdModel)
{
    this->model = bdModel;
    //this->setGeometry(300,300,800,600);
    qDebug() << "creating fields";
    mainLayout = new QGridLayout();
    for (size_t i = 0; i < values.size(); ++i)
    {
        qDebug()<<"started adding field";
        QVBoxLayout* layout = new QVBoxLayout;
        QLabel* label = new QLabel(nullptr);
        label->setText(columnsNames[i]);
        QLineEdit* lineEdit = new QLineEdit(nullptr);
        valuesList.append(lineEdit);
        lineEdit->setText(values[i]);
        connect(lineEdit, &QLineEdit::textChanged,this, &EditWindow::someValueChanged);
        qDebug()<<"created widgets";

        layout->addWidget(label);
        layout->addWidget(lineEdit);
        mainLayout->addLayout(layout,0,i);
        qDebug()<< i << "field";
    }
    isValueChanged = false;
    QPushButton* deleteButton = new QPushButton;
    deleteButton->setText("Удалить запись");
    connect(deleteButton, &QPushButton::clicked, this, &EditWindow::callDeleteWindow);
    mainLayout->addWidget(deleteButton,1,0);
    QPushButton* submitButton = new QPushButton;
    submitButton->setText("Применить");
    submitButton->setFixedSize(120,30);
    connect( submitButton, &QPushButton::clicked, this, &EditWindow::submitChanges);
    QPushButton* cancelButton = new QPushButton;
    cancelButton->setText("Отменить");
    cancelButton->setFixedSize(120,30);

    mainLayout->addWidget(submitButton,1,values.size());
    mainLayout->addWidget(cancelButton,1,values.size()-1,Qt::AlignRight);

    this->setLayout(mainLayout);
}
