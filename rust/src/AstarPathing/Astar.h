#pragma once
#include <vector>
#include "UnityEngine/Vector3/Vector3.hpp"
#include "cache.hpp"
#include <list>

namespace Astar
{

	using v3 = UnityEngine::Vector3;


	class Node
	{
	public:
		v3 pos;
		float G, H;
		std::vector<Node> childNodes;
		union
		{
			bool checked;
			bool unreachable;
		};

		float F();
	};

	class AstarPath
	{
	public:
		Node start, end;
		std::vector<Node> openNodes;
		std::vector<Node> closedNodes;
	};

	void New(v3 start, v3 end);
	void Step();
}