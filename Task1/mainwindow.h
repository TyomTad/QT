#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget* wid{nullptr};
    QPushButton* add{nullptr};
    QPushButton* read{nullptr};
    QPushButton* write{nullptr};
    QVBoxLayout* w1{nullptr};
    QVBoxLayout* w2{nullptr};
    QHBoxLayout* h1{nullptr};
    QTextEdit* textDisplay{nullptr};

private slots:
    void Widgets();
    void Adding();
    void Removing();
    void Editing();
    void LoadFileContent();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
