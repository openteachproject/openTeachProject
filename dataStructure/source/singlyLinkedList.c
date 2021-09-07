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
 * singlyLinkedList.c
 *
 *  Created on: Jun 16, 2021
 *      Author: salar
 */



#include <stdlib.h>
#include <dataStructure/include/singlyLinkedList.h>



singlyLinkedListNode_t *singlyLinkedListCreateNewNode(void *element, uint32_t key) {
    singlyLinkedListNode_t *newNode = (singlyLinkedListNode_t*)malloc(sizeof(singlyLinkedListNode_t));
    if (newNode ==NULL) {
        return NULL;
    }
    newNode -> element = element;
    newNode -> key = key;
    newNode -> nextNode = NULL;
    return newNode;
}

singlyLinkedList_t *singlyLinkedListCreateNewList(void) {
    singlyLinkedList_t *newList = (singlyLinkedList_t*)malloc(sizeof(singlyLinkedList_t));
    if (newList == NULL) {
        return NULL;
    }
    singlyLinkedListNode_t *nilNode = (singlyLinkedListNode_t*)malloc(sizeof(singlyLinkedListNode_t));
    if (nilNode == NULL){
        free(newList);
        return NULL;
    }
    newList -> nilNode = nilNode;
    newList -> size = 0;
    nilNode -> element = NULL;
    nilNode -> key = 0;
    nilNode -> nextNode = nilNode;
    return newList;
}

void singlyLinkedListInsertFirst(singlyLinkedList_t *list, singlyLinkedListNode_t *x) {
    x -> nextNode = list -> nilNode -> nextNode;
    list -> nilNode -> nextNode = x;
    list -> size++;
}

singlyLinkedListNode_t *singlyLinkedListDeleteFirst(singlyLinkedList_t *list) {
    singlyLinkedListNode_t *x = list -> nilNode -> nextNode;
    list -> nilNode -> nextNode = list -> nilNode -> nextNode -> nextNode;
    list -> size--;
    return x;
}

void singlyLinkedListInsertAfter(singlyLinkedList_t *list, singlyLinkedListNode_t *x, singlyLinkedListNode_t *y) {
    x -> nextNode = y -> nextNode;
    y -> nextNode = x;
    list -> size++;
}

singlyLinkedListNode_t *singlyLinkedListDeleteAfter(singlyLinkedList_t *list, singlyLinkedListNode_t *x) {
    singlyLinkedListNode_t *y = x -> nextNode;
    x -> nextNode = x -> nextNode -> nextNode;
    list -> size--;
    return y;
}

singlyLinkedListNode_t *singlyLinkedListSearchByElement(singlyLinkedList_t *list, void* element) {
    singlyLinkedListNode_t *x = list -> nilNode -> nextNode;
    while (x != list -> nilNode && x -> element != element) {
        x = x -> nextNode;
    }
    return x;
}

singlyLinkedListNode_t *singlyLinkedListSearchByKey(singlyLinkedList_t *list, uint32_t key) {
    singlyLinkedListNode_t *x = list -> nilNode -> nextNode;
    while (x != list -> nilNode && x -> key != key){
        x = x -> nextNode;
    }
    return x;
}

uint32_t singlyLinkedListGetSize(singlyLinkedList_t *list) {
    return list -> size;
}
