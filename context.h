#ifndef CONTEXT_H
#define CONTEXT_H

#define MAX_FILENAME 200
#include <stddef.h>
#include "metrics.h"
#include "list.h"

struct AppContext {
    struct DemographList records;
    size_t countCorruptedRecords;
    char filename[MAX_FILENAME];
    char region[MAX_REGION];
    int column;
    struct Metrics metrics;
};

#endif // CONTEXT_H
