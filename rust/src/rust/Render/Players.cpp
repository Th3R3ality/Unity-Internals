#include "Players.h"

#include "Lapis/engine/LapisEngine.h"
#include "cache.hpp"

namespace Render
{
	void Players()
	{
		auto cachedPlayers = cache::cachedPlayers();

		auto local = cache::local();

		for (auto cachedPlayer : cachedPlayers) {
			auto player = cachedPlayer.first;

			if (player == local) continue;

			{
				using namespace Lapis;
				
				Draw::D3::Triangle(Transform(Vec3(player->transform()->position()) + Vec3::up * 2.2, Vec3(0, elapsedTime * 100, 0), Vec3(0.5f, -0.5, 0.5)), "0050FF");
				Draw::D3::Triangle(Transform(Vec3(player->transform()->position()) + Vec3::up * 2.2, Vec3(0,elapsedTime*100 + 90,0), Vec3(0.5f, -0.5, 0.5)), "0050FF");
				//Draw::D3::Line(local->transform()->position(), player->transform()->position(), "0050ff");
			}

		}
	}
}