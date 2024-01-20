#pragma once

inline int fastrand()
{
	static int seed = 0;
	seed = (214013 * seed + 2531011);
	return (seed >> 16) & 0x7FFF;
}