#include "graph.h"

Graph::Graph(QWidget* parent) : QWidget(parent)
{
    list = listNew();
    metrics = {
        .minimumYear = 0,
        .maximumYear = 0,
        .minimum = 0,
        .maximum = 0,
        .median = 0,
    };
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

int Graph::graphHeight() {
    return height() - 30;
}

int Graph::graphWidth() {
    return width() - 70;
}

double Graph::Mx() {
    return (double) graphWidth() / (metrics.maximumYear - metrics.minimumYear);
}

double Graph::My() {
    return ((double) graphHeight() / (metrics.maximum - metrics.minimum + 2));
}

QPoint Graph::getPoint(Node* node) {
    return QPoint{
        (width() - graphWidth()) + (int) (Mx() * (node->record.year - metrics.minimumYear)),
        graphHeight() - (int)(My() * (getColumnByIndex(&node->record, column) - metrics.minimum + 1)),
    };
}

void Graph::drawGraph(QPainter &painter) {
    Node* node = list.first ? (list.first->next) : 0;
    while (node) {
        QPoint current = getPoint(node);
        QPoint previous = node->prev ? getPoint(node->prev) : QPoint{0, 0};
        painter.setPen(Qt::black);
        painter.drawLine(previous, current);
        painter.setPen(QPen(Qt::gray, 1, Qt::DashDotLine));
        node = node->next;
    }
}

void Graph::drawAxis(QPainter &painter) {
    painter.setPen(QPen(Qt::black, 1, Qt::DashDotLine));
    painter.drawLine(0, graphHeight(), width(), graphHeight());
    for (int x = metrics.minimumYear; x <= metrics.maximumYear; x++) {
        double xAxis = (int) (Mx() * (x - metrics.minimumYear)) + width() - graphWidth();
        painter.setPen(Qt::black);
        painter.drawText(xAxis - 35, height() - 10, QString::number(x));
        painter.setPen(QPen(Qt::lightGray, 1, Qt::DashDotLine));
        painter.drawLine(xAxis, 0, xAxis, graphHeight());
    }
    for (int y = metrics.minimum - 1; y <= metrics.maximum + 1; y++) {
        double yAxis = graphHeight() - (int)(My() * (y - metrics.minimum + 1));
        if (yAxis > graphHeight()) {
            continue;
        }
        painter.setPen(Qt::black);
        painter.drawText(width() - graphWidth() - 30, yAxis - 5, QString::number(y));
        painter.setPen(QPen(Qt::lightGray, 1, Qt::DashDotLine));
        painter.drawLine((int) (width() - graphWidth()), yAxis, width(), yAxis);
    }
}

void Graph::paintEvent(QPaintEvent* ev) {
    QPainter painter(this);
    painter.setPen(Qt::black);
    if (metrics.maximumYear - metrics.minimumYear != 0) {
        drawAxis(painter);
        drawGraph(painter);
    }
    painter.drawRect(0, 0, width() - 1, height() - 1);
}

