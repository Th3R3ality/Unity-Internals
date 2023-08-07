#include "constraints.hpp"

#include <iostream>

#include "UnityEngine/Transform/Transform.hpp"

#include "config.hpp"

void constraint::apply()
{
	UnityEngine::Vector3 Vsrc;
	UnityEngine::Quaternion Qsrc;

	Vsrc = source->transform()->position();
	Qsrc = source->transform()->rotation();

	//Vsrc = source->transform()->localPosition();
	//Qsrc = source->transform()->localRotation();


	if (type & constraint_type::position)
		destination->transform()->position(Vsrc);

	if (type & constraint_type::rotation)
		destination->transform()->rotation(Qsrc);

	if (type & constraint_type::position)
		destination->transform()->localPosition(Vsrc);

	if (type & constraint_type::rotation)
		destination->transform()->localRotation(Qsrc);

}
