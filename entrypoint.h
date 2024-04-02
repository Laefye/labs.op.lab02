#ifndef ENTRYPOINT_H
#define ENTRYPOINT_H
#include "logic.h"

struct AppParams {
    const char* filename;
};

enum Operation {
    Init,
    SetFilename,
    LoadData,
};

Error doOperation(Operation operation, AppContext* context, AppParams* params);

#endif // ENTRYPOINT_H
