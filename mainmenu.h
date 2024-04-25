#ifndef MAINMENU_H
#define MAINMENU_H
#include <QtWidgets>
#include "selectwindow.h"
#include "user.h"
class mainMenu:public QWidget
{
private slots:
    void openSelectWindow();
private:
    QGridLayout mainMenuLayout;
    QPushButton selectButton;
    QComboBox userList;
    SelectWindow* sWindow;
public:
    mainMenu(QWidget* parent= nullptr);
};

#endif // MAINMENU_H
