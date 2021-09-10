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
 * binaryTree.c
 *
 *  Created on: Jun 12, 2021
 *      Author: salar
 */



#include <stdlib.h>
#include <dataStructure/include/binaryTree.h>



static void binaryTreeTransplant(binaryTree_t *tree, binaryTreeNode_t *u, binaryTreeNode_t *v);

binaryTreeNode_t *binaryTreeCreateNewNode(void *element, uint32_t key) {
    binaryTreeNode_t *newNode = (binaryTreeNode_t*)malloc(sizeof(binaryTreeNode_t));
    if(newNode == NULL){
        return NULL;
    }
    newNode -> element = element;
    newNode -> key = key;
    newNode -> parentNode = NULL;
    newNode -> leftChildNode = NULL;
    newNode -> rightChildNode = NULL;
    return newNode;
}

binaryTree_t *binaryTreeCreateNewTree(void) {
    binaryTree_t *newTree = (binaryTree_t*)malloc(sizeof(binaryTree_t));
    if (newTree == NULL) {
        return NULL;
    }
    binaryTreeNode_t *nilNode = (binaryTreeNode_t*)malloc(sizeof(binaryTreeNode_t));
    if (nilNode == NULL) {
        free(newTree);
        return NULL;
    }
    newTree -> size = 0;
    newTree -> rootNode = nilNode;
    newTree -> nilNode = nilNode;
    nilNode -> element = NULL;
    nilNode -> key = 0;
    nilNode -> parentNode = nilNode;
    nilNode -> leftChildNode = nilNode;
    nilNode -> rightChildNode = nilNode;
    return newTree;
}

void binaryTreeInsert(binaryTree_t *tree, binaryTreeNode_t *z) {
    binaryTreeNode_t *x = tree -> rootNode;
    binaryTreeNode_t *y = tree -> nilNode;
    while(x != tree -> nilNode){
        y = x;
        if ((z -> key) <= (x -> key)) {
            x = x -> leftChildNode;
        }
        else {
            x = x -> rightChildNode;
        }
    }
    z -> parentNode = y;
    if (y == tree -> nilNode) {
        tree -> rootNode = z;
    }
    else if ((z -> key) <= (y -> key)) {
        y -> leftChildNode = z;
    }
    else {
        y -> rightChildNode = z;
    }
    z -> leftChildNode = tree -> nilNode;
    z -> rightChildNode = tree -> nilNode;
    tree -> size++;
    return;
}

void binaryTreeDelete(binaryTree_t *tree, binaryTreeNode_t *z) {
    binaryTreeNode_t *y;
    if (z -> leftChildNode == tree -> nilNode) {
        binaryTreeTransplant(tree, z, z -> rightChildNode);
    }
    else if (z -> rightChildNode == tree -> nilNode) {
        binaryTreeTransplant(tree, z, z -> leftChildNode);
    }
    else {
        y = binaryTreeMinimum(tree, z -> rightChildNode);
        if (y -> parentNode != z) {
            binaryTreeTransplant(tree, y, y -> rightChildNode);
            y -> rightChildNode = z -> rightChildNode;
            y -> rightChildNode -> parentNode = y;
        }
        binaryTreeTransplant(tree, z, y);
        y -> leftChildNode = z -> leftChildNode;
        y -> leftChildNode -> parentNode = y;
    }
    tree -> size--;
}

binaryTreeNode_t *binaryTreeMinimum(binaryTree_t *tree, binaryTreeNode_t *x) {
    while (x -> leftChildNode != tree -> nilNode){
        x = x -> leftChildNode;
    }
    return x;
}

binaryTreeNode_t *binaryTreeMaximum(binaryTree_t *tree, binaryTreeNode_t *x) {
    while (x -> rightChildNode != tree -> nilNode){
        x = x -> rightChildNode;
    }
    return x;
}

binaryTreeNode_t *binaryTreeInteractiveTreeSearch(binaryTree_t *tree, binaryTreeNode_t *x, uint32_t key) {
    while (x != tree -> nilNode && key != x -> key){
        if(key < (x -> key)){
            x = x -> leftChildNode;
        }
        else {
            x = x -> rightChildNode;
        }
    }
    return x;
}

binaryTreeNode_t *binaryTreeSuccessor(binaryTree_t *tree, binaryTreeNode_t *x) {
    binaryTreeNode_t *y;
    if (x -> rightChildNode != tree -> nilNode) {
        return binaryTreeMinimum(tree, x -> rightChildNode);
    }
    y = x -> parentNode;
    while (y != tree -> nilNode && x == y -> rightChildNode){
        x = y;
        y = y -> parentNode;
    }
    return y;
}

binaryTreeNode_t *binaryTreePredecessor(binaryTree_t *tree, binaryTreeNode_t *x) {
	binaryTreeNode_t *y;
    if (x -> leftChildNode != tree -> nilNode) {
        return binaryTreeMaximum(tree, x -> leftChildNode);
    }
    y = x -> parentNode;
    while (y != tree -> nilNode && x == y -> leftChildNode){
        x = y;
        y = y -> parentNode;
    }
    return y;
}

uint32_t binaryTreeGetSize(binaryTree_t *tree) {
    return tree -> size;
}

static void binaryTreeTransplant(binaryTree_t *tree, binaryTreeNode_t *u, binaryTreeNode_t *v) {
    if (u -> parentNode == tree -> nilNode) {
        tree -> rootNode = v;
    }
    else if (u == u -> parentNode -> leftChildNode) {
        u -> parentNode -> leftChildNode = v;
    }
    else {
        u -> parentNode -> rightChildNode = v;
    }
    if(v != tree -> nilNode){
        v -> parentNode = u -> parentNode;
    }
}
