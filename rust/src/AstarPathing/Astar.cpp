#include "Astar.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <format>

#include "Lapis/engine/LapisEngine.h"

#include "UnityEngine/Physics/Physics.hpp"


namespace Astar
{
	enum nextAction
	{
		findBestOpenNode,
		processFoundNode,
		completed
	};

	Astar::AstarPath path;
	unsigned int idCounter = 0;
	std::shared_ptr<Node> currentNode = nullptr;
	constexpr float stepLength = 0.7;
	nextAction todo = findBestOpenNode;


	void New(v3 start, v3 end)
	{
		std::cout << "\n\nAstar ; New...\n";

		for (auto& node : path.openNodes)
		{
			if (node == nullptr)
				continue;
			cache::removeDraw(node->id);
		}
		for (auto& node : path.closedNodes)
		{
			if (node == nullptr)
				continue;
			cache::removeDraw(node->id);
		}

		idCounter = 0;
		currentNode = nullptr;
		todo = findBestOpenNode;

		path.openNodes.clear();
		path.closedNodes.clear();

		path.start = start;
		path.end = end;
		
		path.openNodes.push_back( std::make_shared<Node>(Node(std::to_string(idCounter), path.start, path.start, path.end, nullptr)));
		idCounter++;

		//cache::debugDraw("pathStart", cache::debugIcosahedron({ start, Lapis::deltaTime * 20,0.2f }, "00ff00bb"));
		cache::debugDraw("pathEnd", cache::debugIcosahedron({end, Lapis::deltaTime * 20, 0.2f}, "ff0000bb"));

		std::cout << "Astar ; New [+]\n";
	}

	void Step()
	{
		std::cout << "Astar ; stepping...\n";

		switch (todo)
		{
		case findBestOpenNode:
		{
			std::cout << "Astar ; findBestOpenNode...\n";

			float lowest_f = 9999999.9f;
			for (auto& node : path.openNodes)
			{
				if (node == nullptr)
					continue;

				if (node->F() < lowest_f)
				{
					lowest_f = node->F();
					currentNode = node;
				}
			}
			if (v3::Distance(currentNode->pos, path.end) < stepLength)
			{
				std::cout << "Astar ; Found End [+++]\n";
				todo = completed;
				break;
			}

			path.closedNodes.push_back(currentNode);
			auto it = std::find_if(path.openNodes.begin(), path.openNodes.end(), [&](std::shared_ptr<Node> const& node) {
				std::cout << "node: " << node->id << " == current: " << currentNode->id << " ?\n";
				return (node->id == currentNode->id);
				});
			if (it != path.openNodes.end())
			{
				std::cout << "Astar ; removing current node from open nodes [-]\n";
				path.openNodes.erase(it);
			}


			todo = processFoundNode;
		}
		std::cout << "Astar ; findBestOpenNode [+]\n";
		break;

		case processFoundNode:
		{
			std::cout << "Astar ; processFoundNode...\n";

			constexpr int rayCount = 4;
			constexpr float segmentTheta = (float)(M_PI * 2) / rayCount;

			if (currentNode == nullptr)
			{
				std::cout << "Astar ; something went very wrong [/]\n";
				std::cout << "Astar ; something went very wrong [/]\n";
				std::cout << "Astar ; something went very wrong [/]\n";
				todo = findBestOpenNode;
				break;
			}
			v3 pos = currentNode->pos;
			for (int i = 0; i < rayCount; i++)
			{
				v3 dir = v3(cosf(segmentTheta * i), 0, sinf(segmentTheta * i));
				auto step = dir * stepLength;

				std::cout << "dir : " << dir << "\n";
				std::cout << "step : " << step << "\n";

				if (Raycast(pos, dir, stepLength) || IsClosedNode(pos + step))
					continue;

				if (!IsOpenNode(pos + step))
				{
					auto newNode = std::make_shared<Node>(Node(std::to_string(idCounter), pos + step, path.start, path.end, currentNode));
					path.openNodes.push_back(newNode);
					idCounter++;
				}
			}


			currentNode = nullptr;
			todo = findBestOpenNode;
		}
			std::cout << "Astar ; processFoundNode [+]\n";
			break;

		case completed:
		{
			std::cout << "Astar ; Path Complete [+++]\n";
		}
			break;
		}

		std::cout << "Astar ; stepping [+]\n";

		UpdateRender();
	}

	void UpdateRender()
	{
		std::cout << "Astar ; UpdateRender...\n";

		for (auto& node : path.closedNodes)
			if (node != nullptr)
				cache::debugDraw(node->id, cache::debugIcosahedron(Lapis::Transform(node->pos, 0, 0.1f), "00000099"));

		for (auto& node : path.openNodes)
			if (node != nullptr)
				cache::debugDraw(node->id, cache::debugIcosahedron(Lapis::Transform(node->pos, 0, 0.1f), "ffff0099"));
		
		if (currentNode != nullptr)
			cache::debugDraw(currentNode->id, cache::debugIcosahedron(Lapis::Transform(currentNode->pos, 0, 0.17f), "00aa0099"));

		std::cout << "Astar ; UpdateRender [+]\n";
	}

	bool IsClosedNode(v3 nodePos)
	{
		for (auto& node : path.closedNodes)
		{
			if (node == nullptr)
				continue;
			if (v3::Distance(node->pos, nodePos) < (stepLength / 2))
			{
				std::cout << "Astar ; Closed Node [-]\n";
				return true;
			}
		}
		return false;
	}

	bool IsOpenNode(v3 nodePos)
	{
		for (auto& node : path.openNodes)
		{
			if (node == nullptr)
				continue;
			if (v3::Distance(node->pos, nodePos) < (stepLength / 2))
			{
				std::cout << "Astar ; Open Node [+]\n";
				return true;
			}
		}
		return false;
	}

	bool Raycast(v3 from, v3 dir, float maxDist)
	{
		UnityEngine::RaycastHit hitInfo;
		bool res = UnityEngine::Physics::Raycast(from, dir, hitInfo, maxDist);
		std::cout << std::format("Astar ; Raycast [ {} ]\n", res);
		return res;
	}

	float Node::F()
	{
		return H + G;
	}
}
