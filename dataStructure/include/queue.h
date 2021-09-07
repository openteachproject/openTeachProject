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
 * queue.h
 *
 *  Created on: Jun 16, 2021
 *      Author: salar
 */



#ifndef DATASTRUCTURE_INCLUDE_QUEUE_H_
#define DATASTRUCTURE_INCLUDE_QUEUE_H_



#include <dataStructure/include/linkedList.h>



typedef linkedList_t                            queue_t;
typedef linkedListNode_t                        queueNode_t;



#define QueueCreateNewNode(Element, Key)        linkedListCreateNewNode(Element, Key)
#define QueueCreateNewQueue()                   linkedListCreateNewList()
#define QueueEnqueue(Queue, Node)               linkedListInsertLast(Queue, Node)
#define QueueDeQueue(Queue)                     linkedListDeleteFirst(Queue)
#define QueueDeleteNode(Queue, Node)            linkedListDelete(Queue, Node)
#define QueueSearchByElement(Queue, Element)    linkedListSearchByElement(Queue, Element);
#define QueueSearchByKey(Queue, Key)            linkedListSearchByKey(Queue, Key);
#define QueueGetSize(Queue)                     linkedListGetSize(Queue)
#define QueueHead(Queue)                        Queue -> nilNode -> nextNode
#define QueueGetNextNode(Node)                  Node -> nextNode
#define QueueNilNode(Queue)                     Queue -> nilNode



#endif /* DATASTRUCTURE_INCLUDE_QUEUE_H_ */
