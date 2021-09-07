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
 * singlyLinkedList.h
 *
 *  Created on: Jun 16, 2021
 *      Author: salar
 */



#ifndef DATASTRUCTURE_INCLUDE_SINGLYLINKEDLIST_H_
#define DATASTRUCTURE_INCLUDE_SINGLYLINKEDLIST_H_



#include <stdint.h>



typedef struct singlyLinkedListNode{
    void                               *element;
    uint32_t                            key;
    struct singlyLinkedListNode        *nextNode;
}singlyLinkedListNode_t;

typedef struct singlyLinkedList{
    singlyLinkedListNode_t             *nilNode;
    uint32_t                            size;
}singlyLinkedList_t;



singlyLinkedListNode_t                 *singlyLinkedListCreateNewNode(void *element, uint32_t key);
singlyLinkedList_t                     *singlyLinkedListCreateNewList(void);
void                                    singlyLinkedListInsertFirst(singlyLinkedList_t *list, singlyLinkedListNode_t *x);
singlyLinkedListNode_t                 *singlyLinkedListDeleteFirst(singlyLinkedList_t *list);
void                                    singlyLinkedListInsertAfter(singlyLinkedList_t *list, singlyLinkedListNode_t *x, singlyLinkedListNode_t *y);
singlyLinkedListNode_t                 *singlyLinkedListDeleteAfter(singlyLinkedList_t *list, singlyLinkedListNode_t *x);
singlyLinkedListNode_t                 *singlyLinkedListSearchByElement(singlyLinkedList_t *list, void* element);
singlyLinkedListNode_t                 *singlyLinkedListSearchByKey(singlyLinkedList_t *list, uint32_t key);
uint32_t                                singlyLinkedListGetSize(singlyLinkedList_t *list);



#endif /* DATASTRUCTURE_INCLUDE_SINGLYLINKEDLIST_H_ */
