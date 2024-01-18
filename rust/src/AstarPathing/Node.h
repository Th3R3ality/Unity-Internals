#pragma once
#include "UnityEngine/Vector3/Vector3.hpp"
#include <iostream>
#include <string>
#include <format>

namespace Astar
{
	using v3 = UnityEngine::Vector3;

	class Node
	{
	private:
		int heapIndex = 0;
		float weightH = 1;

	public:
		std::string id;
		v3 pos{};
		std::shared_ptr<Node> parent;
		unsigned int depth = 0;
		bool inAir = false;
		float G = 0, H = 0;
		union
		{
			bool closed = false;
			bool unreachable;
		};

		float F()
		{
			return H + G;
		}

		Node(std::string id, v3 nodePos, v3 startPos, v3 endPos, std::shared_ptr<Node> parent, float weightH) :
			id(id), pos(nodePos), parent(parent), weightH(weightH)
		{
			if (parent != nullptr)
			{
				G = parent->G + v3::Distance(nodePos, parent->pos);
				depth = parent->depth + 1;
			}
			H = weightH * v3::Distance(nodePos, endPos);
		}

		int GetHeapIndex()
		{
			return heapIndex;
		}

		void SetHeapIndex(int index)
		{
			heapIndex = index;
		}

		float CompareTo(std::shared_ptr<Node> node)
		{
			float compare = node->F() - F();

			if (compare == 0)
				compare = node->H - H;

			return compare;
		}

		bool Equals(std::shared_ptr<Node> node)
		{
			return (this->pos == node->pos);
		}
	};
}
