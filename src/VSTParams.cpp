#include "VSTParams.h"
#include "common.h"

VSTParams::VSTParams(int amount) {
    this->params = static_cast<VSTPARAM*>(malloc(amount * sizeof(VSTPARAM)));
}

VSTParams::~VSTParams() {
    SAFE_DELETE_PTR(this->params);
}
// 
// VSTPARAM& VSTParams::operator [] (int i) {
//     return this->params[i];
// }
