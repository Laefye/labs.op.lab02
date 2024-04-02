#include "entrypoint.h"

Error doOperation(Operation operation, AppContext* context, AppParams* params) {
    Error error = Error::None;
    switch (operation) {
    case Operation::Init:
        init(context);
        break;
    case Operation::SetFilename:
        setFilename(context, params->filename);
        break;
    case Operation::LoadData:
        error = loadData(context);
        break;
    case Operation::ClearRecords:
        clearRecords(context);
        break;
    case Operation::SetCalculationRegion:
        setCalculationRegion(context, params->region);
        break;
    case Operation::SetCalculationColumn:
        setCalculationColumn(context, params->column);
        break;
    case Operation::Calculate:
        error = calculate(context, params->calculateOutputList);
        break;
    }
    return error;
}
