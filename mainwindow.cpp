#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->graph = new Graph(this->centralWidget());
    QGridLayout* layout = (QGridLayout*) this->centralWidget()->layout();
    layout->addWidget(this->graph, 3, 0, 1, 2);

    connect(ui->openFileDialog, &QPushButton::pressed, this, &MainWindow::openFile);
    connect(ui->filename, &QLineEdit::textChanged, this, &MainWindow::setFilename);
    connect(ui->loadData, &QPushButton::pressed, this, &MainWindow::loadData);
    connect(ui->regionComboBox, &QComboBox::currentTextChanged, this, &MainWindow::setRegion);
    connect(ui->spinBoxColumn, SIGNAL(valueChanged(int)), SLOT(setColumn(int)));
    connect(ui->calculateMetrics, &QPushButton::pressed, this, &MainWindow::updateView);


    doOperation(Operation::Init, &context, 0);
    updateUi();
}

QString getErrorString(Error error) {
    QString err;
    switch (error) {
    case Error::FileIsNotOpened:
        err = "File is not opened";
        break;
    case Error::IncorrectColumn:
        err = "Incorrect column";
        break;
    default:
        err = "None";
        break;
    }
    return err;
}

MainWindow::~MainWindow()
{
    doOperation(Operation::ClearRecords, &context, 0);
    delete graph;
    delete ui;
}

void MainWindow::updateView() {
    DemographList list = listNew();
    AppParams params;
    params.outputList = &list;
    Error error = doOperation(Operation::Calculate, &this->context, &params);
    if (error != Error::None) {
        QMessageBox box(this);
        box.setText(getErrorString(error));
        box.exec();
    } else {
        graph->draw(&list, &context.metrics, context.column);
        listClear(&list);
        updateUi();
    }
}

void MainWindow::updateUi() {
    ui->filename->setText(QString::fromLocal8Bit(context.filename));
    ui->countRecordsValue->setText(QString::number(context.records.count));
    ui->countCorruptedRecordsValue->setText(QString::number(context.countCorruptedRecords));
    ui->spinBoxColumn->setValue(context.column);
    ui->regionComboBox->setCurrentText(context.region);
    ui->minimumValueLabel->setText(QString::number(context.metrics.minimum));
    ui->maximumValueLabel->setText(QString::number(context.metrics.maximum));
    ui->medianValueLabel->setText(QString::number(context.metrics.median));
}

void MainWindow::updateRegions() {
    ui->regionComboBox->clear();
    AppParams params;
    DemographList regions = listNew();
    params.outputList = &regions;
    doOperation(Operation::GetRegions, &context, &params);
    Node* node = regions.first;
    while (node) {
        ui->regionComboBox->addItem(QString(node->record.region));
        node = node->next;
    }
    ui->regionComboBox->setCurrentIndex(0);
    listClear(&regions);
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
    QByteArray encoded = filename.toLocal8Bit();
    AppParams params;
    params.filename = encoded.data();
    doOperation(Operation::SetFilename, &this->context, &params);
    updateUi();
}



void MainWindow::loadData() {
    Error error = doOperation(Operation::LoadData, &this->context, 0);
    if (error != Error::None) {
        QMessageBox box(this);
        box.setText(getErrorString(error));
        box.exec();
    }
    updateRegions();
    updateView();
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
