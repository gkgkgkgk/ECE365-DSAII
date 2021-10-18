#include "heap.h"
#include "hash.h"
#include <vector>
#include <iostream>

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
    this->data[hole].id = id;
    this->data[hole].key = key;

    percolateUp(hole);

    this->map.insert(id, &data[hole]);

    return 0;
}

void heap::percolateUp(int hole){
    int key = this->data[hole].key;
    std::string id = this->data[hole].id;

    while (key < this->data[hole / 2].key)
    {
        if(hole == 1){
            break;
        }

        this->data[hole] = this->data[hole / 2];
        this->data[hole].index = hole;
        this->map.setPointer(this->data[hole].id, &this->data[hole]);
        hole /= 2;
    }

    this->data[hole].key = key;
    this->data[hole].id = id;
    this->data[hole].key = key;
    this->data[hole].index = hole;
    this->map.setPointer(this->data[hole].id, &this->data[hole]);
}

int heap::deleteMin(std::string *pId, int *pKey, void *ppData){
    if(this->currentSize == 0){
        return 1;
    }

    if(pId != nullptr){
        *pId = this->data[1].id;
    }
    if(pKey != nullptr){
        *pKey = this->data[1].key;
    }
    if(ppData != nullptr){
        ppData = this->data[1].pData;
    }

    this->map.remove(this->data[1].id);
    this->data[1] = this->data[this->currentSize--];

    percolateDown(1);

    return 0;
}

void heap::percolateDown(int hole){
    heap::node tmp = this->data[hole];
    int child;

    for(; hole * 2 <= this->currentSize; hole = child){
        child = hole * 2;

        if(child != this->currentSize && this->data[child + 1].key < this->data[child].key){
            child++; // pick smaller child
        }

        if(this->data[child].key < tmp.key){
            this->data[hole] = this->data[child];
            this->data[hole].index = hole;
            this->map.setPointer(this->data[hole].id, &this->data[hole]);
        }
        else {
            break;
        }
    }

    this->data[hole] = tmp;
    this->data[hole].index = hole;
    this->map.setPointer(this->data[hole].id, &this->data[hole]);
}

int heap::setKey(const std::string &id, int key){
    if(!this->map.contains(id)){
        return 1;
    }

    bool exists = false;
    heap::node *n = (heap::node*)this->map.getPointer(id, &exists);

    if(!exists){
        return 1;
    }

    int key_old = n->key;
    n->key = key;

    if(key_old > key){
        percolateUp(n->index);
    }
    else if(key_old < key){
        percolateDown(n->index);
    }

    return 0;
}


int heap::remove(const std::string &id, int *pKey, void *ppData){
    if(!this->map.contains(id)){
        return 1;
    }
    
    bool exists = false;
    heap::node *n = (heap::node*)this->map.getPointer(id, &exists);

    if(!exists){
        return 1;
    }

    if(pKey != nullptr){
        *pKey = n->key;
    }
    if(ppData != nullptr){
        ppData = n->pData;
    }

    this->setKey(id, this->data[1].key - 1);
    
    return this-> deleteMin();
}

// print contents of heap
void heap::print()
{
    for (int i = 1; i <= this->currentSize; i++)
    {
        cout << this->data[i].id << " " << this->data[i].key << endl;
    }    
}
