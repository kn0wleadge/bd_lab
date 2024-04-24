#ifndef MAINMENU_H
#define MAINMENU_H
#include <QtWidgets>
#include "selectwindow.h"
class mainMenu:public QWidget
{
private slots:
    void openSelectWindow();
private:
    QGridLayout mainMenuLayout;
    QPushButton selectButton;
    QComboBox user;
    SelectWindow* sWindow;
public:
    mainMenu(QWidget* parent= nullptr);
};

#endif // MAINMENU_H
