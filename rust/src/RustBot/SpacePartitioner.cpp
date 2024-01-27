#include "SpacePartitioner.h"
#include <iostream>

namespace RustBot
{
    void Partition::AddItem(std::shared_ptr<PathNode> item)
    {
        items.push_back(item);
    }

    void SpacePartitioner::Clear()
    {
        partitions.clear();
    }

    void SpacePartitioner::Add(std::shared_ptr<PathNode> item)
    {
        int xCoord = static_cast<int>(item->pos.x / unitSize);
        int zCoord = static_cast<int>(item->pos.z / unitSize);
        
        auto it = partitions.find(std::pair(xCoord, zCoord));
        if ( it != partitions.end())
        {
            it->second->AddItem(item);
        }
        else
        {
            auto it = partitions.insert(std::pair( std::pair(xCoord, zCoord), std::make_shared<Partition>() ));
            it.first->second->AddItem(item);
        }
    }

    std::vector<std::shared_ptr<Partition>> SpacePartitioner::GetNearbyPartitions(v_3 location)
    {
        std::vector<std::shared_ptr<Partition>> outPartitions = {};

        int xCoord = static_cast<int>(location.x / unitSize) - 1;
        int zCoord = static_cast<int>(location.z / unitSize) - 1;

        for (int x = 0; x < 3; x++)
        {
            for (int z = 0; z < 3; z++)
            {
                auto it = partitions.find(std::pair(xCoord + x, zCoord + z));
                if ( it != partitions.end())
                {
                    outPartitions.push_back(it->second);
                }
            }
        }
        return outPartitions;
    }
    std::vector<std::shared_ptr<Partition>> SpacePartitioner::GetAllPartitions()
    {
        std::vector<std::shared_ptr<Partition>> outPartitions;

        for (auto partition : partitions)
        {
            outPartitions.push_back(partition.second);
        }

        return outPartitions;
    }
}
