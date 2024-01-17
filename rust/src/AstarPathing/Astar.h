#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>

#include "UnityEngine/Vector3/Vector3.hpp"
#include "UnityEngine/RaycastHit/RaycastHit.hpp"
#include "cache.hpp"

#include "Node.h"
#include "NodeHeap.h"

using namespace UnityEngine;

namespace Astar
{
	enum nextAction
	{
		findBestOpenNode,
		processFoundNode,
		backtracing,
		completed,
		invalid
	};

	using v3 = UnityEngine::Vector3;

	class AstarPath
	{
	public:
		v3 start, end;

		NodeHeap openNodes;
		//std::vector<std::shared_ptr<Node>> openNodes;
		std::vector<std::shared_ptr<Node>> closedNodes;
		std::vector<std::shared_ptr<Node>> foundPath;

		std::shared_ptr<Node> currentNode = nullptr;
		nextAction todo = findBestOpenNode;
		float yawFix = 0;

		unsigned int idCounter = 0;

		
		// settings
		float stepLength = 1;
		float flightCheckHeight = 1;
		int rayCount = 6;
		float weightH = 5;
		bool disableVertical = false;
		bool allowFlight = false;
		bool preferFlight = false;
		unsigned int maxNodeCount = 2000;
		unsigned int maxPathDepth = 0;
		float radius = 0;
		int layerMask = -5;

		/// <summary>
		/// Constructor With Pather Settings
		/// </summary>
		/// <param name="stepLength"> - meters from node to node (horizontally)</param>
		/// <param name="radius"> - if above 2 this will be the radius of the spherecast</param>
		/// <param name="layerMask"> - what layers to check collisions on</param>
		/// <param name="maxPathDepth"> - max amount of nodes away from start (if reached path is set to complete)</param>
		/// <param name="allowFlight"> - allow the path to step on air</param>
		/// <param name="preferFlight"> - path will prefer staying or going up in the air</param>
		/// <param name="disableVertical"> - disables the paths ability to go vertical</param>
		/// <param name="flightCheckHeight"> - distance check to see if the path is grounded</param>
		/// <param name="rayCount"> - how many raycasts each node should send (horizontal count, vertical rays multiply this by 3)</param>
		/// <param name="weightH"> - weight that gets multiplied on the H value (distance from start), higher means more commitment to a path</param>
		/// <param name="maxNodeCount"> - the max amount of nodes allowed to spawn</param>
		AstarPath(
			float stepLength = 1, float radius = 0, int layerMask = -5,
			unsigned int maxPathDepth = 0, bool allowFlight = false,
			bool preferFlight = false, bool disableVertical = false, float flightCheckHeight = 1,
			int rayCount = 6, float weightH = 5, unsigned int maxNodeCount = 2000) :
			stepLength(stepLength), radius(radius), layerMask(layerMask),
			maxPathDepth(maxPathDepth),	allowFlight(allowFlight),
			preferFlight(preferFlight), disableVertical(disableVertical), flightCheckHeight(flightCheckHeight),
			rayCount(rayCount),	weightH(weightH), maxNodeCount(maxNodeCount)
		{}

		void New(v3 start, v3 end);
		bool Step(bool processSameStep = false);
		bool IsClosedNode(v3 nodePos, float leniency = 1.f, std::shared_ptr<Node>* nearbyClosedNode = nullptr);
		bool IsOpenNode(v3 nodePos, float leniency = 1.f, std::shared_ptr<Node>* nearbyOpenNode = nullptr);
		
		bool GrabPath(std::vector<v3>& points);

		void UpdateRenderPath(std::string hexCol, bool onlyRemove = false);
		void UpdateRender();
	};

	bool Raycast(v3 from, v3 dir, float maxDist, int layerMask);
}