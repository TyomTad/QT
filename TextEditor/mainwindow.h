#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QFile>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();

private:
    Ui::MainWindow *ui;
    QTextEdit *textEdit;
    QString currentFile;

    void setupEditor();
    void setupMenu();
    void setupToolbar();
};
#endif // MAINWINDOW_H
