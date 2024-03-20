//
// Created by Bryan Zhang on 3/18/24.
//

#include "simulator.h"
#include <stdlib.h>
#include <stdio.h>



int missCount ;
cache* globalCache;

LRUCache* createCache(void){
    LRUCache* newLRU = (LRUCache*) malloc(sizeof (LRUCache));
    newLRU->head = NULL;
    newLRU->tail = NULL;
    return newLRU;
}
void sim_start(int B, int S, int W) {
    missCount = 0;
    globalCache = (cache*) malloc(sizeof (cache));
    globalCache->B = B;
    globalCache->S = S;
    globalCache->W = W;
    long long setSize = 1 << S;
    long long lineSize = 1 << W;
    globalCache->sets = (cacheSet**) calloc(setSize, sizeof (cacheSet*));
    for( long long i = 0; i < setSize; i ++){
        cacheSet* newSet = (cacheSet*) malloc(sizeof (cacheSet));
        newSet->lines = (cacheLine **) calloc(lineSize,sizeof (cacheLine*));
        newSet->LRU = createCache();
        for( long long j = 0; j < lineSize; j ++){
            cacheLine* newLine = (cacheLine*) malloc(sizeof (cacheLine ));
            newLine->valid = 0;
            newSet->lines[j] = newLine;
        }
        globalCache->sets[i] = newSet;
    }
    long long indexBit = 1 << S;
    indexBit -=1;
    globalCache->indexBit = indexBit;

}


void sim_access(long long acc) {
    long long index = (acc >> globalCache->B) & (globalCache->indexBit);
    long long tag = acc >> (globalCache->B + globalCache->S);
    cacheSet* set = globalCache->sets[index];
    LRUCache* LRU = set->LRU;
    long long hitIndex = -1, replacementIndex = -1, lineSize = 1 << globalCache->W;
    for (long long i = 0; i < lineSize; i++) {
        if (!set->lines[i]->valid && replacementIndex == -1) {
            replacementIndex =  i;
        }
        if (set->lines[i]->valid && set->lines[i]->tag == tag) {
            hitIndex = i;
            break;
        }
    }

    if (hitIndex != -1) {
        Node* curNode = set->lines[hitIndex]->pos;
        //cache hit
        if(curNode != LRU->head){
            Node* tmpNode = LRU->head;
            Node* prevNode = (Node *) curNode->prev;
            curNode->prev = NULL;
            if(curNode == LRU->tail){
                LRU->tail = prevNode;
                prevNode->next = NULL;
            }
            else{
                Node* nextNode = (Node*) curNode->next;
                prevNode->next = (struct Node *) nextNode;
                nextNode->prev = (struct Node *) prevNode;
            }
            curNode->next = (struct Node *) tmpNode;
            LRU->head = curNode;
            tmpNode->prev = (struct Node *) curNode;
        }
    }
    else {
        // Miss: Choose replacement and update LRU
        missCount ++;
        if (replacementIndex == -1) { // No empty line, find LRU
            replacementIndex = LRU->tail->pos;
            Node* curr = LRU->tail;
            Node* prev = (Node *) curr->prev;
            curr->prev = NULL;
            prev->next = NULL;
            LRU->tail = prev;
            Node* tmp = LRU->head;
            curr->next = (struct Node *) tmp;
            tmp->prev = (struct Node *) curr;
            LRU->head = curr;
            set->lines[replacementIndex]->tag = tag;
        }
        else{
            set->lines[replacementIndex]->tag = tag;
            set->lines[replacementIndex]->valid = 1;
            Node* curr = (Node*) malloc(sizeof (Node));
            set->lines[replacementIndex]->pos = curr;
            curr->pos = replacementIndex;
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
    long long lineSize = 1<< globalCache->W;
    for( long long i = 0; i < setSize; i ++){
        cacheSet* tmpSet = globalCache->sets[i];
        for( long long j = 0; j < lineSize; j ++){
            cacheLine* tmpLine = tmpSet->lines[j];
            free(tmpLine);
        }
        LRUCache* LRU = tmpSet->LRU;
        free(LRU);
        free(tmpSet->lines);
        free(tmpSet);
    }
    free(globalCache->sets);
    free(globalCache);
    return rnt;
}

