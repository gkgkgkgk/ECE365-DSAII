#include "hash.h"
#include <iostream>

using namespace std;

hashTable::hashTable(int size)
{
    this->capacity = hashTable::getPrime(size);
    this->data = vector<hashTable::hashItem>(this->capacity);
    this->filled = 0;
}

// Insert the specified key into the hash table.
// If an optional pointer is provided,
// associate that pointer with the key.
// Returns 0 on success,
// 1 if key already exists in hash table,
// 2 if rehash fails.
int hashTable::insert(const std::string &key, void *pv)
{
    if (this->capacity / 2 < this->filled)
    {
        if (!this->rehash())
            return 2;
    }

    int index = this->hash(key);

    while (this->data[index].isOccupied && !this->data[index].isDeleted)
    {
        if (this->data[index].key == key)
        {
            return 2;
        }

        if (index >= this->capacity - 1)
        {
            index = 0;
        }
        else
        {
            index++;
        }
    }

    this->data[index].key = key;
    this->data[index].isOccupied = true;
    this->data[index].pv = pv;

    this->filled++;

    return 0;
}

bool hashTable::contains(const std::string &key)
{
    return (this->findPos(key) != -1);
}

void *hashTable::getPointer(const std::string &key, bool *b)
{
    int pos = this->findPos(key);
    if (b != nullptr)
        *b = (pos != -1);
    return pos == -1 ? nullptr : this->data[pos].pv;
}

int hashTable::setPointer(const std::string &key, void *pv)
{
    int pos = this->findPos(key);

    if (pos == -1)
        return 1;

    this->data[pos].pv = pv;

    return 0;
}

bool hashTable::remove(const std::string &key)
{
    int pos = this->findPos(key);

    if (pos == -1)
        return false;

    this->data[pos].isDeleted = true;
    return true;
}

int hashTable::hash(const std::string &key)
{
    unsigned int hash = 0;

    for (char c : key)
    {
        hash = hash * 59 + (int)c;
    }

    return hash % this->capacity;
}

// Search for an item with the specified key.
// Return the position if found, -1 otherwise.
int hashTable::findPos(const std::string &key)
{
    int index = this->hash(key);

    while (this->data[index].isOccupied || !this->data[index].isDeleted)
    {
        if (this->data[index].key == key && !this->data[index].isDeleted)
        {
            return index;
        }
        if (!this->data[index].isOccupied)
        {
            return -1;
        }

        if (index >= this->capacity - 1)
        {
            index = 0;
        }
        else
        {
            index++;
        }
    }

    return -1;
}

// The rehash function; makes the hash table bigger.
// Returns true on success, false if memory allocation fails.
bool hashTable::rehash()
{
    int n = hashTable::getPrime(this->capacity * 2);
    if (n < 0)
    {
        return false;
    }
    this->capacity = n;
    vector<hashTable::hashItem> originalData = data;
    this->data = vector<hashTable::hashItem>(this->capacity);
    this->filled = 0;
    for (auto hi : originalData)
    {
        if (hi.isOccupied && !hi.isDeleted)
            this->insert(hi.key, hi.pv);
    }

    return true;
}

unsigned int hashTable::getPrime(int size)
{
    const unsigned int primeNumbers[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741, 3221225473};
    for (int prime : primeNumbers)
    {
        if (size < prime)
        {
            return prime;
        }
    }

    throw std::runtime_error(std::string("Specified size too large."));
}