#include "Vector3.hpp"

bool UnityEngine::Vector3::operator==(Vector3 right)
{
    if (x != right.x || y != right.y || z != right.z)
        return false;
    return true;
}
