#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    selectButton.setText("select");
    addButton.setText("add new raw");
    editButton.setText("edit some raw");

    mainMenuLayout.addWidget(&selectButton);
    mainMenuLayout.addWidget(&addButton);
    mainMenuLayout.addWidget(&editButton);

    this->setLayout(&mainMenuLayout);

}


MainWindow::~MainWindow()
{
    delete ui;
}
