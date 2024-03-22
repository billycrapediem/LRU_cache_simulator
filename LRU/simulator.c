#include "simulator.h"
#include "stdlib.h"
int missCount ;
cache* globalCache;

LRUCache* createCache(long long capacity){
    LRUCache* newLRU = (LRUCache*) malloc(sizeof (LRUCache));
    newLRU->head = NULL;
    newLRU->tail = NULL;
    newLRU->capacity = capacity;
    newLRU->size = 0;
    return newLRU;
}

// initialize simulation
void sim_start(int B, int S, int W) {
    missCount = 0;
    globalCache = (cache*) malloc(sizeof (cache));
    globalCache->B = B;
    globalCache->S = S;
    globalCache->W = W;
    long long setSize = 1 << S;
    long long lineSize = 1 << W;
    globalCache->sets = (LRUCache**) calloc(setSize, sizeof (LRUCache*));
    for( long long i = 0; i < setSize; i ++){
        globalCache->sets[i] = createCache(lineSize);
    }
    long long indexBit = 1 << S;
    indexBit -=1;
    globalCache->indexBit = indexBit;

}


void sim_access(long long acc) {
    long long index = (acc >> globalCache->B) & (globalCache->indexBit);
    long long tag = acc >> (globalCache->B + globalCache->S);
    LRUCache* LRU = globalCache->sets[index];
    Node* traverseNode = (Node*) LRU->head;
    Node* hitNode = NULL;
    while(traverseNode){
        long long curTag = traverseNode->tag;
        if(curTag == tag){
            hitNode = traverseNode;
            break;
        }
        traverseNode = (Node*) traverseNode->next;
    }
    if (hitNode) {
        //cache hit
        if(hitNode != LRU->head){
            Node* tmpNode = LRU->head;
            Node* prevNode = (Node *) hitNode->prev;
            hitNode->prev = NULL;
            if(hitNode == LRU->tail){
                LRU->tail = prevNode;
                prevNode->next = NULL;
            }
            else{
                Node* nextNode = (Node*) hitNode->next;
                prevNode->next = (struct Node *) nextNode;
                nextNode->prev = (struct Node *) prevNode;
            }
            hitNode->next = (struct Node *) tmpNode;
            LRU->head = hitNode;
            tmpNode->prev = (struct Node *) hitNode;
        }
    }
    else {
        // Miss: Choose replacement and update LRU
        missCount ++;
        if (LRU->size >= LRU->capacity) { // No empty line, find LRU
            Node* curr = LRU->tail;
            curr->tag = tag;
            Node* prev = (Node *) curr->prev;
            curr->prev = NULL;
            prev->next = NULL;
            LRU->tail = prev;
            Node* tmp = LRU->head;
            curr->next = (struct Node *) tmp;
            tmp->prev = (struct Node *) curr;
            LRU->head = curr;
        }
        else{
            Node* curr = (Node*) malloc(sizeof (Node));
            curr->tag = tag;
            LRU->size ++;
            // deal with no one elements in cache before
            if(!LRU->tail){
                LRU->tail = curr;
            }
            else{
                Node* tmp = LRU->head;
                curr->next = (struct Node *) tmp;
                tmp->prev = (struct Node *) curr;
            }
            LRU->head = curr;
        }
    }
}

// return the number of misses
int sim_finish(void) {
    int rnt = missCount;
    missCount = 0;
    long long setSize = 1<< globalCache->S;
    for( long long i = 0; i < setSize; i ++){
        LRUCache* LRU = globalCache->sets[i];
        Node* tmpNode = LRU->head;
        while(tmpNode){
            Node* nextNode = (Node*) tmpNode->next;
            free(tmpNode);
            tmpNode = nextNode;
        }
        free(LRU);
    }
    free(globalCache->sets);
    free(globalCache);
    return rnt;
}
