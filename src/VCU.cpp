#include <VCU.hpp>

namespace VCU {
CO_OBJ_T* VCU::getObjectDictionary() {
    return &objectDictionary[0];
}

uint8_t VCU::getNumElements() {
    return OBJECT_DICTIONARY_SIZE;
}

uint8_t VCU::getNodeID() {
    return NODE_ID;
}

void VCU::process() {

}
}
