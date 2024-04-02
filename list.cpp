#include "list.h"
#include <stdlib.h>
#include <string.h>

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
    memcpy_s(&node->record, sizeof(DemographRecord), record, sizeof(DemographRecord));
    if (!list->first) {
        list->first = node;
        list->end = node;
    } else {
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
