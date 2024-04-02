#ifndef LOGIC_H
#define LOGIC_H
#include "context.h"
#include "errors.h"
#define MAX_LINE 100
#define COUNT_FIELDS 7

void init(AppContext* context);
void setFilename(AppContext* context, const char* filename);
Error loadData(AppContext* context);


#endif // LOGIC_H
