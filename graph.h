#ifndef GRAPH_H
#define GRAPH_H
#include <QPainter>
#include <QWidget>
#include "list.h"
#include "metrics.h"

class Graph : public QWidget
{
private:
    DemographList list;
    Metrics metrics;
    int column;

    double Mx();
    double My();
    QPoint getPoint(Node* node);

    void drawAxis(QPainter& painter);
    void drawGraph(QPainter& painter);

    int graphHeight();
    int graphWidth();
public:
    Graph(QWidget* parent = 0);
    ~Graph();

    void draw(DemographList* list, Metrics* metrics, int column);

    void paintEvent(QPaintEvent *event) override;
};

#endif // GRAPH_H
