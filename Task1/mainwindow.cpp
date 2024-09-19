#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: lightblue;");

    Widgets();
}

void MainWindow::Widgets() {
    add = new QPushButton("Add", this);
    read = new QPushButton("Remove", this);
    write = new QPushButton("Edit", this);

    connect(add, &QAbstractButton::clicked, this, &MainWindow::Adding);
    connect(read, &QAbstractButton::clicked, this, &MainWindow::Removing);
    connect(write, &QAbstractButton::clicked, this, &MainWindow::Editing);

    add->setMaximumSize(200, 80);
    add->setStyleSheet("background-color: white;"
                       "color: black;"
                       "border-radius: 8px;"
                       "font-size: 25px;");
    read->setMaximumSize(200, 80);
    read->setStyleSheet("background-color: white;"
                        "color: black;"
                        "border-radius: 8px;"
                        "font-size: 25px;");
    write->setMaximumSize(200, 80);
    write->setStyleSheet("background-color: white;"
                         "color: black;"
                         "border-radius: 8px;"
                         "font-size: 25px;");

    w1 = new QVBoxLayout();
    w1->addWidget(add);
    w1->addWidget(read);
    w1->addWidget(write);

    textDisplay = new QTextEdit(this);
    textDisplay->setStyleSheet("background-color: white; color: black;");
    textDisplay->setReadOnly(true);

    QWidget* left = new QWidget(this);
    left->setLayout(w1);
    left->setStyleSheet("background-color: lightblue;"
                        "border-radius: 4px;");

    QWidget* right = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(right);
    rightLayout->addWidget(textDisplay);
    right->setLayout(rightLayout);
    right->setStyleSheet("background-color: darkblue;"
                         "border-radius: 4px;");

    wid = new QWidget(this);
    h1 = new QHBoxLayout();
    h1->addWidget(left, 1);
    h1->addWidget(right, 2);
    wid->setLayout(h1);

    setCentralWidget(wid);

    LoadFileContent();
}

void MainWindow::LoadFileContent() {
    QFile file("hello.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString content = in.readAll();
        textDisplay->setText(content);
        file.close();
    }
}

void MainWindow::Adding() {
    QDialog dialog(this);
    dialog.setWindowTitle("Add Contact");

    QFormLayout form(&dialog);

    QLineEdit *nameLineEdit = new QLineEdit(&dialog);
    form.addRow("Name Surname:", nameLineEdit);

    QLineEdit *phoneLineEdit = new QLineEdit(&dialog);
    form.addRow("Phone Number:", phoneLineEdit);

    QLineEdit *emailLineEdit = new QLineEdit(&dialog);
    form.addRow("Email:", emailLineEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString name = nameLineEdit->text();
        QString phone = phoneLineEdit->text();
        QString email = emailLineEdit->text();

        if (name.isEmpty() || phone.isEmpty() || email.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "All fields must be filled.");
            return;
        }

        QFile file("hello.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream stream(&file);

            stream << "Name Surname: " << name << "\n"
                   << "Phone Number: " << phone << "\n"
                   << "Email: " << email << "\n\n";

            textDisplay->append("Name Surname: " + name);
            textDisplay->append("Phone Number: " + phone);
            textDisplay->append("Email: " + email);
            textDisplay->append("");  // Add extra line for separation

            file.close();
        } else {
            QMessageBox::warning(this, "Error", "Could not open file for writing.");
        }
    }
}



void MainWindow::Removing() {
    QFile file("hello.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file for reading.");
        return;
    }

    QTextStream stream(&file);
    QStringList contacts;
    QStringList names;

    while (!stream.atEnd()) {
        QString name = stream.readLine();
        QString phone = stream.readLine();
        QString email = stream.readLine();
        QString emptyLine = stream.readLine();

        if (!name.isEmpty()) {
            contacts << name << phone << email << emptyLine;
            names << name.mid(14);
        }
    }
    file.close();

    bool ok;
    QString nameToRemove = QInputDialog::getItem(this, "Remove Contact", "Select a contact to remove:", names, 0, false, &ok);

    if (ok && !nameToRemove.isEmpty()) {
        int index = names.indexOf(nameToRemove);
        if (index != -1) {
            int startIndex = index * 4;
            for (int i = 0; i < 4; ++i) {
                contacts.removeAt(startIndex);
            }

            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                for (const QString& contact : contacts) {
                    out << contact << "\n";
                }
                file.close();

                textDisplay->setText(contacts.join("\n"));
            } else {
                QMessageBox::warning(this, "Error", "Could not open file for writing.");
            }
        }
    }
}


void MainWindow::Editing() {
    QFile file("hello.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file for reading.");
        return;
    }

    QTextStream stream(&file);
    QStringList contacts;
    QStringList names;

    while (!stream.atEnd()) {
        QString name = stream.readLine();
        QString phone = stream.readLine();
        QString email = stream.readLine();
        QString emptyLine = stream.readLine(); // To capture the empty line after each contact

        if (!name.isEmpty()) {
            contacts << name << phone << email << emptyLine;
            names << name.mid(14);  // Extract the actual name (skip "Name Surname: ")
        }
    }
    file.close();

    bool ok;
    QString nameToEdit = QInputDialog::getItem(this, "Edit Contact", "Select a contact to edit:", names, 0, false, &ok);

    if (ok && !nameToEdit.isEmpty()) {
        int index = names.indexOf(nameToEdit);
        if (index != -1) {
            int startIndex = index * 4;

            QString existingName = contacts[startIndex];
            QString existingPhone = contacts[startIndex + 1];
            QString existingEmail = contacts[startIndex + 2];

            QDialog dialog(this);
            dialog.setWindowTitle("Edit Contact");

            QFormLayout form(&dialog);
            QLineEdit *nameLineEdit = new QLineEdit(&dialog);
            nameLineEdit->setText(existingName.mid(14)); // Set existing name
            form.addRow("Name Surname:", nameLineEdit);

            QLineEdit *phoneLineEdit = new QLineEdit(&dialog);
            phoneLineEdit->setText(existingPhone.mid(15)); // Set existing phone
            form.addRow("Phone Number:", phoneLineEdit);

            QLineEdit *emailLineEdit = new QLineEdit(&dialog);
            emailLineEdit->setText(existingEmail.mid(7)); // Set existing email
            form.addRow("Email:", emailLineEdit);

            QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
            form.addRow(&buttonBox);

            QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
            QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

            if (dialog.exec() == QDialog::Accepted) {
                QString newName = "Name Surname: " + nameLineEdit->text();
                QString newPhone = "Phone Number: " + phoneLineEdit->text();
                QString newEmail = "Email: " + emailLineEdit->text();

                contacts[startIndex] = newName;
                contacts[startIndex + 1] = newPhone;
                contacts[startIndex + 2] = newEmail;

                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&file);
                    for (const QString& contact : contacts) {
                        out << contact << "\n";
                    }
                    file.close();

                    textDisplay->setText(contacts.join("\n"));
                } else {
                    QMessageBox::warning(this, "Error", "Could not open file for writing.");
                }
            }
        }
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
