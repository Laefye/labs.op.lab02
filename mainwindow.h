#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QMessageBox>
#include "entrypoint.h"
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    Graph* graph;
    AppContext context;
    void updateUi();
    void updateView();
    void updateRegions();
    void openFile();
    void setFilename(const QString& filename);
    void loadData();
    void setRegion(const QString& filename);
private slots:
    void setColumn(int val);
};
#endif // MAINWINDOW_H
