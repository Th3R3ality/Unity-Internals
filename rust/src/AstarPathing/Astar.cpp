#include "Astar.h"
#include "Lapis/engine/LapisEngine.h"

Astar::AstarPath path;

namespace Astar
{
	void New(v3 start, v3 end)
	{
		path.start.pos = start;
		path.end.pos = end;

		cache::debugDraw("pathStart", cache::debugIcosahedron({ start, Lapis::deltaTime * 20,0.2 }, "00ff00bb"));
		cache::debugDraw("pathEnd", cache::debugIcosahedron({end, Lapis::deltaTime * 20, 0.2}, "ff0000bb"));
	}

	void Step()
	{
		static bool checkingNode = false;
		static Node currentNode;

		if (!checkingNode)
			currentNode.GenerateChildren();
	}

	void Node::GenerateChildren()
	{

	}
}
