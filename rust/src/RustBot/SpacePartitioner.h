#pragma once
#include <unordered_map>
#include <memory>
#include <vector>

#include "PathNode.h"
#include "Lapis/engine/LapisTypes.h"
#include "UnityEngine/Vector3/Vector3.hpp"

namespace RustBot
{
	using Color = Lapis::Color;
	using v_3 = UnityEngine::Vector3;

	struct pair_hash
	{
		template <class T1, class T2>
		std::size_t operator () (const std::pair<T1, T2>& p) const
		{
			auto h1 = std::hash<T1>{}(p.first);
			auto h2 = std::hash<T2>{}(p.second);

			// Mainly for demonstration purposes, i.e. works but is overly simple
			// In the real world, use sth. like boost.hash_combine
			return h1 ^ h2;
		}
	};

	class Partition
	{
	public:
		Color color;
		std::vector<std::shared_ptr<Node>> items{};

		Partition()
		{
			color = Color::Random(0.5);
		}

		void AddItem(std::shared_ptr<Node> item);
	};

	class SpacePartitioner
	{
	public:
		SpacePartitioner(float unitSize = 1)
		{
			this->unitSize = unitSize;
		}

		void Clear();
		void Add(std::shared_ptr<Node> item);
		std::vector<std::shared_ptr<Partition>> GetNearbyPartitions(v_3 location);
		std::vector<std::shared_ptr<Partition>> GetAllPartitions();

	private:
		float unitSize = 1;
		std::unordered_map<std::pair<int,int>, std::shared_ptr<Partition>, pair_hash> partitions = {};
	};
}