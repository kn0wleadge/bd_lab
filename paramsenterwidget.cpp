#include "paramsenterwidget.h"

void ParamsEnterWidget::emitParamsEntered()
{
    QStringList enteredParams;
    for (auto& e: enterFields)
    {
        enteredParams.append(e->text());
    }
    emit paramsEntered(enteredParams);


}

void ParamsEnterWidget::closeEvent(QCloseEvent *event)
{
    emitParamsEntered();
}

ParamsEnterWidget::ParamsEnterWidget(QStringList guideText)
{
    for (size_t i = 0; i < guideText.size(); ++i)
    {
        QLineEdit* someParam = new QLineEdit(this);
        enterFields.append(someParam);
        layout.addWidget(someParam);
        someParam->setPlaceholderText(guideText[i]);

    }
    this->okButton = new QPushButton("ok");
    connect(okButton, &QPushButton::clicked, this, &QWidget::close);
    layout.addWidget(okButton);
    this->setLayout(&layout);



}
