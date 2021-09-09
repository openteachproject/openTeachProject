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
 * circularQueue.h
 *
 *  Created on: Jun 21, 2021
 *      Author: salar
 */



#ifndef DATASTRUCTURE_INCLUDE_CIRCULARQUEUE_H_
#define DATASTRUCTURE_INCLUDE_CIRCULARQUEUE_H_



#include <dataStructure/include/circularList.h>



typedef circularListNode_t                                  circularQueueNode_t;
typedef circularList_t                                      circularQueue_t;



#define CircularQueueCreateNewNode(Element, Key)            circularListCreateNewNode(Element, Key)
#define CircularQueueCreateNewQueue()                       circularListCreateNewList()
#define CircularQueueEnQueue(Queue, Node)                   circularListInsertLast(Queue, Node)
#define CircularQueueDeQueue(Queue)                         circularListDeleteFirst(Queue)
#define CircularQueueDeleteNode(Queue, Node)                circularListDelete(Queue, Node)
#define CircularQueuePushIndex(Queue)                       circularListPushIndex(Queue)
#define CircularQueueSearchByElement(Queue, Element)        cirularListSearchByElement(Queue, Element)
#define CircularQueueSearchByKey(Queue, Key)                cirularListSearchByKey(Queue, Key)
#define CircularQueueGetSize(Queue)                         circularListGetSize(Queue)
#define CircularQueueHead(Queue)                            Queue -> firstNode
#define CircularQueueGetNextNode(Node)                      Node -> nextNode




#endif /* DATASTRUCTURE_INCLUDE_CIRCULARQUEUE_H_ */
