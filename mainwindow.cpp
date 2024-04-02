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
    connect(ui->region, &QLineEdit::textChanged, this, &MainWindow::setRegion);
    connect(ui->spinBoxColumn, SIGNAL(valueChanged(int)), SLOT(setColumn(int)));
    connect(ui->calculateMetrics, &QPushButton::pressed, this, &MainWindow::updateTable);


    doOperation(Operation::Init, &context, 0);
    updateUi();

    ui->tableView->setModel(&model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    doOperation(Operation::ClearRecords, &context, 0);
    delete ui;
}

void MainWindow::updateTable() {
    DemographList list = listNew();
    AppParams params;
    params.calculateOutputList = &list;
    doOperation(Operation::Calculate, &this->context, &params);
    model.setRowCount(list.count);
    int i = 0;
    Node* node = list.first;
    while (node) {
        model.setItem(i, 0, new QStandardItem(QString::number(node->record.year)));
        model.setItem(i, 1, new QStandardItem(node->record.region));
        model.setItem(i, 2, new QStandardItem(QString::number(node->record.npg)));
        model.setItem(i, 3, new QStandardItem(QString::number(node->record.birth_rate)));
        model.setItem(i, 4, new QStandardItem(QString::number(node->record.death_rate)));
        model.setItem(i, 5, new QStandardItem(QString::number(node->record.gdw)));
        model.setItem(i, 6, new QStandardItem(QString::number(node->record.urbanization)));
        node = node->next;
        i++;
    }
    listClear(&list);
    model.setHeaderData(0, Qt::Horizontal, "Year");
    model.setHeaderData(1, Qt::Horizontal, "Region");
    model.setHeaderData(2, Qt::Horizontal, "Natural Population Growth");
    model.setHeaderData(3, Qt::Horizontal, "Birth Rate");
    model.setHeaderData(4, Qt::Horizontal, "Death Rate");
    model.setHeaderData(5, Qt::Horizontal, "General Demographic Weight");
    model.setHeaderData(6, Qt::Horizontal, "Urbanization");
    updateUi();
}

void MainWindow::updateUi() {
    ui->filename->setText(context.filename);
    ui->countRecordsValue->setText(QString::number(context.records.count));
    ui->countCorruptedRecordsValue->setText(QString::number(context.countCorruptedRecords));
    ui->spinBoxColumn->setValue(context.column);
    ui->region->setText(context.region);
    ui->minimumValueLabel->setText(QString::number(context.metrics.minimum));
    ui->maximumValueLabel->setText(QString::number(context.metrics.maximum));
    ui->medianValueLabel->setText(QString::number(context.metrics.median));
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
    AppParams params;
    params.filename = stdString.c_str();
    doOperation(Operation::SetFilename, &this->context, &params);
    updateUi();
}

#include <QMessageBox>

void MainWindow::loadData() {
    Error error = doOperation(Operation::LoadData, &this->context, 0);
    if (error != Error::None) {
        QMessageBox box(this);
        box.setText(QString::number(error));
        box.exec();
    }
    updateTable();
}

void MainWindow::setColumn(int val) {
    AppParams params;
    params.column = val;
    doOperation(Operation::SetCalculationColumn, &this->context, &params);
    updateUi();
}

void MainWindow::setRegion(const QString& region) {
    std::string stdString = region.toStdString();
    AppParams params;
    params.region = stdString.c_str();
    doOperation(Operation::SetCalculationRegion, &this->context, &params);
    updateUi();
}
