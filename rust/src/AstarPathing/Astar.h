#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <iostream>

#include "UnityEngine/Vector3/Vector3.hpp"
#include "UnityEngine/RaycastHit/RaycastHit.hpp"
#include "cache.hpp"

#include "Node.h"
#include "NodeHeap.h"
#include "SpacePartitioner.h"

using namespace UnityEngine;

namespace Astar
{
	using v3 = UnityEngine::Vector3;

	enum nextAction
	{
		findBestOpenNode,
		processFoundNode,
		backtracing,
		completed,
		invalid
	};


	class AstarPath
	{
	public:
		v3 start, end;

		NodeHeap openNodes;
		//std::vector<std::shared_ptr<Node>> openNodes;

		std::unique_ptr<SpacePartitioner> closedNodePartitioner = nullptr;
		//std::vector<std::shared_ptr<Node>> closedNodes;

		std::vector<std::shared_ptr<Node>> foundPath;

		std::shared_ptr<Node> bestFoundNode = nullptr;
		std::shared_ptr<Node> currentNode = nullptr;
		nextAction todo = findBestOpenNode;
		float yawFix = 0;

		unsigned int idCounter = 0;

		
		// settings
		float stepLength = 1;
		float inAirHeight = 1;
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
		v3 capsuleTopOffset;

		/// <summary>
		/// Constructor With Pather Settings
		/// </summary>
		/// <param name="stepLength"> - meters from node to node (horizontally)</param>
		/// <param name="radius"> - the radius of the spherecast (0 == revert to raycast)</param>
		/// <param name="capsuleHeight"> - height offset to the center of the top sphere from the bottom sphere (0 == revert to sphercast)</param>
		/// <param name="layerMask"> - what layers to check collisions on</param>
		/// <param name="distanceCheckY"> - whether or not to account for y in distance checks (useful for goto: x,z)</param>
		/// <param name="maxPathDepth"> - max amount of nodes away from start (if reached path is set to complete)</param>
		/// <param name="inAirHeight"> - max height the path can go above the ground before being marked inAir</param>
		/// <param name="maxFallHeight"> - max height the path is allowed to go down if in air</param>
		/// <param name="rayCount"> - how many raycasts each node should send (horizontal count, vertical rays multiply this by 3)</param>
		/// <param name="weightH"> - weight that gets multiplied on the H value (distance from start), higher means more commitment to a path</param>
		/// <param name="maxNodeCount"> - the max amount of nodes allowed to spawn</param>
		/// <param name="debugLevel"> - level of debug drawing, 0 = none, 1 = simple, 2 = verbose</param>
		/// <param name="disableVertical"> - disables the paths ability to go vertical</param>
		AstarPath(
			float stepLength = 1,
			float radius = 0,
			float capsuleHeight = 0,
			int layerMask = -5,
			bool distanceCheckY = true,
			unsigned int maxPathDepth = 0,
			float inAirHeight = 1,
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
			inAirHeight(inAirHeight),
			maxFallHeight(maxFallHeight),
			rayCount(rayCount),
			weightH(weightH),
			maxNodeCount(maxNodeCount),
			debugLevel(debugLevel),
			disableVertical(disableVertical)
		{
			closedNodePartitioner = std::make_unique<SpacePartitioner>(this->stepLength);
			capsuleTopOffset = v3(0, capsuleHeight - (radius * 2 < capsuleHeight ? radius * 2 : 0), 0);
		}

		void New(v3 start, v3 end);
		bool Step(bool processSameStep = false);
		bool IsClosedNode(v3 nodePos, float leniency = 1.f, std::shared_ptr<Node>* nearbyClosedNode = nullptr);
		bool IsOpenNode(v3 nodePos, float leniency = 1.f, std::shared_ptr<Node>* nearbyOpenNode = nullptr);
		
		bool GrabPath(std::vector<v3>& points);

		void UpdateRenderPath(std::string hexCol, bool onlyRemove = false);
		void UpdateRender();
	};
}