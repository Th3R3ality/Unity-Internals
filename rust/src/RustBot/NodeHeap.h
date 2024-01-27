#include <iostream>
#include <vector>
#include <algorithm>
#include <concepts>

#include "PathNode.h"

//#define func std::cout << __func__ << "\n"

namespace RustBot
{

    class NodeHeap
    {
    public:
        std::vector<std::shared_ptr<PathNode>> items;

        NodeHeap() = default;

        void Add(std::shared_ptr<PathNode> item)
        {
            items.push_back(item);
            item->SetHeapIndex(items.size() - 1);
            SortUp(item);
        }

        std::shared_ptr<PathNode> RemoveFirst()
        {
            if (items.size() == 0)
                return nullptr;

            auto firstItem = items[0];
            if (items.size() > 1)
            {
                items[0] = items[items.size() - 1];
                items[0]->SetHeapIndex(0);
                items.pop_back();
                SortDown(items[0]);
            }
            else
                items.pop_back();

            return firstItem;
        }

        void UpdateItem(std::shared_ptr<PathNode> item)
        {
            SortUp(item);
        }

        int Count()
        {
            return items.size();
        }

        bool Contains(std::shared_ptr<PathNode> item)
        {
            return items[item->GetHeapIndex()]->Equals(item);
        }

        void SortDown(std::shared_ptr<PathNode> item)
        {
            while (true)
            {
                const int childIndexLeft = item->GetHeapIndex() * 2 + 1;
                const int childIndexRight = item->GetHeapIndex() * 2 + 2;
                
                if (childIndexLeft < items.size())
                {
                    int swapIndex = childIndexLeft;

                    if (childIndexRight < items.size())
                    {
                        if (items[childIndexLeft]->CompareTo(items[childIndexRight]) < 0) // under 0 means arg has lower value, i.e. better;
                            swapIndex = childIndexRight;
                    }

                    if (item->CompareTo(items[swapIndex]) < 0) // under 0 means arg has lower value, i.e. better;
                    {
                        Swap(item, items[swapIndex]);
                    }
                    else
                        return;
                }
                else
                    return;
            }
        }

        void SortUp(std::shared_ptr<PathNode> item)
        {
            int parentIndex = (item->GetHeapIndex() - 1) / 2;
            if (parentIndex < 0 || item->GetHeapIndex() == 0)
                return;
            
            while (true)
            {
                if (GetAsyncKeyState(VK_DELETE))
                    break;

                std::shared_ptr<PathNode> parentItem = items.at(parentIndex);
                if (item->CompareTo(parentItem) > 0) // over 0 means arg has higher value, i.e. parent is worse than item;
                {
                    Swap(item, parentItem);
                }
                else
                    break;

                parentIndex = (item->GetHeapIndex() - 1) / 2;
            }
        }

        void Swap(std::shared_ptr<PathNode> itemA, std::shared_ptr<PathNode> itemB)
        {
            items[itemA->GetHeapIndex()] = itemB;
            items[itemB->GetHeapIndex()] = itemA;

            int temp = itemA->GetHeapIndex();
            itemA->SetHeapIndex(itemB->GetHeapIndex());
            itemB->SetHeapIndex(temp);
        }
    };

}