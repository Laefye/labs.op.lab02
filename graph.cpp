#include "graph.h"

Graph::Graph(QWidget* parent) : QWidget(parent)
{
    list = listNew();
}

Graph::~Graph() {
    listClear(&list);
}

void Graph::draw(DemographList* list, Metrics* metrics, int column) {
    listClear(&this->list);
    Node* node = list->first;
    while (node) {
        listPush(&this->list, &node->record);
        node = node->next;
    }
    this->metrics = *metrics;
    this->column = column;
    repaint();
}

QPoint Graph::getPoint(Node* node) {
    double Mx = (double) width() / (metrics.maximumYear - metrics.minimumYear);
    double My = ((double) height() / (metrics.maximum - metrics.minimum));
    return QPoint{
        (int) (Mx * (node->record.year - metrics.minimumYear)),
        height() - (int)(My * (getColumnByIndex(&node->record, column) - metrics.minimum))
    };
}

void Graph::paintEvent(QPaintEvent* ev) {
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.drawRect(0, 0, width() - 1, height() - 1);
    if (metrics.maximumYear - metrics.minimumYear != 0) {
        Node* node = list.first ? (list.first->next) : 0;
        while (node) {
            QPoint current = getPoint(node);
            QPoint previous = node->prev ? getPoint(node->prev) : QPoint{0, 0};
            painter.drawLine(previous, current);
            node = node->next;
        }
    }
}

