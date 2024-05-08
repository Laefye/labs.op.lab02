#include "list.h"
#include <stdlib.h>
#include <string.h>

double getColumnByIndex(DemographRecord* record, int index) {
    double value = 0;
    switch (index) {
    case 1:
        value = record->year;
        break;
    case 3:
        value = record->npg;
        break;
    case 4:
        value = record->birth_rate;
        break;
    case 5:
        value = record->death_rate;
        break;
    case 6:
        value = record->gdw;
        break;
    case 7:
        value = record->urbanization;
        break;
    default:
        break;
    }
    return value;
}

DemographList listNew() {
    return DemographList {
        .first = 0,
        .end = 0,
        .count = 0,
    };
}

void listPush(DemographList* list, DemographRecord* record) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->next = 0;
    node->prev = 0;
    memcpy_s(&node->record, sizeof(DemographRecord), record, sizeof(DemographRecord));
    if (!list->first) {
        list->first = node;
        list->end = node;
    } else {
        node->prev = list->end;
        list->end->next = node;
        list->end = node;
    }
    list->count++;
}

void listClear(struct DemographList* list) {
    Node* node = list->first;
    while (node) {
        Node* previous = node;
        node = node->next;
        free(previous);
    }
    list->first = 0;
    list->end = 0;
    list->count = 0;
}
