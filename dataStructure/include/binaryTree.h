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
 * binaryTree.h
 *
 *  Created on: Jun 12, 2021
 *      Author: salar
 */



#ifndef DATASTRUCTURE_INCLUDE_BINARYTREE_H_
#define DATASTRUCTURE_INCLUDE_BINARYTREE_H_



#include <stdint.h>



typedef struct binaryTreeNode{
    void                               *element;
    uint32_t                            key;
    struct binaryTreeNode              *parentNode;
    struct binaryTreeNode              *leftChildNode;
    struct binaryTreeNode              *rightChildNode;
}binaryTreeNode_t;

typedef struct binaryTree{
    uint32_t                            size;
    binaryTreeNode_t                   *rootNode;
    binaryTreeNode_t                   *nilNode;
}binaryTree_t;



binaryTreeNode_t                       *binaryTreeCreateNewNode(void *element, uint32_t key);
binaryTree_t                           *binaryTreeCreateNewTree(void);
void                                    binaryTreeInsert(binaryTree_t *tree, binaryTreeNode_t *z);
void                                    binaryTreeDelete(binaryTree_t *tree, binaryTreeNode_t *z);
binaryTreeNode_t                       *binaryTreeMinimum(binaryTree_t *tree, binaryTreeNode_t *x);
binaryTreeNode_t                       *binaryTreeMaximum(binaryTree_t *tree, binaryTreeNode_t *x);
binaryTreeNode_t                       *binaryTreeInteractiveTreeSearch(binaryTree_t *tree, binaryTreeNode_t *x, uint32_t key);
binaryTreeNode_t                       *binaryTreeSuccessor(binaryTree_t *tree, binaryTreeNode_t *x);
binaryTreeNode_t                       *binaryTreePredecessor(binaryTree_t *tree, binaryTreeNode_t *x);
uint32_t                                binaryTreeGetSize(binaryTree_t *tree);



#endif /* DATASTRUCTURE_INCLUDE_BINARYTREE_H_ */
