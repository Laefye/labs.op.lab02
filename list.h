#ifndef LIST_H
#define LIST_H
#define MAX_REGION 60
#include <stddef.h>

struct DemographRecord {
    int year;
    char region[MAX_REGION];
    double npg;
    double birth_rate;
    double death_rate;
    double gdw;
    double urbanization;
};

double getColumnByIndex(DemographRecord* record, int index);

struct Node {
    struct Node* next;
    struct Node* prev;
    struct DemographRecord record;
};

struct DemographList {
    struct Node* first;
    struct Node* end;
    size_t count;
};

struct DemographList listNew();
void listPush(struct DemographList* list, struct DemographRecord* record);
void listClear(struct DemographList* list);

#endif // LIST_H
