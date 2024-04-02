#include "entrypoint.h"

Error doOperation(Operation operation, AppContext* context, AppParams* params) {
    Error error = Error::None;
    switch (operation) {
    case Init:
        init(context);
        break;
    case SetFilename:
        setFilename(context, params->filename);
        break;
    case LoadData:
        error = loadData(context);
        break;
    }
    return error;
}
