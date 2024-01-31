#pragma once
#include <iostream>
#include <string>
#include <format>

#include "Alias.h"
#include "Goal.h"

namespace RustBot
{
	class PathNode
	{
	private:
		int heapIndex = 0;
		float weightH = 1;

	public:
		std::string id;
		v3 pos{};
		bool jump = false;
		bool duck = false;
		bool fall = false;
		std::shared_ptr<PathNode> parent;
		unsigned int depth = 0;
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

		PathNode(std::string id, v3 nodePos, Goal goal, std::shared_ptr<PathNode> parent, float weightH, bool jump = false, bool duck = false, bool fall = false) :
			id(id), pos(nodePos), parent(parent), weightH(weightH), jump(jump), duck(duck), fall(fall)
		{
			if (parent != nullptr)
			{
				G = parent->G + v3::Distance(nodePos, parent->pos);
				depth = parent->depth + 1;
			}
			H = weightH * goal.Heuristic(nodePos);
		}

		int GetHeapIndex()
		{
			return heapIndex;
		}

		void SetHeapIndex(int index)
		{
			heapIndex = index;
		}

		float CompareTo(std::shared_ptr<PathNode> node)
		{
			float compare = node->F() - F();

			if (compare == 0)
				compare = node->H - H;

			return compare;
		}

		bool Equals(std::shared_ptr<PathNode> node)
		{
			return (this->pos == node->pos);
		}
	};
}
