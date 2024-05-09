#include "mainmenu.h"

mainMenu::mainMenu(QWidget* parent):QWidget(parent)
{
    this->setGeometry(600,600,400,300);
    this->setWindowTitle("Название");

    selectButton.setText("Ок");
    selectButton.setFixedSize(60,30);


    userList.addItem("Регистратор улова рыбы");

    mainMenuLayout.addWidget(&userList);
    mainMenuLayout.addWidget(&selectButton);

    bool con1 = connect(&selectButton, &QPushButton::clicked, this, &mainMenu::openSelectWindow);
    this->setLayout(&mainMenuLayout);
}
void mainMenu::openSelectWindow()
{
    User* user = new User(this->userList.currentText());
    qDebug()<<"open select window";
    sWindow = new SelectWindow(user,nullptr);
    sWindow->show();
}
