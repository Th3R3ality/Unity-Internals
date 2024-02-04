#include "Pathing.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <format>
#include <algorithm>
#include <array>

#include "Lapis/engine/LapisEngine.h"

#include "UnityEngine/Physics/Physics.hpp"

int dbg = 0;
int dbg3 = 0;
int dbg4 = 0;
int dbgj = 0;
int dbgd = 0;

namespace RustBot
{
	void Pather::New(v3 startPos, v3 goalPos, GoalType goalType)
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

		openNodes.items.clear();
		closedNodePartitioner->Clear();
		foundPath.clear();

		start = startPos;
		goal = Goal(goalType, goalPos);

		yawFix = atan2f(goalPos.z - startPos.z, goalPos.x - startPos.x);

		openNodes.Add(std::make_shared<PathNode>(std::to_string(idCounter), start + v3(0,0.1f,0), goal, nullptr, weightH));
		idCounter++;

		//cache::debugDraw("pathStart", cache::debugIcosahedron({ startPos, 0, radius }, "00ff00bb"));
		//cache::debugDraw("pathEnd", cache::debugIcosahedron({ goalPos, 0, radius }, "ff0000bb"));
		isInitialNode = true;
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
				if (allowResortUponExhaust)
				{
					if (bestFoundNode != nullptr && bestFoundNode->parent != nullptr)
					{
						std::cout << "RustBot ; Resorting best found path [+]\n";
						currentNode = bestFoundNode;
						todo = backtracing;
						break;
					}
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

			float endDist = v3::Distance(currentNode->pos, goal.Position(currentNode->pos));
			if (((endDist < stepLength + radius) && !UnityEngine::Physics::AutoCast(currentNode->pos, goal.Position(currentNode->pos), layerMask, endDist, radius))
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
			while (dbg > 0)
			{
				dbg--;
				cache::removeDraw("dbg" + std::to_string(dbg));
			}
			while (dbg4 > 0)
			{
				dbg4--;
				cache::removeDraw("dbg4" + std::to_string(dbg4));
			}
			while (dbg3 > 0)
			{
				dbg3--;
				cache::removeDraw("dbg3" + std::to_string(dbg3));
			}

			v3 nodePos = currentNode->pos;
			const float segmentTheta = (float)(M_PI * 2) / rayCount;
			const float segmentDistance = sinf(segmentTheta / 2) * 2;
			constexpr float segmentThetaVertical = (float)(M_PI * 2) / 24;

			for (int i = 0; i < rayCount; i++)
			{
				for (int vertical = 0; vertical < 5; vertical++)
				{
					float pitch = -(segmentThetaVertical * 2) + segmentThetaVertical * vertical;
					auto yaw = (segmentTheta * i) + yawFix;

					v3 dir = v3(
						cosf(yaw) * cosf(pitch),
						sinf(pitch),
						sinf(yaw) * cosf(pitch)
					);

					bool foundValidPos = false;
					int subDivisions = 3;
					for (int subDiv = 0; subDiv < subDivisions; subDiv++)
					{

						float subStepMod = 1.f - ((1.f / (subDivisions)) * subDiv);
						if (subStepMod == 0.f) std::cout << "[bad] subStepMod == 0\n";

						const float moddedStepLen = stepLength * subStepMod;
						const auto moddedStep = dir * moddedStepLen;
						auto finalPos = nodePos + moddedStep;

						bool jump = false;
						bool duck = false;
						bool fall = false;

						//cache::debugDraw("dbg3" + std::to_string(dbg3), cache::debugArrow3d(currentNode->pos, dir, "#eb7a34", moddedStepLen)); dbg3 += 1;
						//cache::debugDraw("dbg" + std::to_string(dbg), cache::debugCube(Lapis::Transform(finalPos, 0, 0.04f), "ff000099")); dbg += 1;

						bool overrideClosedCheck = false;
						RaycastHit hitInfo;
						if (CapsuleCast(nodePos, capsuleHeight, radius, dir, moddedStepLen, &hitInfo, isInitialNode)
							|| UnityEngine::Physics::Linecast(nodePos, nodePos + moddedStep, layerMask))
						{
							if (subDiv != 0)
								continue;
							if (hitInfo.m_Normal.y > 0.8f)
								continue;

							float distToGround = 0.f;
							RaycastHit distToGroundHit;
							if (UnityEngine::Physics::Linecast(nodePos, nodePos - v3(0, -1, 0), distToGroundHit, layerMask))
								distToGround = distToGroundHit.m_Distance - 0.01f;


							//check duck first since its less computations
							if (pitch <= 0)
							{
								if (!CapsuleCast(nodePos, capsuleCrouchHeight - distToGround, radius, dir, moddedStepLen, nullptr, isInitialNode)
									&& !UnityEngine::Physics::CheckCapsule(finalPos, finalPos + capsuleTopOffset, radius, layerMask)) //capsuleTopCrouchOffset to make multiple crouch nodes possible
								{
									//if (pitch > 0) //1.1f * segmentThetaVertical)
									//	continue;
									duck = true;
								}
							}

							if (!duck && pitch != 0)
								continue;
							
							const float jumpHeight = capsuleHeight -0.2f - distToGround;
							if (!duck
								&& !CapsuleCast(nodePos, capsuleHeight, radius, {0,1,0}, jumpHeight, nullptr, isInitialNode))
								//up check ^^
							{
								//direction check VV
								RaycastHit jumpForwardHitInfo;
								bool hit = CapsuleCast(nodePos + Vector3(0, jumpHeight, 0), capsuleHeight, radius, dir, stepLength, &jumpForwardHitInfo, isInitialNode);			
								
								const float jumpForwardDist = hit ? jumpForwardHitInfo.m_Distance - 0.01f : stepLength;
								if (jumpForwardDist < 0.1f)
									continue;

								const auto jumpFinalPos = (nodePos + Vector3(0, jumpHeight, 0) + dir * jumpForwardDist);

								finalPos = jumpFinalPos;
								jump = true;
								overrideClosedCheck = true;
							}

							if (!duck && !jump)
							{
								if (hitInfo.m_Distance > 0.11f)
								{
									if (UnityEngine::Physics::CheckCapsule(nodePos + dir * (hitInfo.m_Distance - 0.1f), nodePos + dir * (hitInfo.m_Distance - 0.1f) + capsuleTopOffset, radius, layerMask))
										continue;
									finalPos = nodePos + dir * (hitInfo.m_Distance - 0.1f);
									overrideClosedCheck = true;
								}
								else
									continue;
							}
						}
						//cache::debugDraw("dbg" + std::to_string(dbg - 1), cache::debugCube(Lapis::Transform(finalPos, 0, 0.05f), "#fc520399"));


						std::shared_ptr<PathNode> nearbyClosedNode = nullptr;
						if (IsClosedNode(finalPos, overrideClosedCheck ? 0.15f : 1.1f, &nearbyClosedNode))
						{
							if (currentNode->parent != nullptr && nearbyClosedNode->G < currentNode->parent->G)
							{
								float dist = v3::Distance(currentNode->pos, nearbyClosedNode->pos);
								if (!CapsuleCast(currentNode->pos, capsuleHeight, radius, v3::Normalize(nearbyClosedNode->pos - currentNode->pos), dist, nullptr, isInitialNode))
								{
									currentNode->parent = nearbyClosedNode;
									currentNode->depth = nearbyClosedNode->depth + 1;
									currentNode->G = nearbyClosedNode->G + v3::Distance(currentNode->pos, nearbyClosedNode->pos);
								}
							}
							continue;
						}

						//cache::debugDraw("dbg" + std::to_string(dbg - 1), cache::debugCube(Lapis::Transform(finalPos, 0, 0.05f), "#fcd70399"));


						RaycastHit groundedHit;
						if (Grounded(finalPos, jump ? groundedHeight * 1.25f : groundedHeight, groundedHit))
						{
							if (groundedHit.m_Point.y < -0.8f)
								continue;

							if (finalPos.y > currentNode->pos.y)
							{
								if (jump)
								{
									if (groundedHit.m_Normal.y < 0.05f)
										continue;
								}
								else
								{
									if (groundedHit.m_Normal.y < 0.4f)
										continue;
								}

							}
							else
							{
								if (groundedHit.m_Normal.y < 0.08f)
									continue;
							}
						}
						else
						{
							if (jump)
								continue;
							RaycastHit FallHit;
							if (!Grounded(finalPos, maxFallHeight, FallHit))
								continue;
							if (FallHit.m_Normal.y < 0.4 || FallHit.m_Point.y < -0.8)
								continue;
							if (CapsuleCast(finalPos, capsuleHeight, radius, { 0,-1,0 }, FallHit.m_Distance - 0.01f, nullptr, isInitialNode))
								continue;
							finalPos = finalPos + Vector3(0, -(FallHit.m_Distance - 0.02f), 0);
							fall = true;
							if (IsClosedNode(finalPos))
								continue;
						}

						//cache::debugDraw("dbg" + std::to_string(dbg - 1), cache::debugCube(Lapis::Transform(finalPos, 0, 0.05f), "#bafc0399"));

						std::shared_ptr<PathNode> nearbyOpenNode = nullptr;
						if (!IsOpenNode(finalPos, segmentDistance, &nearbyOpenNode))
						{
							auto newNode = std::make_shared<PathNode>(std::to_string(idCounter), finalPos, goal, currentNode, weightH, jump, duck, fall);
							openNodes.Add(newNode);
							idCounter++;
							//cache::debugDraw("dbg" + std::to_string(dbg - 1), cache::debugCube(Lapis::Transform(finalPos, 0, 0.05f), fall ? "0000ff99" : jump ? "00ffff99" : duck ? "ff00ff99" : "00ff0099"));
							foundValidPos = true;
							break; // break subStep
						}
					}
					if (foundValidPos)
						break; // break verticals
				}
			}

			isInitialNode = false;
			closedNodePartitioner->Add(currentNode);
			currentNode = nullptr;
			todo = findBestOpenNode;

			if (idCounter > maxNodeCount)
			{
				std::cout << "RustBot ; Reached maximum node count [/]\n";
				if (allowResortUponMaxNodes)
				{
					if (bestFoundNode != nullptr && bestFoundNode->parent != nullptr)
					{
						std::cout << "RustBot ; Resorting best found path [+]\n";
						currentNode = bestFoundNode;
						todo = backtracing;
						break;
					}
				}
				todo = invalid;
			}
			break;
		}
		case backtracing:
		{
			if (currentNode != nullptr)
			{
				foundPath.push_back(currentNode);
				currentNode = currentNode->parent;
				UpdateRender();
				break;
			}
			else
				todo = completed;
		}
		case completed:
		{
			std::cout << "RustBot ; Path Complete [+++]\n";
			UpdateRender();
			return false;
			break;
		}
		case invalid:
		{
			std::cout << "RustBot ; Couldn't Find Path [XXX]\n";
			UpdateRenderPath("000000");
			return false;
		}
		}
		// end of todo switch

		if (currentNode != nullptr)
			UpdateRenderPath("0000aa");

		UpdateRender();
		return true;
	}
#pragma warning(pop)
		
	
#pragma warning(pop)

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


	std::vector<v3> Pather::CurrentPath()
	{
		std::vector<v3> points = {};
		while (currentNode != nullptr)
		{
			points.push_back(currentNode->pos);
			currentNode = currentNode->parent;
		}
		return points;
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
		//points.push_back(this->goalPos);
		return true;
	}

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

	bool Pather::CapsuleCast(Vector3 base, float height, float radius, Vector3 direction, float maxDistance, RaycastHit* hitInfo, bool firstPass)
	{
		RaycastHit unused;
		bool didHit = UnityEngine::Physics::CapsuleCast(base, base + Vector3(0,height - radius*2,0), firstPass ? radius - 0.01f : radius, direction, (hitInfo == nullptr) ? unused : *hitInfo, maxDistance, layerMask);
		if (firstPass && !didHit)
		{
			UnityEngine::Physics::CheckCapsule(base + Vector3(0, height / 2 - radius, 0), base + Vector3(0, height / 2 + radius, 0), radius, layerMask);
		}
		return didHit;
	}
}
