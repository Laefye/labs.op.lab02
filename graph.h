#ifndef GRAPH_H
#define GRAPH_H
#include "QGraphicsView"
#include "list.h"
#include "metrics.h"

class Graph : public QWidget
{
private:
    DemographList list;
    Metrics metrics;
    int column;
public:
    Graph(QWidget* parent = 0);
    ~Graph();

    void draw(DemographList* list, Metrics* metrics, int column);
    QPoint getPoint(Node* node);

    void paintEvent(QPaintEvent *event) override;
};

#endif // GRAPH_H
