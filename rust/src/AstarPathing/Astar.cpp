#include "Astar.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <format>

#include "Lapis/engine/LapisEngine.h"

#include "UnityEngine/Physics/Physics.hpp"
#include <array>

#define deborg(x) //x

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
	constexpr float stepLength = 1;
	constexpr int rayCount = 6;
	constexpr float weightH = 4;
	bool canSubStep = false;
	float subStepModifier = 0.25f;
	unsigned int maxNodeCount = 5000;

	void New(v3 start, v3 end)
	{
		end.y = start.y;
		deborg(std::cout << "\n\nAstar ; New...\n";)

		for (auto node : path.openNodes.items)
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

		path.openNodes.items.clear();
		path.closedNodes.clear();
		path.foundPath.clear();

		path.start = start;
		path.end = end;
		
		path.openNodes.Add( std::make_shared<Node>(std::to_string(idCounter), path.start, path.start, path.end, nullptr, weightH));
		idCounter++;

		//cache::debugDraw("pathStart", cache::debugIcosahedron({ start, Lapis::deltaTime * 20,0.2f }, "00ff00bb"));
		cache::debugDraw("pathEnd", cache::debugIcosahedron({end, Lapis::deltaTime * 20, 0.2f}, "ff0000bb"));

		deborg(std::cout << "Astar ; New [+]\n";)
	}

	bool Step()
	{
		deborg(std::cout << "\nAstar ; stepping...\n";)

		switch (todo)
		{
		case findBestOpenNode:
		{
			deborg(std::cout << "Astar ; findBestOpenNode...\n";)

			currentNode = path.openNodes.RemoveFirst();
			
			if (currentNode == nullptr)
			{
				deborg(std::cout << "Astar ; all nodes explored [!]\n";)
				todo = invalid;
				break;
			}

			if (v3::Distance(currentNode->pos, path.end) < stepLength && !UnityEngine::Physics::Linecast(currentNode->pos, path.end))
			{
				for (auto& node : path.closedNodes)
					if (node != nullptr)
						if (node->id == currentNode->id)
							currentNode = node;

				deborg(std::cout << "Astar ; Found End [+++]\n";)
				todo = completed;
				break;
			}

			path.closedNodes.push_back(currentNode);
			
			todo = processFoundNode;
		}
			deborg(std::cout << "Astar ; findBestOpenNode [+]\n";)
		break;

		case processFoundNode:
		{
			deborg(std::cout << "Astar ; processFoundNode...\n";)

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
					auto newNode = std::make_shared<Node>(std::to_string(idCounter), pos + step, path.start, path.end, currentNode, weightH);
					path.openNodes.Add(newNode);
					idCounter++;
				}
			}


			currentNode = nullptr;
			todo = findBestOpenNode;

			if (idCounter > maxNodeCount)
			{
				std::cout << "Astar ; reached maximum node count [/]\n";
				todo = invalid;
			}
		}
			break;

		case completed:
			if (currentNode != nullptr)
			{
				deborg(std::cout << "Astar ; Backtracing Path\n";)
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

		deborg(std::cout << "Astar ; stepping [+]\n";)

		deborg(UpdateRender();)
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

		deborg(std::cout << "Astar ; UpdateRender...\n");

		
		deborg(
		for (auto node : path.closedNodes)
			if (node != nullptr)
				cache::debugDraw(node->id, cache::debugCube(Lapis::Transform(node->pos, 0, 0.09f), "00000099"));
		);
		deborg(
			for (auto node : path.openNodes.items)
				if (node != nullptr)
					cache::debugDraw(node->id, cache::debugCube(Lapis::Transform(node->pos, 0, 0.09f), "ffff0099"));
		);
		deborg(
		if (currentNode != nullptr)
			cache::debugDraw(currentNode->id, cache::debugCube(Lapis::Transform(currentNode->pos, 0, 0.17f), "00aa0099"));
		);
		
		for (auto node : path.foundPath)
			if (node != nullptr)
				cache::debugDraw(node->id, cache::debugIcosahedron(Lapis::Transform(node->pos, 0, 0.17f), "0000aa99"));


		std::cout << "Astar ; UpdateRender [+]\n";
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
				deborg(std::cout << "Astar ; Closed Node [-]\n";)
				return true;
			}
		}
		return false;
	}

	bool IsOpenNode(v3 nodePos, float leniency, std::shared_ptr<Node>* nearbyOpenNode)
	{
		for (auto node : path.openNodes.items)
		{
			if (node == nullptr)
				continue;
			if (v3::Distance(node->pos, nodePos) < (stepLength * 0.7 * leniency))
			{
				deborg(std::cout << "Astar ; Open Node [+]\n";)
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
		deborg(std::cout << std::format("Astar ; Raycast [ {} ]\n", res);)
		return res;
	}
}
