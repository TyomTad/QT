#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setupEditor();
    setupMenu();
    setupToolbar();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupEditor() {
    textEdit = new QTextEdit(this);
    setCentralWidget(textEdit);
}

void MainWindow::setupMenu() {
    QMenu *fileMenu = menuBar()->addMenu(tr("File"));

    QAction *newAction = new QAction(tr("New"), this);
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAction);

    QAction *openAction = new QAction(tr("Open"), this);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openAction);

    QAction *saveAction = new QAction(tr("Save"), this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    fileMenu->addAction(saveAction);

    QAction *saveAsAction = new QAction(tr("Save As"), this);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);
    fileMenu->addAction(saveAsAction);

    QAction *exitAction = new QAction(tr("Exit"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
    fileMenu->addAction(exitAction);
}

void MainWindow::setupToolbar() {
    QToolBar *toolBar = addToolBar(tr("Toolbar"));

    QAction *newAction = new QAction(tr("New"), this);
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    toolBar->addAction(newAction);

    QAction *openAction = new QAction(tr("Open"), this);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    toolBar->addAction(openAction);

    QAction *saveAction = new QAction(tr("Save"), this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    toolBar->addAction(saveAction);
}

void MainWindow::newFile() {
    textEdit->clear();
    currentFile.clear();
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open the File"));
    QFile file(fileName);
    currentFile = fileName;

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot open file: ") + file.errorString());
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    textEdit->setText(text);
    file.close();
}

void MainWindow::saveFile() {
    if (currentFile.isEmpty()) {
        saveAsFile();
        return;
    }

    QFile file(currentFile);

    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot save file: ") + file.errorString());
        return;
    }

    QTextStream out(&file);
    QString text = textEdit->toPlainText();
    out << text;
    file.close();
}

void MainWindow::saveAsFile() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"));
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot save file: ") + file.errorString());
        return;
    }

    currentFile = fileName;
    QTextStream out(&file);
    QString text = textEdit->toPlainText();
    out << text;
    file.close();
}
