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
 * linkedList.h
 *
 *  Created on: Jun 16, 2021
 *      Author: salar
 */



#ifndef DATASTRUCTURE_INCLUDE_LINKEDLIST_H_
#define DATASTRUCTURE_INCLUDE_LINKEDLIST_H_



#include <stdint.h>



typedef struct linkedListNode{
    void                               *element;
    uint32_t                            key;
    struct linkedListNode              *previousNode;
    struct linkedListNode              *nextNode;
}linkedListNode_t;

typedef struct linkedList{
    struct linkedListNode              *nilNode;
    uint32_t                            size;
}linkedList_t;



linkedListNode_t                       *linkedListCreateNewNode(void *element, uint32_t key);
linkedList_t                           *linkedListCreateNewList(void);
void                                    linkedListInsertFirst(linkedList_t *list, linkedListNode_t *x);
void                                    linkedListInsertLast(linkedList_t *list, linkedListNode_t *x);
linkedListNode_t                       *linkedListDeleteFirst(linkedList_t *list);
linkedListNode_t                       *linkedListDeleteLast(linkedList_t *list);
void                                    linkedListDelete(linkedList_t *list, linkedListNode_t *x);
linkedListNode_t                       *linkedListSearchByElement(linkedList_t *list, void* element);
linkedListNode_t                       *linkedListSearchByKey(linkedList_t *list, uint32_t key);
uint32_t                                linkedListGetSize(linkedList_t *list);



#endif /* DATASTRUCTURE_INCLUDE_LINKEDLIST_H_ */
