#include "logic.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

void init(AppContext* context) {
    context->filename[0] = 0;
    context->region[0] = 0;
    context->column = 3;
    context->records = listNew();
    context->metrics = {.minimum = 0, .maximum = 0, .median = 0};
    clearRecords(context);
}

void setFilename(AppContext* context, const char* filename) {
    strcpy_s(context->filename, MAX_FILENAME, filename);
}

void skipLine(FILE* file) {
    char line[MAX_LINE];
    fgets(line, MAX_LINE, file);
}

int readRecord(FILE* file, DemographRecord* record) {
    char line[MAX_LINE];
    fgets(line, MAX_LINE, file);
    int count = -1;
    if (strlen(line) > strlen("\r\n")) {
        count = sscanf(line, "%d,%[^,],%lf,%lf,%lf,%lf,%lf", &record->year, &record->region, &record->npg, &record->birth_rate, &record->death_rate, &record->gdw, &record->urbanization);
    }
    return count;
}


Error readAllRecords(AppContext* context, FILE* file) {
    Error error = Error::None;
    skipLine(file);
    DemographRecord record;
    while (!feof(file)) {
        int count = readRecord(file, &record);
        if (count >= 0 && count != COUNT_FIELDS) {
            context->countCorruptedRecords++;
            continue;
        } else if (count < 0) {
            continue;
        }
        listPush(&context->records, &record);
    }
    return error;
}

Error loadData(AppContext* context) {
    Error error = Error::None;
    clearRecords(context);
    FILE* file = fopen(context->filename, "r");
    if (file == 0) {
        error = Error::NotFoundFile;
    } else {
        error = readAllRecords(context, file);
        fclose(file);
    }
    return error;
}

void clearRecords(AppContext* context) {
    listClear(&context->records);
    context->countCorruptedRecords = 0;
}

double getColumnByIndex(DemographRecord* record, int index) {
    double value = 0;
    switch (index) {
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

void calculateMetrics(AppContext* context, struct DemographList* list) {
    Node* node = list->first;
    if (node) {
        context->metrics.maximum = getColumnByIndex(&node->record, context->column);
        context->metrics.minimum = getColumnByIndex(&node->record, context->column);
    }  else {
        context->metrics.maximum = 0;
        context->metrics.minimum = 0;
        context->metrics.median = 0;
    }
    int i = 0;
    while (node) {
        context->metrics.maximum = getColumnByIndex(&node->record, context->column) > context->metrics.maximum ? getColumnByIndex(&node->record, context->column) : context->metrics.maximum;
        context->metrics.minimum = getColumnByIndex(&node->record, context->column) < context->metrics.minimum ? getColumnByIndex(&node->record, context->column) : context->metrics.minimum;
        if (!(list->count % 2) && i + 1 == list->count / 2) {
            context->metrics.median = getColumnByIndex(&node->record, context->column);
        } else if (list->count % 2 && i + 1 == list->count / 2) {
            context->metrics.median = (getColumnByIndex(&node->record, context->column) + getColumnByIndex(&node->next->record, context->column)) / 2;
        }
        node = node->next;
        i++;
    }
}

Error calculate(AppContext* context, struct DemographList* output) {
    Error error = Error::None;
    Node* node = context->records.first;
    while (node) {
        if (strlen(context->region) == 0 || (!strcmp(context->region, node->record.region))) {
            listPush(output, &node->record);
        }
        node = node->next;
    }
    calculateMetrics(context, output);
    return error;
}

void setCalculationRegion(AppContext* context, const char* region) {
    strcpy_s(context->region, MAX_REGION, region);
}

void setCalculationColumn(AppContext* context, int column) {
    context->column = column;
}
