#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>

#include "UnityEngine/Vector3/Vector3.hpp"
#include "UnityEngine/RaycastHit/RaycastHit.hpp"
#include "cache.hpp"

#include "Alias.h"
#include "Goal.h"
#include "PathNode.h"
#include "NodeHeap.h"
#include "SpacePartitioner.h"

using namespace UnityEngine;

namespace RustBot
{

	enum nextAction
	{
		findBestOpenNode,
		processFoundNode,
		backtracing,
		completed,
		invalid
	};


	class Pather
	{
	public:
		// settings
		float stepLength = 1;
		float groundedHeight = 0.5f;
		float maxFallHeight = 5;
		int rayCount = 6;
		float weightH = 5;
		unsigned int maxNodeCount = 5000;
		unsigned int maxPathDepth = 0;
		float radius = 0;
		int layerMask = -5;
		int debugLevel = 0;
		bool disableVertical = false;
		bool distanceCheckY = true;
		float capsuleHeight = 0;
		float capsuleCrouchHeight = 1.1f;
		v3 capsuleTopOffset;
		v3 capsuleCrouchTopOffset;

		bool allowResortUponExhaust = false;
		bool allowResortUponMaxNodes = false;

		Goal goal = Goal(GoalType::none, 0);

		/// <summary>
		/// Constructor With Pather Settings
		/// </summary>
		/// <param name="stepLength"> - meters from node to node (horizontally)</param>
		/// <param name="radius"> - the radius of the spherecast (0 == revert to raycast)</param>
		/// <param name="capsuleHeight"> - height offset to the center of the top sphere from the bottom sphere (0 == revert to sphercast)</param>
		/// <param name="layerMask"> - what layers to check collisions on</param>
		/// <param name="distanceCheckY"> - whether or not to account for y in distance checks (useful for goto: x,z)</param>
		/// <param name="maxPathDepth"> - max amount of nodes away from start (if reached path is set to complete)</param>
		/// <param name="groundedHeight"> - max height the path can go above the ground before being marked inAir</param>
		/// <param name="maxFallHeight"> - max height the path is allowed to go down if in air</param>
		/// <param name="rayCount"> - how many raycasts each node should send (horizontal count, vertical rays multiply this by 3)</param>
		/// <param name="weightH"> - weight that gets multiplied on the H value (distance from start), higher means more commitment to a path</param>
		/// <param name="maxNodeCount"> - the max amount of nodes allowed to spawn</param>
		/// <param name="debugLevel"> - level of debug drawing, 0 = none, 1 = simple, 2 = verbose</param>
		/// <param name="disableVertical"> - disables the paths ability to go vertical</param>
		Pather(
			float stepLength = 1,
			float radius = 0,
			float capsuleHeight = 0,
			int layerMask = -5,
			bool distanceCheckY = true,
			unsigned int maxPathDepth = 0,
			float groundedHeight = 1,
			float maxFallHeight = 5,
			int rayCount = 8,
			float weightH = 5,
			unsigned int maxNodeCount = 5000,
			int debugLevel = 0,
			bool disableVertical = false) :
			stepLength(stepLength),
			radius(radius),
			capsuleHeight(capsuleHeight),
			layerMask(layerMask),
			distanceCheckY(distanceCheckY),
			maxPathDepth(maxPathDepth),
			groundedHeight(groundedHeight),
			maxFallHeight(maxFallHeight),
			rayCount(rayCount),
			weightH(weightH),
			maxNodeCount(maxNodeCount),
			debugLevel(debugLevel),
			disableVertical(disableVertical)
		{
			closedNodePartitioner = std::make_unique<SpacePartitioner>(this->stepLength);
			capsuleTopOffset = v3(0, capsuleHeight - (radius * 2 < capsuleHeight ? radius * 2 : 0), 0);
			capsuleCrouchTopOffset = v3(0, 1.1f - (radius * 2 < capsuleCrouchHeight ? radius * 2 : 0), 0);
		}

		void New(v3 startPos, v3 endPos, GoalType goalType);
		bool Step(bool processSameStep = false);
		bool IsClosedNode(v3 nodePos, float leniency = 1.f, std::shared_ptr<PathNode>* nearbyClosedNode = nullptr);
		bool IsOpenNode(v3 nodePos, float leniency = 1.f, std::shared_ptr<PathNode>* nearbyOpenNode = nullptr);
		

		std::vector<v3> CurrentPath();
		bool GrabPath(std::vector<v3>& points);

		void UpdateRenderPath(std::string hexCol, bool onlyRemove = false);
		void UpdateRender();

	private:

		v3 start;

		NodeHeap openNodes;
		//std::vector<std::shared_ptr<PathNode>> openNodes;

		std::unique_ptr<SpacePartitioner> closedNodePartitioner = nullptr;
		//std::vector<std::shared_ptr<PathNode>> closedNodes;

		std::vector<std::shared_ptr<PathNode>> foundPath;

		std::shared_ptr<PathNode> bestFoundNode = nullptr;
		std::shared_ptr<PathNode> currentNode = nullptr;
		nextAction todo = findBestOpenNode;
		float yawFix = 0;

		unsigned int idCounter = 0;
		bool isInitialNode = false;

		bool Grounded(v3 pos, float length, RaycastHit& hitInfo);
		bool CapsuleCast(Vector3 base, float height, float radius, Vector3 direction, float maxDistance, RaycastHit* hitInfo = nullptr, bool firstPass = false);
	};
}