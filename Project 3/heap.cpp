#include "heap.h"
#include "hash.h"
#include <vector>
#include <iostream>

using namespace std;

heap::heap(int capacity)
{
    this->capacity = capacity;
    this->data = vector<heap ::node>(capacity + 1);
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
    this->data[hole].id = id;
    this->data[hole].key = key;
    this->data[hole].pData = pv;

    this->map.insert(id, &data[hole]);

    percolateUp(hole);
    return 0;
}

void heap::percolateUp(int hole)
{
    void * pData = this->data[hole].pData;
    int key = this->data[hole].key;
    std::string id = this->data[hole].id;

    for (; key < this->data[hole / 2].key; hole /= 2)
    {
        if (hole == 1)
        {
            break;
        }

        this->data[hole] = this->data[hole / 2];
        this->data[hole].index = hole;
        this->map.setPointer(this->data[hole].id, &this->data[hole]);
    }

    this->data[hole].key = key;
    this->data[hole].id = id;
    this->data[hole].index = hole;
    this->data[hole].pData = pData;
    this->map.setPointer(this->data[hole].id, &this->data[hole]);
}

int heap::deleteMin(std::string *pId, int *pKey, void *ppData)
{
    if (this->currentSize == 0)
    {
        return 1;
    }

    if (pId != nullptr)
    {
        *pId = this->data[1].id;
    }
    if (pKey != nullptr)
    {
        *pKey = this->data[1].key;
    }
    if (ppData != nullptr)
    {
        *(static_cast<void **> (ppData)) = data[1].pData;
    }

    this->map.remove(this->data[1].id);
    this->data[1] = this->data[this->currentSize--];

    percolateDown(1);
    return 0;
}

void heap::percolateDown(int hole)
{
    void * pData = this->data[hole].pData;
    heap::node tmp = this->data[hole];
    int child;

    for (; hole * 2 <= this->currentSize; hole = child)
    {
        child = hole * 2;

        if (child != this->currentSize && this->data[child + 1].key < this->data[child].key)
        {
            child++; // pick smaller child
        }

        if (this->data[child].key < tmp.key)
        {
            this->data[hole] = this->data[child];
            this->data[hole].index = hole;
            this->map.setPointer(this->data[hole].id, &this->data[hole]);
        }
        else
        {
            break;
        }
    }

    this->data[hole] = tmp;
    this->data[hole].index = hole;
    this->data[hole].pData = pData;
    this->map.setPointer(this->data[hole].id, &this->data[hole]);
}

int heap::setKey(const std::string &id, int key)
{
    if (!this->map.contains(id))
    {
        return 1;
    }

    heap::node *n = (heap::node *)this->map.getPointer(id);

    int key_old = n->key;
    n->key = key;

    if (key_old > key)
    {
        percolateUp(n->index);
    }
    else if (key_old < key)
    {
        percolateDown(n->index);
    }
    return 0;
}

int heap::remove(const std::string &id, int *pKey, void *ppData)
{
    if (!this->map.contains(id))
    {
        return 1;
    }

    heap::node *n = (heap::node *)this->map.getPointer(id);

    if (pKey != nullptr)
    {
        *pKey = n->key;
    }
    if (ppData != nullptr)
    {
        ppData = n->pData;
    }

    this->setKey(id, this->data[1].key - 1);
    int d = this->deleteMin();
    return 0;
}

// print contents of heap
void heap::print()
{
    for (int i = 1; i <= this->currentSize; i++)
    {
        heap::node *n = (heap::node *)this->map.getPointer(this->data[i].id);
        cout << this->data[i].key << this->data[i].id << " " << n->id << " " << endl;
    }
}

void heap::checkAlignment()
{
    for (int i = 1; i <= this->currentSize; i++)
    {
        heap::node *n = (heap::node *)this->map.getPointer(this->data[i].id);
        if (this->data[i].key != n->key)
        {
            cout << "Alignment error" << endl;
            exit(1);
        }
    }
}

void * heap::getPointer(const string &name, bool * found){
    return this->map.getPointer(name, found);
} 

int heap::getKey(const string &name, bool * found){
    node *n = (node *)this->map.getPointer(name, found);
    return * found ? n-> key : -1;
}

int heap::getCurrentSize(){
    return this->currentSize;
}