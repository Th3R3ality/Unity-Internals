#pragma once
#include <vector>
#include <unordered_map>
#include <string>

#include "UnityEngine/Vector3/Vector3.hpp"
#include "cache.hpp"

#include "Node.h"
#include "NodeHeap.h"

namespace Astar
{

	using v3 = UnityEngine::Vector3;

	class AstarPath
	{
	public:
		v3 start, end;
		NodeHeap openNodes;
		//std::vector<std::shared_ptr<Node>> openNodes;
		std::vector<std::shared_ptr<Node>> closedNodes;
		std::vector<std::shared_ptr<Node>> foundPath;
	};

	void New(v3 start, v3 end);
	bool Step();
	void UpdateRenderPath();
	void UpdateRender();
	bool IsClosedNode(v3 nodePos, float leniency = 1.f, std::shared_ptr<Node>* nearbyClosedNode = nullptr);
	bool IsOpenNode(v3 nodePos, float leniency = 1.f, std::shared_ptr<Node>* nearbyOpenNode = nullptr);
	bool Raycast(v3 from, v3 dir, float maxDist);
}