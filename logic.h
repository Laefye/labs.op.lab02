#ifndef LOGIC_H
#define LOGIC_H
#include "context.h"
#include "errors.h"
#define MAX_LINE 200
#define LAST_FIELD 7
#define FIRST_FIELD 3
#define YEAR_FIELD 1

void init(AppContext* context);
void setFilename(AppContext* context, const char* filename);
Error loadData(AppContext* context);
void clearRecords(AppContext* context);
Error calculate(AppContext* context, struct DemographList* output);
void setCalculationRegion(AppContext* context, const char* region);
void setCalculationColumn(AppContext* context, int column);
void getRegions(AppContext* context, struct DemographList* output);

#endif // LOGIC_H
