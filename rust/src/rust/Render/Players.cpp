#include "Players.h"

#include "Lapis/engine/LapisEngine.h"
#include "cache.hpp"

namespace Render
{
	void Players()
	{
		auto cachedPlayers = cache::cachedPlayers();

		for (auto cachedPlayer : cachedPlayers) {
			auto player = cachedPlayer.first;


		}
	}
}