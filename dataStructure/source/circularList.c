/*
 *OpenTeachProject
 *Copyright (C) 2021  OpenTeachProject <openteachproject@gmail.com>
 *
 *This program is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



/*
 * circularList.c
 *
 *  Created on: Jun 11, 2021
 *      Author: salar
 */



#include <stdlib.h>
#include <dataStructure/include/circularList.h>



circularListNode_t *circularListCreateNewNode(void *element, uint32_t key) {
    circularListNode_t *newNode = (circularListNode_t*)malloc(sizeof(circularListNode_t));
    if (newNode == NULL) {
        return NULL;
    }
    newNode -> element = element;
    newNode -> key = key;
    newNode -> previousNode = NULL;
    newNode -> nextNode = NULL;
    return newNode;
}

circularList_t *circularListCreateNewList(void){
    circularList_t *newList = (circularList_t*)malloc(sizeof(circularList_t) );
    if (newList == NULL) {
        return NULL;
    }
    newList -> size = 0;
    newList -> firstNode = NULL;
    newList -> lastNode = NULL;
    return newList;
}

void circularListInsertFirst(circularList_t *list, circularListNode_t *x) {
    circularListNode_t *firstNode;
    circularListNode_t *lastNode;
    if (list -> size == 0) {
        x -> nextNode = x;
        x -> previousNode = x;
        list -> firstNode = x;
        list -> lastNode = x;
    } else {
        firstNode = list -> firstNode;
        lastNode = list -> lastNode;
        x -> nextNode = firstNode;
        x -> previousNode = lastNode;
        lastNode -> nextNode = x;
        firstNode -> previousNode = x;
        list -> firstNode = x;
    }
    list -> size++;
}

void circularListInsertLast(circularList_t *list, circularListNode_t *x) {
    circularListNode_t *firstNode;
    circularListNode_t *lastNode;
    if (list -> size == 0) {
        x -> nextNode = x;
        x -> previousNode = x;
        list -> firstNode = x;
        list -> lastNode = x;
    }
    else {
        firstNode = list -> firstNode;
        lastNode = list -> lastNode;
        x -> nextNode = firstNode;
        x -> previousNode = lastNode;
        lastNode -> nextNode = x;
        firstNode -> previousNode = x;
        list -> lastNode = x;
    }
    list -> size++;
}

void circularListInsertAfter(circularList_t *list, circularListNode_t *x, circularListNode_t *y) {
    if (y == NULL){
        return;
    }
    x -> nextNode = y -> nextNode;
    x -> previousNode = y;
    y -> nextNode -> previousNode = x;
    y -> nextNode = x;
    if (list -> lastNode == y) {
        list -> lastNode = x;
    }
    list -> size++;
}

void circularListInsertBefore(circularList_t *list, circularListNode_t *x, circularListNode_t *y) {
    if (y == NULL){
        return;
    }
    x -> nextNode = y;
    x -> previousNode = y -> previousNode;
    y -> previousNode -> nextNode = x;
    y -> previousNode = x;
    if (list -> firstNode == y) {
        list -> firstNode = x;
    }
    list -> size++;
}

circularListNode_t *circularListDeleteFirst(circularList_t *list) {
    circularListNode_t *firstNode;
    circularListNode_t *lastNode;
    if (list -> size == 0) {
        return NULL;
    }
    firstNode = list -> firstNode;
    lastNode = list -> lastNode;
    if (list -> size == 1) {
        list -> firstNode = NULL;
        list -> lastNode = NULL;
    }
    else {
        lastNode -> nextNode = firstNode -> nextNode;
        firstNode -> nextNode -> previousNode = lastNode;
        list -> firstNode = firstNode -> nextNode;
    }
    list -> size--;
    return firstNode;
}

circularListNode_t *circularListDeleteLast(circularList_t *list) {
    circularListNode_t *firstNode;
    circularListNode_t *lastNode;
    if (list -> size == 0) {
        return NULL;
    }
    firstNode = list -> firstNode;
    lastNode = list -> lastNode;
    if (list -> size == 1) {
        list -> firstNode = NULL;
        list -> lastNode = NULL;
    }
    else {
        firstNode -> previousNode = lastNode -> previousNode;
        lastNode -> previousNode -> nextNode = firstNode;
        list -> lastNode = lastNode -> previousNode;
    }
    list -> size--;
    return lastNode;
}

circularListNode_t *circularListDeleteAfter(circularList_t *list, circularListNode_t *x) {
    circularListNode_t *afterNode;
    if (list -> size == 0 || x == NULL || x -> nextNode == NULL) {
        return NULL;
    }
    afterNode = x -> nextNode;
    afterNode -> nextNode -> previousNode = x;
    x -> nextNode = afterNode -> nextNode;
    if (list -> lastNode == afterNode) {
        list -> lastNode = x;
    }
    list -> size--;
    return afterNode;
}

circularListNode_t *circularListDeleteBefore(circularList_t *list, circularListNode_t *x) {
    circularListNode_t *beforNode;
    if (list -> size == 0 || x == NULL || x -> previousNode == NULL) {
        return NULL;
    }
    beforNode = x -> previousNode;
    beforNode -> previousNode -> nextNode = x;
    x -> previousNode = beforNode -> previousNode;
    if (list -> firstNode == beforNode) {
        list -> firstNode = x;
    }
    list -> size--;
    return beforNode;
}

void circularListDelete(circularList_t *list, circularListNode_t *x) {
    circularListNode_t *firstNode;
    circularListNode_t *lastNode;
    if (list -> size == 0 || x == NULL) {
        return;
    }
    firstNode = list -> firstNode;
    lastNode = list -> lastNode;
    if (list -> size == 1) {
        list -> firstNode = NULL;
        list -> lastNode = NULL;
    }
    else {
        if (x == firstNode) {
            list -> firstNode = x -> nextNode;
        }
        else if (x == lastNode) {
            list -> lastNode = x -> previousNode;
        }
        x -> previousNode -> nextNode = x -> nextNode;
        x -> nextNode -> previousNode = x -> previousNode;
    }
    list -> size--;
}

void circularListPushIndex(circularList_t *list) {
    if (list -> size == 0) {
        return;
    }
    list -> lastNode = list -> firstNode;
    list -> firstNode = list -> firstNode -> nextNode;
}

circularListNode_t *cirularListSearchByElement(circularList_t *list, void* element) {
    circularListNode_t *x;
    if (list -> size == 0) {
        return NULL;
    }
    x = list -> firstNode -> nextNode;
    while (x != NULL && x -> element != element){
        x = x -> nextNode;
    }
    return x;
}

circularListNode_t *cirularListSearchByKey(circularList_t *list, uint32_t key) {
    circularListNode_t *x;
    if (list -> size == 0) {
        return NULL;
    }
    x = list -> firstNode -> nextNode;
    while (x != NULL && x -> key != key){
        x = x -> nextNode;
    }
    return x;
}

uint32_t circularListGetSize(circularList_t *list) {
    uint32_t size;
    if (list == NULL) {
        size = 0;
    }
    else {
        size = list -> size;
    }
    return size;
}
