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
		completed,
		invalid
	};

	Astar::AstarPath path;
	unsigned int idCounter = 0;
	std::shared_ptr<Node> currentNode = nullptr;
	nextAction todo = findBestOpenNode;

	// settings
	constexpr float stepLength = 2;
	constexpr int rayCount = 6;
	constexpr float weightH = 4;
	bool canSubStep = true;
	float subStepModifier = 0.25f;
	unsigned int maxNodeCount = 1000;

	void New(v3 start, v3 end)
	{
		end.y = start.y;
		//std::cout << "\n\nAstar ; New...\n";

		for (auto node : path.openNodes)
		{
			if (node == nullptr)
				continue;
			cache::removeDraw(node->id);
		}
		for (auto node : path.closedNodes)
		{
			if (node == nullptr)
				continue;
			cache::removeDraw(node->id);
		}

		for (auto node : path.foundPath)
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
		path.foundPath.clear();

		path.start = start;
		path.end = end;
		
		path.openNodes.push_back( std::make_shared<Node>(std::to_string(idCounter), path.start, path.start, path.end, nullptr));
		idCounter++;

		//cache::debugDraw("pathStart", cache::debugIcosahedron({ start, Lapis::deltaTime * 20,0.2f }, "00ff00bb"));
		cache::debugDraw("pathEnd", cache::debugIcosahedron({end, Lapis::deltaTime * 20, 0.2f}, "ff0000bb"));

		//std::cout << "Astar ; New [+]\n";
	}

	bool Step()
	{
		//std::cout << "\nAstar ; stepping...\n";

		switch (todo)
		{
		case findBestOpenNode:
		{
			//std::cout << "Astar ; findBestOpenNode...\n";

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
			if (currentNode == nullptr)
			{
				//std::cout << "Astar ; all nodes explored [!]\n";
				todo = invalid;
				break;
			}

			if (v3::Distance(currentNode->pos, path.end) < stepLength && !UnityEngine::Physics::Linecast(currentNode->pos, path.end)) // add linecast to end to make sure :)
			{
				for (auto& node : path.closedNodes)
					if (node != nullptr)
						if (node->id == currentNode->id)
							currentNode = node;

				//std::cout << "Astar ; Found End [+++]\n";
				todo = completed;
				break;
			}

			path.closedNodes.push_back(currentNode);
			auto it = std::find_if(path.openNodes.begin(), path.openNodes.end(), [&](std::shared_ptr<Node> const& node) {
				return (node->id == currentNode->id); });
			if (it != path.openNodes.end())
			{
				//std::cout << "Astar ; removing current node from open nodes [-]\n";
				path.openNodes.erase(it);
			}


			todo = processFoundNode;
		}
			//std::cout << "Astar ; findBestOpenNode [+]\n";
		break;

		case processFoundNode:
		{
			//std::cout << "Astar ; processFoundNode...\n";

			v3 pos = currentNode->pos;
			constexpr float segmentTheta = (float)(M_PI * 2) / rayCount;
			for (int i = 0; i < rayCount; i++)
			{
				v3 dir = v3(cosf(segmentTheta * i), 0, sinf(segmentTheta * i));
				auto step = dir * stepLength;

				std::shared_ptr<Node> nearbyClosedNode = nullptr;
				if (IsClosedNode(pos + step, 1, &nearbyClosedNode))
				{
					if (nearbyClosedNode->G < currentNode->parent->G)
						if (!UnityEngine::Physics::Linecast(currentNode->pos, nearbyClosedNode->pos))
						{
							currentNode->parent = nearbyClosedNode;
							currentNode->G = nearbyClosedNode->G + v3::Distance(currentNode->pos, nearbyClosedNode->pos);
						}
					continue;
				}
				if (Raycast(pos, dir, stepLength))
						continue;


				std::shared_ptr<Node> nearbyOpenNode = nullptr;
				if (!IsOpenNode(pos + step, 1, &nearbyOpenNode))
				{
					auto newNode = std::make_shared<Node>(std::to_string(idCounter), pos + step, path.start, path.end, currentNode);
					path.openNodes.push_back(newNode);
					idCounter++;
				}
			}


			currentNode = nullptr;
			todo = findBestOpenNode;

			if (idCounter > maxNodeCount)
			{
				//std::cout << "Astar ; reached maximum node count [/]\n";
				todo = invalid;
			}
		}
			break;

		case completed:
			if (currentNode != nullptr)
			{
				//std::cout << "Astar ; Backtracing Path\n";
				path.foundPath.push_back(currentNode);
				currentNode = currentNode->parent;
				break;
			}
			else
			{
				UpdateRender();
				std::cout << "Astar ; Path Complete [+++]\n";
				return false;
			}
		case invalid:
			std::cout << "Astar ; Couldn't Find Path [XXX]\n";
			return false;
		}

		//std::cout << "Astar ; stepping [+]\n";

		//UpdateRender();
		if (currentNode != nullptr)
			UpdateRenderPath();

		return true;
	}
	void UpdateRenderPath()
	{
		if (currentNode == nullptr)
			return;
	
		auto _currentNode = currentNode;
		static int depth = 0;

		while (depth > 0)
		{
			cache::removeDraw("path_" + std::to_string(depth));
			depth--;
		}

		while (_currentNode->parent != nullptr)
		{
			depth++;
			cache::debugDraw("path_" + std::to_string(depth), cache::debugLine3d(_currentNode->pos, _currentNode->parent->pos, "0000aa"));
			_currentNode = _currentNode->parent;
		}
	}

	void UpdateRender()
	{
		//std::cout << "Astar ; UpdateRender...\n";

		/*
		for (auto node : path.closedNodes)
			if (node != nullptr)
				cache::debugDraw(node->id, cache::debugCube(Lapis::Transform(node->pos, 0, 0.09f), "00000099"));
		/*
		for (auto node : path.openNodes)
			if (node != nullptr)
				cache::debugDraw(node->id, cache::debugCube(Lapis::Transform(node->pos, 0, 0.09f), "ffff0099"));
		
		if (currentNode != nullptr)
			cache::debugDraw(currentNode->id, cache::debugCube(Lapis::Transform(currentNode->pos, 0, 0.17f), "00aa0099"));

		*/
		for (auto node : path.foundPath)
			if (node != nullptr)
				cache::debugDraw(node->id, cache::debugIcosahedron(Lapis::Transform(node->pos, 0, 0.17f), "0000aa99"));


		//std::cout << "Astar ; UpdateRender [+]\n";
	}

	bool IsClosedNode(v3 nodePos, float leniency, std::shared_ptr<Node>* nearbyClosedNode)
	{
		for (auto node : path.closedNodes)
		{
			if (node == nullptr)
				continue;
			if (v3::Distance(node->pos, nodePos) < (stepLength * 0.7 * leniency))
			{
				if (nearbyClosedNode != nullptr)
					*nearbyClosedNode = node;
				//std::cout << "Astar ; Closed Node [-]\n";
				return true;
			}
		}
		return false;
	}

	bool IsOpenNode(v3 nodePos, float leniency, std::shared_ptr<Node>* nearbyOpenNode)
	{
		for (auto node : path.openNodes)
		{
			if (node == nullptr)
				continue;
			if (v3::Distance(node->pos, nodePos) < (stepLength * 0.7 * leniency))
			{
				//std::cout << "Astar ; Open Node [+]\n";
				if (nearbyOpenNode != nullptr)
					*nearbyOpenNode = node;
				return true;
			}
		}
		return false;
	}

	bool Raycast(v3 from, v3 dir, float maxDist)
	{
		UnityEngine::RaycastHit hitInfo;
		bool res = UnityEngine::Physics::Raycast(from, dir, hitInfo, maxDist);
		//std::cout << std::format("Astar ; Raycast [ {} ]\n", res);
		return res;
	}

	float Node::F()
	{
		return weightH * H + G;
	}
}
