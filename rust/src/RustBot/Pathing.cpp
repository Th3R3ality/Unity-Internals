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
	void Pather::New(v3 startPos, v3 goalPos, GoalType goalType)
	{
		if (idCounter == 0)
			start = startPos;
	
		goal = Goal(goalType, goalPos);
		yawFix = atan2f(goalPos.z - start.z, goalPos.x - start.x);

		if (idCounter == 0)
		{
			openNodes.Add(std::make_shared<PathNode>(std::to_string(idCounter), start, goal, nullptr, weightH));
			idCounter++;
		}

		isInitialNode = true;
		todo = PatherAction::findBestOpenNode;
	}

	void Pather::Clean()
	{
		idCounter = 0;
		currentNode = nullptr;
		bestFoundNode = nullptr;

		openNodes.items.clear();
		closedNodePartitioner->Clear();
		foundPath.clear();

		todo = PatherAction::idle;
	}

#pragma warning(push)
#pragma warning(disable: 26819) // switch fallthrough warning
	bool Pather::Step(bool processSameStep)
	{
		switch (todo)
		{
		case PatherAction::findBestOpenNode:
		{
			if ((currentNode = openNodes.RemoveFirst()) == nullptr)
			{
				std::cout << "RustBot [Pathing] ; Exhausted all open nodes [-]\n";
				if (allowResortUponExhaust)
				{
					if (bestFoundNode != nullptr && bestFoundNode->parent != nullptr)
					{
						std::cout << "RustBot [Pathing] ; Resorting best found path [+]\n";
						currentNode = bestFoundNode;
						todo = PatherAction::backtracing;
						break;
					}
				}

				todo = PatherAction::invalid;
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
				todo = PatherAction::backtracing;
				break;
			}

			//closedNodePartitioner->Add(currentNode);
			todo = PatherAction::processFoundNode;
			if (!processSameStep)
				break;
		}
		case PatherAction::processFoundNode:
		{
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

						const float moddedStepLen = stepLength * subStepMod;
						const auto moddedStep = dir * moddedStepLen;
						auto finalPos = nodePos + moddedStep;

						bool jump = false;
						bool duck = false;
						bool fall = false;

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

							//duck less computations but jump more likely
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

						std::shared_ptr<PathNode> nearbyOpenNode = nullptr;
						if (!IsOpenNode(finalPos, segmentDistance, &nearbyOpenNode))
						{
							auto newNode = std::make_shared<PathNode>(std::to_string(idCounter), finalPos, goal, currentNode, weightH, jump, duck, fall);
							openNodes.Add(newNode);
							idCounter++;
							
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
			todo = PatherAction::findBestOpenNode;

			if (idCounter > maxNodeCount)
			{
				std::cout << "RustBot [Pathing] ; Reached maximum node count [/]\n";
				if (allowResortUponMaxNodes)
				{
					if (bestFoundNode != nullptr && bestFoundNode->parent != nullptr)
					{
						std::cout << "RustBot [Pathing] ; Resorting best found path [+]\n";
						currentNode = bestFoundNode;
						todo = PatherAction::backtracing;
						break;
					}
				}
				todo = PatherAction::invalid;
			}
			break;
		}
		case PatherAction::backtracing:
		{
			if (currentNode != nullptr)
			{
				foundPath.push_back(currentNode);
				currentNode = currentNode->parent;
				//UpdateRender();
				break;
			}
			else
				todo = PatherAction::completed;
		}
		case PatherAction::completed:
		{
			std::cout << "RustBot [Pathing] ; Path Complete [+++]\n";
			return false;
			break;
		}
		case PatherAction::invalid:
		{
			std::cout << "RustBot [Pathing] ; Couldn't Find Path [XXX]\n";
			return false;
		}
		}
		// end of todo switch

		return true;
	}
#pragma warning(pop)

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

	std::vector<v3> Pather::GetCurrentPath()
	{
		std::vector<v3> points = {};
		if (todo == PatherAction::completed)
		{
			for (auto node : foundPath)
			{
				points.push_back(node->pos);
			}
		}
		while (currentNode != nullptr)
		{
			points.push_back(currentNode->pos);
			currentNode = currentNode->parent;
		}
		return points;
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
