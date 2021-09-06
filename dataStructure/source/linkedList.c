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
 * linkedList.c
 *
 *  Created on: Jun 16, 2021
 *      Author: salar
 */



#include <stdlib.h>
#include <dataStructure/include/linkedList.h>



linkedListNode_t *linkedListCreateNewNode(void *element, uint32_t key) {
    linkedListNode_t *newNode = (linkedListNode_t*)malloc(sizeof(linkedListNode_t));
    if (newNode == NULL) {
        return NULL;
    }
    newNode -> element = element;
    newNode -> key = key;
    newNode -> previousNode = NULL;
    newNode -> nextNode = NULL;
    return newNode;
}

linkedList_t *linkedListCreateNewList(void) {
    linkedList_t *newList = (linkedList_t*)malloc(sizeof(linkedList_t));
    if (newList == NULL) {
        return NULL;
    }
    linkedListNode_t *nilNode = (linkedListNode_t*)malloc(sizeof(linkedListNode_t));
    if (nilNode == NULL) {
        free(newList);
        return NULL;
    }
    newList -> nilNode = nilNode;
    newList-> size = 0;
    nilNode -> element = NULL;
    nilNode -> key = 0;
    nilNode -> nextNode = nilNode;
    nilNode -> previousNode = nilNode;
    return newList;
}

void linkedListInsertFirst(linkedList_t *list, linkedListNode_t *x) {
    x -> nextNode = list -> nilNode -> nextNode;
    list -> nilNode -> nextNode -> previousNode = x;
    list -> nilNode -> nextNode = x;
    x -> previousNode = list -> nilNode;
    list -> size++;
}

void linkedListInsertLast(linkedList_t *list, linkedListNode_t *x) {
    x -> previousNode = list -> nilNode -> previousNode;
    list -> nilNode -> previousNode -> nextNode = x;
    list -> nilNode -> previousNode = x;
    x -> nextNode = list -> nilNode;
    (list -> size)++;
}

linkedListNode_t *linkedListDeleteFirst(linkedList_t *list) {
    linkedListNode_t *x = list -> nilNode -> nextNode;
    list -> nilNode -> nextNode -> nextNode -> previousNode = list -> nilNode;
    list -> nilNode -> nextNode = list -> nilNode -> nextNode -> nextNode;
    list -> size--;
    return x;
}

linkedListNode_t *linkedListDeleteLast(linkedList_t *list) {
    linkedListNode_t *x = list -> nilNode -> previousNode;
    list -> nilNode -> previousNode -> previousNode -> nextNode = list -> nilNode;
    list -> nilNode -> previousNode = list -> nilNode -> previousNode -> previousNode;
    list -> size--;
    return x;
}

void linkedListDelete(linkedList_t *list, linkedListNode_t *x) {
    x -> previousNode -> nextNode = x -> nextNode;
    x -> nextNode -> previousNode = x -> previousNode;
    list -> size--;
}

linkedListNode_t *linkedListSearchByElement(linkedList_t *list, void* element) {
    linkedListNode_t *x = list -> nilNode -> nextNode;
    while (x != list -> nilNode && x -> element != element) {
        x = x -> nextNode;
    }
    return x;
}

linkedListNode_t *linkedListSearchByKey(linkedList_t *list, uint32_t key) {
    linkedListNode_t *x = list -> nilNode -> nextNode;
    while (x != list -> nilNode && x -> key != key){
        x = x -> nextNode;
    }
    return x;
}

uint32_t linkedListGetSize(linkedList_t *list) {
    return list -> size;
}
