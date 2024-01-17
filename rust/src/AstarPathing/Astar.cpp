#include "Astar.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <format>
#include <algorithm>

#include "Lapis/engine/LapisEngine.h"

#include "UnityEngine/Physics/Physics.hpp"
#include <array>


namespace Astar
{


	void AstarPath::New(v3 start, v3 end)
	{
		for (auto node : this->openNodes.items)
		{
			if (node == nullptr)
				continue;
			cache::removeDraw(node->id);
		}
		for (auto node : this->closedNodes)
		{
			if (node == nullptr)
				continue;
			cache::removeDraw(node->id);
		}

		for (auto node : this->foundPath)
		{
			if (node == nullptr)
				continue;
			cache::removeDraw(node->id);
			cache::removeDraw("final_path_" + node->id);
		}

		UpdateRenderPath("000000", true);

		idCounter = 0;
		currentNode = nullptr;
		todo = findBestOpenNode;

		this->openNodes.items.clear();
		this->closedNodes.clear();
		this->foundPath.clear();

		this->start = start;
		this->end = end;
		
		this->yawFix = atan2f(end.z - start.z, end.x - start.x);

		this->openNodes.Add( std::make_shared<Node>(std::to_string(idCounter), this->start, this->start, this->end, nullptr, weightH));
		idCounter++;

		//cache::debugDraw("pathStart", cache::debugIcosahedron({ start, Lapis::deltaTime * 20,0.2f }, "00ff00bb"));
		cache::debugDraw("pathEnd", cache::debugIcosahedron({end, Lapis::deltaTime * 20, 0.2f}, "ff0000bb"));

	}

#pragma warning disable 26819 // switch fallthrough warning
	bool AstarPath::Step(bool processSameStep)
	{
		switch (todo)
		{
		case findBestOpenNode:
		{
			currentNode = this->openNodes.RemoveFirst();
			
			if (currentNode == nullptr)
			{
				todo = invalid;
				break;
			}

			float endDist = v3::Distance(currentNode->pos, this->end);
			static RaycastHit hitInfo{};
			if ((endDist < stepLength && !UnityEngine::Physics::AutoCast(currentNode->pos, this->end, hitInfo, layerMask, endDist, this->radius))
				|| (maxPathDepth != 0 && currentNode->depth >= maxPathDepth))
			{
				for (auto& node : this->closedNodes)
					if (node != nullptr)
						if (node->id == currentNode->id)
							currentNode = node;

				todo = backtracing;
				break;
			}

			this->closedNodes.push_back(currentNode);
			
			todo = processFoundNode;
			if (!processSameStep)
				break;
		}
		case processFoundNode:
		{
			v3 pos = currentNode->pos;
			const float segmentTheta = (float)(M_PI * 2) / rayCount;
			constexpr float segmentThetaVertical = (float)(M_PI * 2) / 12;
			for (int i = 0; i < rayCount; i++)
			{
				for (int vertical = 0; vertical < (disableVertical ? 1 : 3); vertical++)
				{

					int choice = preferFlight ? 1 : -1;

					float pitch = segmentThetaVertical * (vertical < 1 ? 0 : vertical < 2 ? choice : -choice);

					auto yaw = (segmentTheta * i) + yawFix;

					v3 dir = v3(
						cosf(yaw) * cosf(pitch),
						sinf(pitch), 
						sinf(yaw) * cosf(pitch));

					auto step = dir * stepLength;

					std::shared_ptr<Node> nearbyClosedNode = nullptr;
					if (IsClosedNode(pos + step, 1, &nearbyClosedNode))
					{
						if (currentNode->parent != nullptr && nearbyClosedNode->G < currentNode->parent->G)
						{
							float dist = v3::Distance(currentNode->pos, nearbyClosedNode->pos);
							static RaycastHit hitInfo{};
							if (!UnityEngine::Physics::AutoCast(currentNode->pos, v3::Normalize(nearbyClosedNode->pos - currentNode->pos), hitInfo, layerMask, dist, this->radius))
							{
								currentNode->parent = nearbyClosedNode;
								currentNode->depth = nearbyClosedNode->depth + 1;
								currentNode->G = nearbyClosedNode->G + v3::Distance(currentNode->pos, nearbyClosedNode->pos);
							}
						}
						continue;
					}

					static RaycastHit hitInfo;
					if (UnityEngine::Physics::AutoCast(pos, dir, hitInfo, layerMask, stepLength, radius) || (!allowFlight && (!UnityEngine::Physics::AutoCast(pos, {0,-1,0}, hitInfo, layerMask, max(0, flightCheckHeight - radius), radius/2))))
						continue;


					std::shared_ptr<Node> nearbyOpenNode = nullptr;
					if (!IsOpenNode(pos + step, 1, &nearbyOpenNode))
					{
						auto newNode = std::make_shared<Node>(std::to_string(idCounter), pos + step, this->start, this->end, currentNode, weightH);
						this->openNodes.Add(newNode);
						idCounter++;
					}
				}
			}

			currentNode = nullptr;
			todo = findBestOpenNode;

			if (idCounter > maxNodeCount)
			{
				std::cout << "Astar ; reached maximum node count [/]\n";
				this->todo = invalid;
			}
			break;
		}
		case backtracing:
			if (currentNode != nullptr)
			{
				this->foundPath.push_back(currentNode);
				currentNode = currentNode->parent;
				UpdateRender();
				break;
			}
			else
				todo = completed;
		case completed:
			{
				std::cout << "Astar ; Path Complete [+++]\n";
				UpdateRender();
				return false;
			}
			break;
		case invalid:
			std::cout << "Astar ; Couldn't Find Path [XXX]\n";
			UpdateRenderPath("000000");
			return false;
		}

		if (currentNode != nullptr)
			UpdateRenderPath("0000aa");

		UpdateRender();
		return true;
	}
#pragma warning restore 26819

