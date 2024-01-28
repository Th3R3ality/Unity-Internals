#include "Pathing.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <format>
#include <algorithm>
#include <array>

#include "Lapis/engine/LapisEngine.h"

#include "UnityEngine/Physics/Physics.hpp"

namespace RustBot
{
	void Pather::New(v3 start, v3 end)
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
			cache::removeDraw("jump_" + node->id);
			cache::removeDraw("duck_" + node->id);
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

		this->openNodes.Add(std::make_shared<PathNode>(std::to_string(idCounter), this->start, this->start, this->end, nullptr, weightH));
		idCounter++;

		cache::debugDraw("pathStart", cache::debugIcosahedron({ start, 0, radius }, "00ff00bb"));
		cache::debugDraw("pathEnd", cache::debugIcosahedron({ end, 0, radius }, "ff0000bb"));

	}

#pragma warning(push)
#pragma warning(disable: 26819) // switch fallthrough warning
	bool Pather::Step(bool processSameStep)
	{
		switch (todo)
		{
		case findBestOpenNode:
		{
			if ((currentNode = openNodes.RemoveFirst()) == nullptr)
			{
				std::cout << "RustBot ; Exhausted all open nodes [-]\n";
				if (bestFoundNode != nullptr && bestFoundNode->parent != nullptr)
				{
					std::cout << "RustBot ; Resorting best found path [+]\n";
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

			//closedNodePartitioner->Add(currentNode);

			todo = processFoundNode;
			if (!processSameStep)
				break;
		}
		case processFoundNode:
		{
			v3 pos = currentNode->pos;
			const float segmentTheta = (float)(M_PI * 2) / rayCount;
			const float segmentDistance = sinf(segmentTheta / 2) * 2;
			constexpr float segmentThetaVertical = (float)(M_PI * 2) / 24;

			for (int i = 0; i < rayCount; i++)
			{
				for (int vertical = 0; vertical < (disableVertical ? 1 : 5); vertical++)
				{
					float pitch = (disableVertical ? 0 : -(segmentThetaVertical * 2) + segmentThetaVertical * vertical);
					auto yaw = (segmentTheta * i) + yawFix;

					v3 dir = v3(
						cosf(yaw) * cosf(pitch),
						sinf(pitch),
						sinf(yaw) * cosf(pitch)
					);
					auto step = (dir * stepLength);
					auto finalPos = pos + step;

					bool jump = false;
					bool duck = false;
					bool fall = false;

					bool contin = false;
					int maxSubs = 5;
					for (int j = 0; j < maxSubs; j++)
					{
						if (j == maxSubs - 1)
						{
							contin = true;
							break;
						}
						float jmod = 1.f - ((1.f / (maxSubs + 1)) * j);
						step = dir * (stepLength * jmod);
						finalPos = pos + step;
					

						bool overrideClosedCheck = false;

						RaycastHit hitInfo;
						if (UnityEngine::Physics::AutoCast(pos, dir, hitInfo, layerMask, stepLength, radius, capsuleTopOffset))
						{
							if (j != 0)
								continue;

							if (hitInfo.m_Distance < stepLength / 5)
							{
								if (!UnityEngine::Physics::AutoCast(pos, dir, hitInfo, layerMask, stepLength, radius, capsuleCrouchTopOffset))
								{
									if (pitch > 0)
										continue;
									duck = true;
									//contin = true;
								}
								else
								{
									if (UnityEngine::Physics::AutoCast(pos, { 0,1,0 }, layerMask, capsuleHeight - radius, radius, capsuleTopOffset))
										continue;
									UnityEngine::Physics::AutoCast(pos + v3(0, capsuleHeight - radius, 0), dir, hitInfo, layerMask, stepLength, radius, capsuleTopOffset);
									if (hitInfo.m_Distance < 0.2f)
										continue;
									finalPos = pos + v3(0, capsuleHeight - radius, 0) + dir * (hitInfo.m_Distance - 0.1f);
									jump = true;
									//contin = true;
								}
							}
							else
							{
								finalPos = pos + dir * (hitInfo.m_Distance - 0.1f);
								//contin = true;
								overrideClosedCheck = true;
							}
						}
						else
						{
							finalPos = pos + dir * stepLength;
						}
						

						std::shared_ptr<PathNode> nearbyClosedNode = nullptr;
						if (IsClosedNode(finalPos, overrideClosedCheck ? 0.2f : 1.1f, &nearbyClosedNode))
						{
							if (currentNode->parent != nullptr && nearbyClosedNode->G < currentNode->parent->G)
							{
								float dist = v3::Distance(currentNode->pos, nearbyClosedNode->pos);
								if (!UnityEngine::Physics::AutoCast(currentNode->pos, v3::Normalize(nearbyClosedNode->pos - currentNode->pos), layerMask, dist, radius, capsuleTopOffset))
								{
									currentNode->parent = nearbyClosedNode;
									currentNode->depth = nearbyClosedNode->depth + 1;
									currentNode->G = nearbyClosedNode->G + v3::Distance(currentNode->pos, nearbyClosedNode->pos);
								}
							}
							continue;
						}


						RaycastHit groundedHit;
						if (Grounded(finalPos, jump ? groundedHeight * 1.5f : groundedHeight, groundedHit))
						{
							if (groundedHit.m_Point.y < -0.8)
								continue;

							if (finalPos.y > currentNode->pos.y)
							{
								if (!jump)
								{
									if (groundedHit.m_Normal.y < 0.4)
										continue;
								}
								else
								{
									if (groundedHit.m_Normal.y < 0.1)
										continue;
								}

							}
							else
							{
								if (groundedHit.m_Normal.y < 0.08)
									continue;
							}
						}
						else
						{
							if (!Grounded(finalPos, maxFallHeight, groundedHit))
								continue;
							if (groundedHit.m_Normal.y < 0.4 || groundedHit.m_Point.y < -0.8)
								continue;
							if (UnityEngine::Physics::AutoCast(finalPos, { 0,-1,0 }, layerMask, (groundedHit.m_Distance - 0.02f), radius))
								continue;
							finalPos = (groundedHit.m_Point + groundedHit.m_Normal * radius) + Vector3(0,0.5,0);
							fall = true;
							if (IsClosedNode(finalPos, 1.f * jmod))
								continue;
						}

						std::shared_ptr<PathNode> nearbyOpenNode = nullptr;
						if (!IsOpenNode(finalPos, segmentDistance * jmod, &nearbyOpenNode))
						{
							auto newNode = std::make_shared<PathNode>(std::to_string(idCounter), finalPos, this->start, this->end, currentNode, weightH, jump, duck, fall);
							openNodes.Add(newNode);
							idCounter++;
						}
						break;
					}
					if (contin)
						continue;
					break;
				}
			}


			closedNodePartitioner->Add(currentNode);
			currentNode = nullptr;
			todo = findBestOpenNode;

			if (idCounter > maxNodeCount)
			{
				std::cout << "RustBot ; Reached maximum node count [/]\n";
				if (bestFoundNode != nullptr && bestFoundNode->parent != nullptr)
				{
					std::cout << "RustBot ; Resorting best found path [+]\n";
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
				foundPath.push_back(currentNode);
				currentNode = currentNode->parent;
				UpdateRender();
				break;
			}
			else
				todo = completed;
		case completed:
		{
			std::cout << "RustBot ; Path Complete [+++]\n";
			UpdateRender();
			return false;
		}
		break;
		case invalid:
			std::cout << "RustBot ; Couldn't Find Path [XXX]\n";
			UpdateRenderPath("000000");
			return false;
		}

		if (currentNode != nullptr)
			UpdateRenderPath("0000aa");

		UpdateRender();
		return true;
	}
#pragma warning(pop)

	bool Pather::Grounded(v3 pos, float length, RaycastHit& hitInfo)
	{
		bool grounded = false;
		if (!UnityEngine::Physics::AutoCast(pos, { 0,-1,0 }, hitInfo, layerMask, length, radius, 0))
			return false;

		RaycastHit hit2;
		if (!UnityEngine::Physics::AutoCast(pos, { 0,-1,0 }, hit2, layerMask, hitInfo.m_Distance + radius * 1.5))
			return false;

		return true;

	}

	void Pather::UpdateRenderPath(std::string hexCol, bool onlyRemove)
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

	void Pather::UpdateRender()
	{
		if (debugLevel < 1)
			return;

		////////// visualize path
		for (auto node : this->foundPath)
		{
			if (node != nullptr)
			{
				if (node->parent != nullptr)
				{
					cache::debugDraw("final_path_" + node->id, cache::debugLine3d(node->pos, node->parent->pos, "00aa00"));
				}
				if (node->jump)
				{
					cache::debugDraw("jump_" + node->id, cache::debugArrow3d(node->pos, Lapis::Vec3::up, "aa00aa"));
				}
				if (node->duck)
				{
					cache::debugDraw("duck_" + node->id, cache::debugArrow3d(node->pos, -Lapis::Vec3::up, "aa00aa"));
				}
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

	bool Pather::IsClosedNode(v3 nodePos, float leniency, std::shared_ptr<PathNode>* nearbyClosedNode)
	{
		for (auto partition : closedNodePartitioner->GetNearbyPartitions(nodePos))
		{
			for (auto node : partition->items)
			{
				if (node == nullptr)
					continue;
				if (v3::Distance(node->pos, nodePos) < (stepLength * 0.8 * leniency))
				{
					if (UnityEngine::Physics::Linecast(nodePos, node->pos, layerMask))
						continue;

					if (nearbyClosedNode != nullptr)
						*nearbyClosedNode = node;
					return true;
				}
			}
		}
		return false;
	}

	bool Pather::IsOpenNode(v3 nodePos, float leniency, std::shared_ptr<PathNode>* nearbyOpenNode)
	{
		for (auto node : this->openNodes.items)
		{
			if (node == nullptr)
				continue;
			if (v3::Distance(node->pos, nodePos) < (stepLength * 0.95 * leniency))
			{
				if (nearbyOpenNode != nullptr)
					*nearbyOpenNode = node;
				return true;
			}
		}
		return false;
	}


	bool Pather::GrabPath(std::vector<v3>& points)
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


}
