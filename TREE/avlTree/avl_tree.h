/***********************************************
*
* Copyright (c), 2022-2025, xxxx. Co., Ltd.
*
* ⽂件名称: 
* 版 本 号: 1.0
* ⽣成⽇期: 2025-6-5
* 作    者: Mr.Xiong
* 所 属 层:
* 功    能:
*
************************************************/

#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

/* 需要的头文件 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/****************************************************************
*
* Defines and Macros
*
****************************************************************/

/**
 @addtogroup avltree AVLTREE
 @{
*/

/**
 @brief Define avl tree node
*/
 typedef struct avl_node
 {
     struct avl_node *left;     /**< Left node. */
     struct avl_node *right;    /**< Right node. */
     struct avl_node *parent;   /**< Parent node. */
     int32_t balance;                 /**< Balance factor. */
     int32_t rank;                    /**< Relative position of node in own subtree i.e. number of nodes in left subtree + 1. */
     int32_t flag;                    /**< 0 -> left child of its parent or is root of the tree. 1 -> right child of its parent. */
     void *info;                    /**< Data. */
 } avl_node_t;

/**
 @brief Define avl tree
*/
typedef struct avl_tree
{
  int32_t max_nodes ;                                     /**< Maximum number of bytes for data. */
  int32_t (*compare_function) (void *data1, void *data2); /**< Compare function. */
  char *data;                                           /**< Array for preallocated entries. */
  struct avl_node *root;                            /**< Root. */
  struct avl_node *free_list;                       /**< Free list. */
  int32_t count;                                          /**< Number of entries in tree. */
} avl_tree_t;

typedef int32_t (*avl_traversal_fn)(void *data, void *user_data);
typedef int32_t (*avl_traversal2_fn)(void *data, void *user_data1,
                                 void *user_data2);
typedef int32_t (*avl_traversal3_fn)(void *data, void *user_data1,
                                 void *user_data2, void *user_data3);
#define AVL_NODE_INFO(n)       (n)->info
#define AVL_COUNT(t)           (t)->count
#define AVL_NODE_LEFT(n)       (n)->left
#define AVL_NODE_RIGHT(n)      (n)->right

#define AVL_TREE_LOOP(T,V,N)                              \
  if (T)                                                  \
    for ((N) = avl_top ((T)); (N); (N) = avl_next ((N)))  \
      if (((V) = (N)->info) != NULL)


#define AVL_LOOP_DEL(T,V,N) \
  if (T) \
    for ((N) = avl_top (T); N ;  \
         (N) = avl_top (T))\
      if (((V) = (N)->info) != NULL)

/**< Function declarations. */

/**< Delete node from AVL tree. */
int32_t avl_delete_node (struct avl_tree *avl_tree, struct avl_node *node);

/**< If the max_nodes is 0, no preallocation is done. Every node is allocated first from the free_list and then from the system memory. */
int32_t avl_create (struct avl_tree **avl_tree,
                      int32_t max_nodes,
                      int32_t (*compare_function) (void *data1, void *data2));

/**< Traverse tree. */
int32_t avl_traverse (struct avl_tree *avl_tree, avl_traversal_fn fn, void *data);

int32_t avl_traverse2 (struct avl_tree *avl_tree, avl_traversal2_fn fn,
                         void *data1, void *data2);

int32_t avl_traverse3 (struct avl_tree *avl_tree, avl_traversal3_fn fn,
                         void *data1, void *data2, void *data3);

/**< Tree left bottom node */
struct avl_node *
avl_left_bottom_node (struct avl_tree *avl_tree,struct avl_node *left_node);
/**< Tree right bottom node . */
struct avl_node *
avl_right_bottom_node (struct avl_tree *avl_tree,struct avl_node *right_node);

/**< Tree top. */
struct avl_node *avl_top (struct avl_tree *avl_tree);

/**< Get next entry in AVL tree. */
struct avl_node* avl_getnext (struct avl_tree *avl_tree, struct avl_node *node);

/**< Delete AVL node containing the data. */
int32_t avl_remove (struct avl_tree *avl_tree, void *data);

/**< Insert a node in AVL tree. */
int32_t avl_insert (struct avl_tree *avl_tree, void *data);


/**< Lookup AVL tree. */
struct avl_node *avl_search (struct avl_tree *avl_tree, void *data);

/**< Get next node. */
struct avl_node* avl_next (struct avl_node *node);

/**< Get  AVL tree node count. */
int32_t avl_get_tree_size(struct avl_tree *p_avl_tree);

/**< Tree cleanup. Remove all nodes from tree but do not free the tree. */
int32_t avl_tree_cleanup (struct avl_tree *avl_tree, void (*avl_data_free)(void *ptr));

/**< Free AVL tree . */
int32_t avl_tree_free (struct avl_tree **pp_avl_tree, void (*avl_data_free)(void *ptr));

void* avl_lookup_min (struct avl_tree *tree);
void* avl_lookup_max (struct avl_tree *tree);

/**@} end of @defgroup avltree  */

#endif /* _AVL_TREE_H_ */
