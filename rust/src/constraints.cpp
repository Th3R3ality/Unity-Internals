#include "constraints.hpp"

#include <iostream>

#include "UnityEngine/Transform/Transform.hpp"

void constraint::apply()
{
	if (type & constraint_type::position)
		destination->transform()->position(source->transform()->position());

	if (type & constraint_type::rotation)
		destination->transform()->rotation(source->transform()->rotation());
	
}
