#pragma once
#include <vector>
#include <unordered_map>
#include <string>

#include "UnityEngine/Vector3/Vector3.hpp"
#include "cache.hpp"

#include "Node.h"
#include "NodeHeap.h"

namespace Astar
{
	enum nextAction
	{
		findBestOpenNode,
		processFoundNode,
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
		
		AstarPath(float stepLength = 1, bool disableVertical = false, bool allowFlight = false, bool preferFlight = false, float flightCheckHeight = 1,
			int rayCount = 6, float weightH = 5, unsigned int maxNodeCount = 2000) :
			stepLength(stepLength), disableVertical(disableVertical), allowFlight(allowFlight),preferFlight(preferFlight),
			rayCount(rayCount),weightH(weightH),maxNodeCount(maxNodeCount)
		{}

		void New(v3 start, v3 end);
		bool Step();
		bool IsClosedNode(v3 nodePos, float leniency = 1.f, std::shared_ptr<Node>* nearbyClosedNode = nullptr);
		bool IsOpenNode(v3 nodePos, float leniency = 1.f, std::shared_ptr<Node>* nearbyOpenNode = nullptr);
		
		void UpdateRenderPath(std::string hexCol, bool onlyRemove = false);
		void UpdateRender();
	};

	bool Raycast(v3 from, v3 dir, float maxDist);
}