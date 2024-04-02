#include "logic.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

void init(AppContext* context) {
    context->filename[0] = 0;
    context->records = 0;
    context->countRecords = 0;
}

void setFilename(AppContext* context, const char* filename) {
    strcpy_s(context->filename, MAX_FILENAME, filename);
}

void skipLine(FILE* file) {
    char line[MAX_LINE];
    fgets(line, MAX_LINE, file);
}

int readRecord(FILE* file, DemographRecord* record) {
    return fscanf(file, "%d,%[^,],%lf,%lf,%lf,%lf,%lf", &record->year, &record->region, &record->npg, &record->birth_rate, &record->death_rate, &record->gdw, &record->urbanization);
}

void addRecord(AppContext* context, DemographRecord* record) {
    context->records = (DemographRecord*) realloc(context->records, sizeof(DemographRecord) * (++context->countRecords));
    memcpy_s(&context->records[context->countRecords-1], sizeof(DemographRecord), record, sizeof(DemographRecord));
}

Error readAllRecords(AppContext* context, FILE* file) {
    Error error = Error::None;
    skipLine(file);
    DemographRecord record;
    while (!feof(file)) {
        if (readRecord(file, &record) != COUNT_FIELDS) {
            error = Error::CorruptedFile;
            break;
        }
        addRecord(context, &record);
    }
    return error;
}

Error loadData(AppContext* context) {
    Error error = Error::None;
    FILE* file = fopen(context->filename, "r");
    if (file == 0) {
        error = Error::NotFoundFile;
    } else {
        error = readAllRecords(context, file);
        fclose(file);
    }
    return error;
}
