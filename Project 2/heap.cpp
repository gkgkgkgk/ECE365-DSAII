
#include "heap.h"
#include "hash.h"
#include <vector>

using namespace std;

heap::heap(int capacity)
{
    this->capacity = capacity;
    this->data = vector<heap ::node>(capacity);
    this->map = hashTable(capacity);
}

int heap::insert(const std::string &id, int key, void *pv)
{
    if (this->capacity == this->currentSize)
    {
        return 1;
    }

    if (this->map.contains(id))
    {
        return 2;
    }

    int hole = ++this->currentSize;
    for (; hole > 1 && key < this->data[hole / 2].key; hole /= 2)
    {
        this->data[hole] = this->data[hole / 2];
    }

    data[hole] = heap::node(id, key, nullptr);

    return 0;
}