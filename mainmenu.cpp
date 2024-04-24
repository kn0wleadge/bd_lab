#include "mainmenu.h"

mainMenu::mainMenu(QWidget* parent):QWidget(parent)
{
    this->setGeometry(0,0,400,300);
    this->setWindowTitle("Database pro");

    selectButton.setText("select");
    selectButton.setFixedWidth(180);

    user.addItem("fish catch registrator");

    mainMenuLayout.addWidget(&user);
    mainMenuLayout.addWidget(&selectButton);

    bool con1 = connect(&selectButton, &QPushButton::clicked, this, &mainMenu::openSelectWindow);
    this->setLayout(&mainMenuLayout);
}
void mainMenu::openSelectWindow()
{
    qDebug()<<"open select window";
    sWindow = new SelectWindow();
    sWindow->show();
}
