#include "constraints.hpp"

#include <iostream>

#include "UnityEngine/Transform/Transform.hpp"

void constraint::apply()
{
	if (type & constraint_type::position)
		destination->transform()->localPosition(source->transform()->localPosition());

	if (type & constraint_type::rotation)
		destination->transform()->localRotation(source->transform()->localRotation());
	
}
