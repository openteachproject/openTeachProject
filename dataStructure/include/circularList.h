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
 * circularList.h
 *
 *  Created on: Jun 11, 2021
 *      Author: salar
 */



#ifndef DATASTRUCTURE_INCLUDE_CIRCULARLIST_H_
#define DATASTRUCTURE_INCLUDE_CIRCULARLIST_H_



#include <stdint.h>



typedef struct circularListNode{
    void                               *element;
    uint32_t                            key;
    struct circularListNode            *previousNode;
    struct circularListNode            *nextNode;
}circularListNode_t;

typedef struct circularList{
    uint32_t                            size;
    circularListNode_t                 *firstNode;
    circularListNode_t                 *lastNode;
}circularList_t;



circularListNode_t                     *circularListCreateNewNode(void *element, uint32_t key);
circularList_t                         *circularListCreateNewList(void);
void                                    circularListInsertFirst(circularList_t *list, circularListNode_t *x);
void                                    circularListInsertLast(circularList_t *list, circularListNode_t *x);
void                                    circularListInsertAfter(circularList_t *list, circularListNode_t *x, circularListNode_t *y);
void                                    circularListInsertBefore(circularList_t *list, circularListNode_t *x, circularListNode_t *y);
circularListNode_t                     *circularListDeleteFirst(circularList_t *list);
circularListNode_t                     *circularListDeleteLast(circularList_t *list);
circularListNode_t                     *circularListDeleteAfter(circularList_t *list, circularListNode_t *x);
circularListNode_t                     *circularListDeleteBefore(circularList_t *list, circularListNode_t *x);
void                                    circularListDelete(circularList_t *list, circularListNode_t *x);
void                                    circularListPushIndex(circularList_t *list);
circularListNode_t                     *cirularListSearchByElement(circularList_t *list, void* element);
circularListNode_t                     *cirularListSearchByKey(circularList_t *list, uint32_t key);
uint32_t                                circularListGetSize(circularList_t *list);



#endif /* DATASTRUCTURE_INCLUDE_CIRCULARLIST_H_ */
