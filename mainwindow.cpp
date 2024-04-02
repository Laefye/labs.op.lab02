#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->openFileDialog, &QPushButton::pressed, this, &MainWindow::openFile);
    connect(ui->filename, &QLineEdit::textChanged, this, &MainWindow::setFilename);
    connect(ui->loadData, &QPushButton::pressed, this, &MainWindow::loadData);

    doOperation(Operation::Init, &context, 0);
    updateUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateUi() {
    ui->filename->setText(context.filename);
    if (context.countRecords > 0) {
        QStandardItemModel* model = new QStandardItemModel(context.countRecords, COUNT_FIELDS);
        for (size_t i = 0; i < context.countRecords; i++) {
            model->setItem(i, 0, new QStandardItem(QString::number(context.records[i].year)));
            model->setItem(i, 1, new QStandardItem(context.records[i].region));
            model->setItem(i, 2, new QStandardItem(QString::number(context.records[i].npg)));
            model->setItem(i, 3, new QStandardItem(QString::number(context.records[i].birth_rate)));
            model->setItem(i, 4, new QStandardItem(QString::number(context.records[i].death_rate)));
            model->setItem(i, 5, new QStandardItem(QString::number(context.records[i].gdw)));
            model->setItem(i, 6, new QStandardItem(QString::number(context.records[i].urbanization)));
        }
        model->setHeaderData(0, Qt::Horizontal, "Year");
        model->setHeaderData(1, Qt::Horizontal, "Region");
        model->setHeaderData(2, Qt::Horizontal, "Natural Population Growth");
        model->setHeaderData(3, Qt::Horizontal, "Birth Rate");
        model->setHeaderData(4, Qt::Horizontal, "Death Rate");
        model->setHeaderData(5, Qt::Horizontal, "General Demographic Weight");
        model->setHeaderData(6, Qt::Horizontal, "Urbanization");
        ui->tableView->setModel(model);
    }
}

void MainWindow::openFile() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("CSV (*.csv)");
    if (dialog.exec()) {
        QStringList selectedFiles = dialog.selectedFiles();
        setFilename(selectedFiles.first());
    }
}

void MainWindow::setFilename(const QString &filename) {
    std::string stdString = filename.toStdString();
    AppParams params = {
        .filename = stdString.c_str(),
    };
    doOperation(Operation::SetFilename, &this->context, &params);
    updateUi();
}

void MainWindow::loadData() {
    Error error = doOperation(Operation::LoadData, &this->context, 0);
    updateUi();

}
