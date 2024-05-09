#include "addwindow.h"

void AddWindow::closeEvent(QCloseEvent *event)
{
    emitValuesEntered();
}

void AddWindow::emitValuesEntered()
{
    QStringList values;
    for (auto& e: enterFields)
    {
        values << e->text();
    }
    emit valuesEntered(values);
}

AddWindow::AddWindow(QStringList params)
{
    for (auto& e: params)
    {
        QLineEdit* newField = new QLineEdit(this);
        enterFields.append(newField);
        layout.addWidget(newField);
        newField->setPlaceholderText("Введите " + e);

    }
    okButton.setText("ok");
    layout.addWidget(&okButton);
    connect(&okButton, &QPushButton::clicked, this, &QWidget::close);
    this->setLayout(&layout);
    this->setGeometry(600,600,300,200);
}
