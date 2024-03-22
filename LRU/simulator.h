//
// Created by Bryan Zhang on 3/18/24.
//

#ifndef CACHE_SIMULATOR_SIMULATOR_H
#define CACHE_SIMULATOR_SIMULATOR_H
typedef struct{
    struct Node* prev;
    struct Node* next;
    long long tag;
} Node;

typedef struct LRUCache {
    Node* head;
    Node* tail;
    long long capacity;
    long long size;
} LRUCache;

typedef struct{
    LRUCache** sets;
    int B;
    int S;
    int W;
    long long indexBit;
} cache;
LRUCache* createCache(long long capacity);

void sim_start(int B, int S, int W);
void sim_access(long long acc);
int sim_finish(void);
#endif //CACHE_SIMULATOR_SIMULATOR_H
