#pragma once
#include <vector>
#include <unordered_map>
#include <string>

#include "UnityEngine/Vector3/Vector3.hpp"
#include "cache.hpp"

namespace Astar
{

	using v3 = UnityEngine::Vector3;


	class Node
	{
	public:
		std::string id;
		v3 pos{};
		std::shared_ptr<Node> parent;
		std::vector<std::shared_ptr<Node>> childNodes;
		float G = 0, H = 0;
		union
		{
			bool closed = false;
			bool unreachable;
		};
		float F();

		Node(std::string id, v3 nodePos, v3 startPos, v3 endPos, std::shared_ptr<Node> parent) :
			id(id), pos(nodePos)
		{
			G = v3::Distance(nodePos, startPos);
			H = v3::Distance(nodePos, endPos);
		}

	};

	class AstarPath
	{
	public:
		v3 start, end;
		std::vector<std::shared_ptr<Node>> openNodes;
		std::vector<std::shared_ptr<Node>> closedNodes;
	};

	void New(v3 start, v3 end);
	void Step();
	void UpdateRender();
	bool IsClosedNode(v3 nodePos);
	bool IsOpenNode(v3 nodePos);
	bool Raycast(v3 from, v3 dir, float maxDist);
}