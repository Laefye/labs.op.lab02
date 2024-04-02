#ifndef CONTEXT_H
#define CONTEXT_H
#define MAX_REGION 60
#define MAX_FILENAME 200
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

struct AppContext {
    struct DemographRecord* records;
    size_t countRecords;
    char filename[MAX_FILENAME];
};

#endif // CONTEXT_H
