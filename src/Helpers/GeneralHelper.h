#ifndef HELPERS_GENERALHELPER_H_GUARD
#define HELPERS_GENERALHELPER_H_GUARD

#include <vector>
#include <cstdint>
#include <string>

#include "Enums.h"
#include "TypeTraits.h"

const std::string genUUID();
bool floatsAreEqual(float a, float b);
Vector2u8 getChankIndexByPosition(Vector3f &position);

#endif