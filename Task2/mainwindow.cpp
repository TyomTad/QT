#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fileSystemModel = new QFileSystemModel(this);
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    qDebug() << "Desktop Path:" << desktopPath;
    fileSystemModel->setRootPath(desktopPath);

    treeView = new QTreeView(this);
    treeView->setModel(fileSystemModel);

    treeView->setRootIndex(fileSystemModel->index(desktopPath));

    setCentralWidget(treeView);
}

MainWindow::~MainWindow()
{
    delete ui;
}
