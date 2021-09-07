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
 * stack.h
 *
 *  Created on: Jun 9, 2021
 *      Author: salar
 */



#ifndef DATASTRUCTURE_INCLUDE_STACK_H_
#define DATASTRUCTURE_INCLUDE_STACK_H_



#include <dataStructure/include/singlyLinkedList.h>



typedef singlyLinkedList_t                      stack_t;
typedef singlyLinkedListNode_t                  stackNode_t;



#define StackCreateNewNode(Element, Key)        singlyLinkedListCreateNewNode(Element, Key)
#define StackCreateNewStack()                   singlyLinkedListCreateNewList()
#define StackPush(Stack, Node)                  singlyLinkedListInsertFirst(Stack, Node)
#define StackPop(Stack)                         singlyLinkedListDeleteFirst(Stack)
#define StackSearchByElement(Stack, Element)    singlyLinkedListSearchByElement(Stack, Element);
#define StackSearchByKey(Stack, Key)            singlyLinkedListSearchByKey(Stack, Key);
#define StackGetSize(Stack)                     singlyLinkedListGetSize(Stack)
#define StackTop(Stack)                         Stack -> nilNode -> nextNode



#endif /* DATASTRUCTURE_INCLUDE_STACK_H_ */
