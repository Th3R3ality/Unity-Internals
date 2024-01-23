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
		for (auto partition : closedNodePartitioner->GetAllPartitions())
		{
			for (auto node : partition->items)
			{
				if (node == nullptr)
					continue;
				cache::removeDraw(node->id);
			}
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
		bestFoundNode = nullptr;
		todo = findBestOpenNode;

		this->openNodes.items.clear();
		closedNodePartitioner->Clear();
		this->foundPath.clear();

		this->start = start;
		this->end = end;
		this->yawFix = atan2f(end.z - start.z, end.x - start.x);

		this->openNodes.Add( std::make_shared<Node>(std::to_string(idCounter), this->start, this->start, this->end, nullptr, weightH));
		idCounter++;

		cache::debugDraw("pathStart", cache::debugIcosahedron({ start, 0, radius }, "00ff00bb"));
		cache::debugDraw("pathEnd", cache::debugIcosahedron({end, 0, radius}, "ff0000bb"));

	}

#pragma warning(push)
#pragma warning(disable: 26819) // switch fallthrough warning
	bool AstarPath::Step(bool processSameStep)
	{
		switch (todo)
		{
		case findBestOpenNode:
		{
			if ((currentNode = openNodes.RemoveFirst()) == nullptr)
			{
				std::cout << "Astar ; Exhausted all open nodes [-]\n";
				if (bestFoundNode != nullptr && bestFoundNode->parent != nullptr)
				{
					std::cout << "Astar ; Resorting best found path [+]\n";
					currentNode = bestFoundNode;
					todo = backtracing;
					break;
				}

				todo = invalid;
				break;
			}

			if (bestFoundNode.get() == nullptr)
				bestFoundNode = currentNode;
			else
			{
				if (currentNode->F() < bestFoundNode->F())
					bestFoundNode = currentNode;
			}

			float endDist = v3::Distance(currentNode->pos, end);
			if (((endDist < stepLength + radius) && !UnityEngine::Physics::AutoCast(currentNode->pos, end, layerMask, endDist, radius))
				|| (maxPathDepth != 0 && currentNode->depth >= maxPathDepth))
			{
				todo = backtracing;
				break;
			}

			closedNodePartitioner->Add(currentNode);
			
			todo = processFoundNode;
			if (!processSameStep)
				break;
		}
		case processFoundNode:
		{
			v3 pos = currentNode->pos;
			if (!currentNode->inAir)
			{
				const float segmentTheta = (float)(M_PI * 2) / rayCount;
				constexpr float segmentThetaVertical = (float)(M_PI * 2) / 24;
				for (int i = 0; i < rayCount; i++)
				{
					for (int vertical = 0; vertical < (disableVertical ? 1 : 5); vertical++)
					{

						float pitch = (disableVertical ? 0 : -(segmentThetaVertical*2) + segmentThetaVertical * vertical);

						auto yaw = (segmentTheta * i) + yawFix;

						v3 dir = v3(
							cosf(yaw) * cosf(pitch),
							sinf(pitch),
							sinf(yaw) * cosf(pitch));

						auto step = dir * stepLength;
						auto finalPos = pos + step;

						std::shared_ptr<Node> nearbyClosedNode = nullptr;
						if (IsClosedNode(pos + step, 1, &nearbyClosedNode))
						{
							if (currentNode->parent != nullptr && nearbyClosedNode->G < currentNode->parent->G)
							{
								float dist = v3::Distance(currentNode->pos, nearbyClosedNode->pos);
								if (!UnityEngine::Physics::AutoCast(currentNode->pos, v3::Normalize(nearbyClosedNode->pos - currentNode->pos), layerMask, dist, this->radius))
								{
									currentNode->parent = nearbyClosedNode;
									currentNode->depth = nearbyClosedNode->depth + 1;
									currentNode->G = nearbyClosedNode->G + v3::Distance(currentNode->pos, nearbyClosedNode->pos);
								}
							}
							continue;
						}

						if (UnityEngine::Physics::AutoCast(pos, dir, layerMask, stepLength, radius))
							continue;

						bool nextInAir = false;
						if (!allowFlight)
						{
							RaycastHit inAirHitInfo;
							bool hitGroundedCheck = UnityEngine::Physics::AutoCast(finalPos, { 0,-1,0 }, inAirHitInfo, layerMask, max(0, inAirHeight), radius);
							if (hitGroundedCheck)
							{
								if (inAirHitInfo.m_Normal.y < 0.4 || inAirHitInfo.m_Point.y < -0.8)
									continue;
								if (!UnityEngine::Physics::AutoCast(finalPos, { 0,-1,0 }, inAirHitInfo, layerMask, max(0, inAirHeight + radius)))
									continue;
								if (inAirHitInfo.m_Normal.y < 0.5)
									continue;
							}
							else
							{
								RaycastHit fallHitInfo;
								if (!UnityEngine::Physics::AutoCast(finalPos, { 0,-1,0 }, fallHitInfo, layerMask, max(0, maxFallHeight), radius))
									continue;
								if (fallHitInfo.m_Normal.y < 0.5 || fallHitInfo.m_Point.y < -0.8)
									continue;
								if (!UnityEngine::Physics::AutoCast(finalPos, { 0,-1,0 }, fallHitInfo, layerMask, max(0, maxFallHeight + radius)))
									continue;
								if (fallHitInfo.m_Normal.y < 0.6)
									continue;
								if (IsClosedNode(fallHitInfo.m_Point + fallHitInfo.m_Normal * radius))
									continue;

								nextInAir = true;
							}
						}
						std::shared_ptr<Node> nearbyOpenNode = nullptr;
						if (!IsOpenNode(finalPos, 1, &nearbyOpenNode))
						{
							auto newNode = std::make_shared<Node>(std::to_string(idCounter), finalPos, this->start, this->end, currentNode, weightH);
							newNode->inAir = nextInAir;
							openNodes.Add(newNode);
							idCounter++;
						}
						break;
					}
				}
			}
			else
			{
				RaycastHit hitInfo;
				if (UnityEngine::Physics::AutoCast(pos, { 0,-1,0 }, hitInfo, layerMask, max(0, maxFallHeight), radius))
				{
					auto finalPos = Vector3(0, 0.1f, 0) + hitInfo.m_Point + hitInfo.m_Normal * radius;
					std::shared_ptr<Node> nearbyOpenNode = nullptr;
					if (!IsOpenNode(finalPos, 1, &nearbyOpenNode))
					{
						auto newNode = std::make_shared<Node>(std::to_string(idCounter), finalPos, this->start, this->end, currentNode, weightH);
						newNode->inAir = false;
						openNodes.Add(newNode);
						idCounter++;
					}
				}
			}

			currentNode = nullptr;
			todo = findBestOpenNode;

			if (idCounter > maxNodeCount)
			{
				std::cout << "Astar ; Reached maximum node count [/]\n";
				if (bestFoundNode != nullptr && bestFoundNode->parent != nullptr)
				{
					std::cout << "Astar ; Resorting best found path [+]\n";
					currentNode = bestFoundNode;
					todo = backtracing;
					break;
				}
				todo = invalid;
			}
			break;
		}
		case backtracing:
			if (currentNode != nullptr)
			{
				if (!currentNode->inAir)
					foundPath.push_back(currentNode);
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
#pragma warning(pop)

	void AstarPath::UpdateRenderPath(std::string hexCol, bool onlyRemove)
	{
		if (debugLevel < 1 && !onlyRemove)
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
		if (debugLevel < 1 || onlyRemove)
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

		////////// visualize path
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
		
		for (auto partition : closedNodePartitioner->GetAllPartitions())
			for (auto node : partition->items)
				if (node != nullptr)
					cache::debugDraw(node->id, cache::debugCube(Lapis::Transform(node->pos, 0, 0.09f), partition->color));

		for (auto node : this->openNodes.items)
			if (node != nullptr)
				cache::debugDraw(node->id, cache::debugCube(Lapis::Transform(node->pos, 0, 0.09f), "ffff0099"));

		if (currentNode != nullptr)
			cache::debugDraw(currentNode->id, cache::debugCube(Lapis::Transform(currentNode->pos, 0, 0.17f), "00aa0099"));
		
		
	}

	bool AstarPath::IsClosedNode(v3 nodePos, float leniency, std::shared_ptr<Node>* nearbyClosedNode)
	{
		for (auto partition : closedNodePartitioner->GetNearbyPartitions(nodePos))
		{
			for (auto node : partition->items)
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
