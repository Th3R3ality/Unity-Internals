#include "Vector3.hpp"

#include <iostream>

bool UnityEngine::Vector3::operator==(Vector3 right)
{
    if (this->x != right.x || this->y != right.y || this->z != right.z)
    {
        return false;
    }
    return true;
}