	void AstarPath::UpdateRenderPath(std::string hexCol, bool onlyRemove)
	{
		if (debugLevel < 1)
			return;
		if (currentNode == nullptr)
			return;
	
		auto _currentNode = currentNode;
		static int depth = 0;

		while (depth > 0)
		{
			cache::removeDraw("path_" + std::to_string(depth));
			depth--;
		}
		if (onlyRemove)
			return;

		while (_currentNode->parent != nullptr)
		{
			depth++;
			cache::debugDraw("path_" + std::to_string(depth), cache::debugLine3d(_currentNode->pos, _currentNode->parent->pos, hexCol.c_str()));
			_currentNode = _currentNode->parent;
		}
	}

	void AstarPath::UpdateRender()
	{
		if (debugLevel < 1)
			return;

		////////// visualise path
		for (auto node : this->foundPath)
		{
			if (node != nullptr)
			{
				if (node->parent != nullptr)
					cache::debugDraw("final_path_" + node->id, cache::debugLine3d(node->pos, node->parent->pos, "00aa00"));
				cache::debugDraw(node->id, cache::debugIcosahedron(Lapis::Transform(node->pos, 0, 0.04f), "00aa0099"));
			}
		}
		

		////////// Verbose debugging
		if (debugLevel < 2)
			return;
		
		for (auto node : this->closedNodes)
			if (node != nullptr)
				cache::debugDraw(node->id, cache::debugCube(Lapis::Transform(node->pos, 0, 0.09f), "00000099"));

		for (auto node : this->openNodes.items)
			if (node != nullptr)
				cache::debugDraw(node->id, cache::debugCube(Lapis::Transform(node->pos, 0, 0.09f), "ffff0099"));

		if (currentNode != nullptr)
			cache::debugDraw(currentNode->id, cache::debugCube(Lapis::Transform(currentNode->pos, 0, 0.17f), "00aa0099"));
		
		
	}

	bool AstarPath::IsClosedNode(v3 nodePos, float leniency, std::shared_ptr<Node>* nearbyClosedNode)
	{
		for (auto node : this->closedNodes)
		{
			if (node == nullptr)
				continue;
			if (v3::Distance(node->pos, nodePos) < (stepLength * 0.5 * leniency))
			{
				if (nearbyClosedNode != nullptr)
					*nearbyClosedNode = node;
				return true;
			}
		}
		return false;
	}

	bool AstarPath::IsOpenNode(v3 nodePos, float leniency, std::shared_ptr<Node>* nearbyOpenNode)
	{
		for (auto node : this->openNodes.items)
		{
			if (node == nullptr)
				continue;
			if (v3::Distance(node->pos, nodePos) < (stepLength * 0.5 * leniency))
			{
				if (nearbyOpenNode != nullptr)
					*nearbyOpenNode = node;
				return true;
			}
		}
		return false;
	}


	bool AstarPath::GrabPath(std::vector<v3>& points)
	{
		if (todo != completed && todo != invalid)
			return false;

		points.clear();
		for (auto node : foundPath)
		{
			if (node != nullptr)
				points.push_back(node->pos);
		}

		std::reverse(points.begin(), points.end());
		//points.push_back(this->end);
		return true;
	}

	bool Raycast(v3 from, v3 dir, float maxDist, int layerMask)
	{
		UnityEngine::RaycastHit hitInfo;
		bool res = UnityEngine::Physics::Raycast(from, dir, hitInfo, maxDist, layerMask);
		return res;
	}
}
