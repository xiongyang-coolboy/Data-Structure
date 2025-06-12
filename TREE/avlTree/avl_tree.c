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
#include "avl_tree.h"

/* Get a free AVL tree node. */
static struct avl_node *
_avl_node_new(struct avl_tree *avl_tree)
{
    struct avl_node *node;
    int32_t node_sz;

    node = avl_tree->free_list;
    if (node)
    {
        avl_tree->free_list = node->right;
        avl_tree->count++;
    }
    else if (avl_tree->max_nodes == 0)
    {
        node_sz = sizeof(struct avl_node);

        node = (struct avl_node *)malloc(node_sz);
        if (!node)
            return NULL;
        memset(node, 0, node_sz);

        avl_tree->count++;
    }

    return node;
}

/* Free a AVL tree node. */
static void
_avl_node_free(struct avl_tree *avl_tree, struct avl_node *node)
{
    node->right = avl_tree->free_list;
    node->left = NULL;
    avl_tree->free_list = node;
    avl_tree->count--;

    return;
}

/* Free AVL free node list. */
static void
_avl_freelist_free(struct avl_tree *avl_tree)
{
    struct avl_node *node;
    struct avl_node *next;

    for (node = avl_tree->free_list; node; node = next)
    {
        next = node->right;
        if (node)
            free(node);
        avl_tree->count--;
    }
}

/* Create AVL tree.
   If the max_nodes is 0, no preallocation is done. Every node is allocated
   first from the free_list and then from the system memory. */
int32_t avl_create(struct avl_tree **avl_tree,
                   int32_t max_nodes,
                   int32_t (*compare_function)(void *data1, void *data2))
{
    int32_t i;
    struct avl_tree *avl;
    struct avl_node *node;
    int32_t node_sz;

    avl = (struct avl_tree *)malloc(sizeof(struct avl_tree));
    if (!avl)
        return -1;

    memset(avl, 0, sizeof(struct avl_tree));

    avl->max_nodes = max_nodes;
    avl->root = NULL;
    avl->compare_function = compare_function;

    /* Initialize free list. */
    avl->free_list = NULL;

    /* Initialize max count. */
    avl->count = max_nodes;

    /* Check if nodes are to be preallocated. */
    if (max_nodes != 0)
    {
        /* Preallocate maximum number of entries and put on free list. */
        node_sz = sizeof(struct avl_node);

        avl->data = (char *)malloc(node_sz * max_nodes);
        if (!avl->data)
        {
            free(avl);
            return -1;
        }

        memset(avl->data, 0, node_sz * max_nodes);

        /* Add all entries in the free list. */
        for (i = 0; i < max_nodes; i++)
        {
            node = (struct avl_node *)(avl->data + i * node_sz);
            _avl_node_free(avl, node);
        }
    }

    *avl_tree = avl;

    return 0;
}

/* Delete AVL tree. */
static int32_t
avl_finish(struct avl_tree *avl_tree)
{
    /* Check tree nodes allocation type. */
    if (avl_tree->max_nodes == 0)
    {
        /* Free free list. */
        _avl_freelist_free(avl_tree);
    }
    else if (avl_tree->data)
    {
        /* Free all preallocated nodes. */
        free(avl_tree->data);
    }

    /* Free tree. */
    free(avl_tree);

    return 0;
}

/* Internal function for traversal. */
static int32_t
_avl_traverse(struct avl_tree *avl_tree,
              struct avl_node *node,
              avl_traversal_fn fn,
              void *data)
{
    int32_t ret;

    if (node != NULL)
    {
        if ((ret = _avl_traverse(avl_tree, node->left, fn, data)) < 0)
            return ret;

        if ((ret = (*fn)(node->info, data)) < 0)
            return ret;

        if ((ret = _avl_traverse(avl_tree, node->right, fn, data)) < 0)
            return ret;
    }

    return 0;
}

static int32_t
_avl_traverse2(struct avl_tree *avl_tree,
               struct avl_node *node,
               avl_traversal2_fn fn,
               void *data1, void *data2)
{
    int32_t ret;

    if (node != NULL)
    {
        if ((ret = _avl_traverse2(avl_tree, node->left, fn, data1, data2)) < 0)
            return ret;

        if ((ret = (*fn)(node->info, data1, data2)) < 0)
            return ret;

        if ((ret = _avl_traverse2(avl_tree, node->right, fn, data1, data2)) < 0)
            return ret;
    }

    return 0;
}

static int32_t
_avl_traverse3(struct avl_tree *avl_tree,
               struct avl_node *node,
               avl_traversal3_fn fn,
               void *data1, void *data2, void *data3)
{
    int32_t ret;

    if (node != NULL)
    {
        if ((ret = _avl_traverse3(avl_tree, node->left, fn, data1, data2,
                                  data3)) < 0)
            return ret;

        if ((ret = (*fn)(node->info, data1, data2, data3)) < 0)
            return ret;

        if ((ret = _avl_traverse3(avl_tree, node->right, fn, data1, data2,
                                  data3)) < 0)
            return ret;
    }

    return 0;
}

/* Traverse tree. */
int32_t avl_traverse(struct avl_tree *avl_tree, avl_traversal_fn fn, void *data)
{
    return _avl_traverse(avl_tree, avl_tree->root, fn, data);
}

int32_t avl_traverse2(struct avl_tree *avl_tree, avl_traversal2_fn fn, void *data1,
                      void *data2)
{
    return _avl_traverse2(avl_tree, avl_tree->root, fn, data1, data2);
}

int32_t avl_traverse3(struct avl_tree *avl_tree, avl_traversal3_fn fn, void *data1,
                      void *data2, void *data3)
{
    return _avl_traverse3(avl_tree, avl_tree->root, fn, data1, data2, data3);
}

/* Tree top. */
struct avl_node *
avl_top(struct avl_tree *avl_tree)
{
    return (avl_tree != NULL) ? avl_tree->root : NULL;
}

/* Tree left bottom node */
struct avl_node *avl_left_bottom_node(struct avl_tree *avl_tree, struct avl_node *left_node)
{

    if (left_node == NULL)
    {
        return NULL;
    }

    if (left_node->left != NULL)
    {
        return avl_left_bottom_node(avl_tree, left_node->left);
    }
    else
    {
        return left_node;
    }
}
/* Tree right bottom node . */
struct avl_node *
avl_right_bottom_node(struct avl_tree *avl_tree, struct avl_node *right_node)
{
    if (right_node == NULL)
    {
        return NULL;
    }

    if (right_node->right != NULL)
    {
        return avl_left_bottom_node(avl_tree, right_node->right);
    }
    else
    {
        return right_node;
    }
}

/* Internal function to lookup node. */
static struct avl_node *
_avl_lookup(struct avl_tree *tree, struct avl_node *node, void *data)
{
    int32_t cmp;

    if (node == NULL)
        return NULL;

    cmp = (*(tree->compare_function))(data, node->info);
    if (cmp < 0)
        return _avl_lookup(tree, node->left, data);
    if (cmp > 0)
        return _avl_lookup(tree, node->right, data);

    return node;
}

/* Lookup AVL tree. */
struct avl_node *
avl_search(struct avl_tree *tree, void *data)
{
    if (tree->root == NULL)
        return NULL;

    return _avl_lookup(tree, tree->root, data);
}

void *
avl_lookup_min(struct avl_tree *tree)
{
    avl_node_t *t;

    if (tree == NULL)
        return NULL;

    t = tree->root;

    while (NULL != t->left)
    {
        t = t->left;
    }

    return t->info;
}

void *
avl_lookup_max(struct avl_tree *tree)
{
    avl_node_t *t;

    if (tree == NULL)
        return NULL;

    t = tree->root;

    while (NULL != t->right)
    {
        t = t->right;
    }

    return t->info;
}

/* Next node. */
struct avl_node *
avl_next(struct avl_node *node)
{
    struct avl_node *start;
    int32_t count = 0;

    if (node->left)
        return node->left;

    if (node->right)
        return node->right;

    start = node;
    while (node->parent)
    {
        if (node->parent->left == node && node->parent->right)
            return node->parent->right;

        count++;

        node = node->parent;
    }

    return NULL;
}

/* Get next node. */
struct avl_node *
avl_getnext(struct avl_tree *tree, struct avl_node *node)
{
    struct avl_node *nodeA, *nodeB;

    if (tree->root == NULL)
        return NULL;
    nodeA = node;
    nodeB = (nodeA == NULL ? tree->root : nodeA->right);
    if (nodeB == NULL)
    {
        /* Go up parent from the left subtree */
        for (;;)
        {
            nodeB = nodeA->parent;
            if (nodeB == NULL)
                break;
            if (nodeA->flag == 0)
                break;
            nodeA = nodeB;
        }
    }
    else
    { /* go downstairs into the right subtree */
        for (;;)
        {
            nodeA = nodeB->left;
            if (nodeA == NULL)
                break;
            nodeB = nodeA;
        }
    }

    return nodeB;
}

/* Rotate tree for balancing. */
static struct avl_node *
_avl_rotate(struct avl_tree *tree, struct avl_node *node)
{
    struct avl_node *nodeA, *nodeB, *nodeC, *nodeD, *nodeE, *nodeF;

    nodeB = node;
    /*失衡结点的平衡因子小于0, 说明该结点左子树偏高*/
    if (nodeB->balance < 0)
    {
        nodeA = nodeB->parent;
        nodeC = nodeB->left;
        nodeD = nodeC->right;
        /*失衡结点的左子结点平衡因子小于0, LL型, 将二叉树右旋*/
        if (nodeC->balance <= 0)
        {
            if (nodeA == NULL)
                tree->root = nodeC;
            else if (nodeB->flag == 0)
                nodeA->left = nodeC;
            else
                nodeA->right = nodeC;
            nodeB->rank -= nodeC->rank;
            nodeC->parent = nodeA;
            nodeC->flag = nodeB->flag;
            nodeC->balance++;
            nodeC->right = nodeB;
            nodeB->parent = nodeC;
            nodeB->flag = 1;
            nodeB->balance = (-nodeC->balance);
            nodeB->left = nodeD;
            if (nodeD != NULL)
            {
                nodeD->parent = nodeB;
                nodeD->flag = 0;
            }
            node = nodeC;
        }
        else /*失衡结点的左子结点平衡因子大于0, LR型, 先左旋后右旋*/
        {
            /* Double negative rotation */
            nodeE = nodeD->left;
            nodeF = nodeD->right;
            if (nodeA == NULL)
                tree->root = nodeD;
            else if (nodeB->flag == 0)
                nodeA->left = nodeD;
            else
                nodeA->right = nodeD;
            nodeB->rank -= (nodeC->rank + nodeD->rank);
            nodeD->rank += nodeC->rank;
            nodeB->balance = (nodeD->balance >= 0 ? 0 : +1);
            nodeC->balance = (nodeD->balance <= 0 ? 0 : -1);
            nodeD->parent = nodeA;
            nodeD->flag = nodeB->flag;
            nodeD->balance = 0;
            nodeD->left = nodeC;
            nodeD->right = nodeB;
            nodeB->parent = nodeD;
            nodeB->flag = 1;
            nodeB->left = nodeF;
            nodeC->parent = nodeD;
            nodeC->flag = 0;
            nodeC->right = nodeE;
            if (nodeE != NULL)
            {
                nodeE->parent = nodeC;
                nodeE->flag = 1;
            }
            if (nodeF != NULL)
            {
                nodeF->parent = nodeB;
                nodeF->flag = 0;
            }
            node = nodeD;
        }
    }
    else /*失衡结点平衡因子大于0，右子树偏高*/
    {
        nodeA = nodeB->parent;
        nodeC = nodeB->right;
        nodeD = nodeC->left;
        /*失衡结点的右子结点的平衡因子大于0，是RR型,对二叉树进行左旋调整*/
        if (nodeC->balance >= 0)
        {
            /* Single positive rotation */
            if (nodeA == NULL)
                tree->root = nodeC;
            else if (nodeB->flag == 0)
                nodeA->left = nodeC;
            else
                nodeA->right = nodeC;
            nodeC->rank += nodeB->rank;
            nodeC->parent = nodeA;
            nodeC->flag = nodeB->flag;
            nodeC->balance--;
            nodeC->left = nodeB;
            nodeB->parent = nodeC;
            nodeB->flag = 0;
            nodeB->balance = (-nodeC->balance);
            nodeB->right = nodeD;
            if (nodeD != NULL)
            {
                nodeD->parent = nodeB;
                nodeD->flag = 1;
            }
            node = nodeC;
        }
        else /*失衡结点的右子结点的平衡因子小于0，是RL型,对二叉树进行先 右旋后左旋 调整*/
        {
            nodeE = nodeD->left;
            nodeF = nodeD->right;
            if (nodeA == NULL)
                tree->root = nodeD;
            else if (nodeB->flag == 0)
                nodeA->left = nodeD;
            else
                nodeA->right = nodeD;
            nodeC->rank -= nodeD->rank;
            nodeD->rank += nodeB->rank;
            nodeB->balance = (nodeD->balance <= 0 ? 0 : -1);
            nodeC->balance = (nodeD->balance >= 0 ? 0 : +1);
            nodeD->parent = nodeA;
            nodeD->flag = nodeB->flag;
            nodeD->balance = 0;
            nodeD->left = nodeB;
            nodeD->right = nodeC;
            nodeB->parent = nodeD;
            nodeB->flag = 0;
            nodeB->right = nodeE;
            nodeC->parent = nodeD;
            nodeC->flag = 1;
            nodeC->left = nodeF;
            if (nodeE != NULL)
            {
                nodeE->parent = nodeB;
                nodeE->flag = 1;
            }
            if (nodeF != NULL)
            {
                nodeF->parent = nodeC;
                nodeF->flag = 0;
            }
            node = nodeD;
        }
    }
    return node;
}

/* Insert a node in the AVL tree. */
int32_t avl_insert(struct avl_tree *tree, void *data)
{
    struct avl_node *nodeA, *nodeB, *nodeC;
    int32_t flag = 0; /*flag 标识结点为父节点的左子树还是右子树*/

    /* Create a new node. */
    nodeC = _avl_node_new(tree);

    if (nodeC == NULL)
        return -1;

    /* Find location for insertion. */
    nodeA = NULL;
    nodeB = tree->root;

    while (nodeB != NULL)
    {
        nodeA = nodeB;
        if (tree->compare_function(data, nodeA->info) <= 0)
        {
            flag = 0;
            nodeB = nodeA->left;
            nodeA->rank++;
        }
        else
        {
            flag = 1;
            nodeB = nodeA->right;
        }
    }

    nodeC->info = data;
    nodeC->rank = 1;
    nodeC->parent = nodeA;
    nodeC->flag = (nodeA == NULL ? 0 : flag);
    nodeC->balance = 0;
    nodeC->left = nodeC->right = NULL;
    if (nodeA == NULL)
        tree->root = nodeC;
    else if (flag == 0)
        nodeA->left = nodeC;
    else
        nodeA->right = nodeC;

    /* Go parent root and correct all subtrees affected by insertion. */
    while (nodeA != NULL)
    {
        /*左子树增加，平衡因子-1， 右子树高度增加 平衡因子+1*/
        if (flag == 0)
        {
            /* Height of p's left subtree increased. */
            if (nodeA->balance > 0)
            {
                nodeA->balance = 0;
                break;
            }
            if (nodeA->balance < 0)
            {
                _avl_rotate(tree, nodeA); /*失衡，对二叉树进行调整*/
                break;
            }
            nodeA->balance = -1;
            flag = nodeA->flag;
            nodeA = nodeA->parent; /*向父节点回溯*/
        }
        else
        {
            /* Height of p's right subtree increased. */
            if (nodeA->balance < 0)
            {
                nodeA->balance = 0;
                break;
            }
            if (nodeA->balance > 0)
            {
                _avl_rotate(tree, nodeA);
                break;
            }
            nodeA->balance = +1;
            flag = nodeA->flag;
            nodeA = nodeA->parent;
        }
    }

    return 0;
}

/* Remove a node entry. */
static int32_t
_avl_delete_node(struct avl_tree *tree, struct avl_node *node)
{
    struct avl_node *nodeA, *nodeB, *nodeC, *nodeD, *nodeE, *nodeF, *nodeG;
    int32_t flag;

    if (node == NULL)
        return -1;

    nodeB = node;

    /* If both subtrees of the specified node are non-empty, the node
    should be interchanged with the next one, at least one subtree
    of which is always empty. */
    if (nodeB->left == NULL || nodeB->right == NULL)
        goto SKIP;

    nodeA = nodeB->parent;
    nodeC = nodeB->left;

    nodeD = avl_getnext(tree, nodeB);
    nodeE = nodeD->right;
    if (nodeB->right == nodeD)
    {
        if (nodeA == NULL)
            tree->root = nodeD;
        else if (nodeB->flag == 0)
            nodeA->left = nodeD;
        else
            nodeA->right = nodeD;
        nodeD->rank = nodeB->rank;
        nodeD->parent = nodeA;
        nodeD->flag = nodeB->flag;
        nodeD->balance = nodeB->balance;
        nodeD->left = nodeC;
        nodeD->right = nodeB;
        nodeC->parent = nodeD;
        nodeB->rank = 1;
        nodeB->parent = nodeD;
        nodeB->flag = 1;
        nodeB->balance = (nodeE == NULL ? 0 : +1);
        nodeB->left = NULL;
        nodeB->right = nodeE;

        if (nodeE != NULL)
            nodeE->parent = nodeB;
    }
    else
    {
        nodeF = nodeB->right;
        nodeG = nodeD->parent;

        if (nodeA == NULL)
            tree->root = nodeD;
        else if (nodeB->flag == 0)
            nodeA->left = nodeD;
        else
            nodeA->right = nodeD;

        nodeD->rank = nodeB->rank;
        nodeD->parent = nodeA;
        nodeD->flag = nodeB->flag;
        nodeD->balance = nodeB->balance;
        nodeD->left = nodeC;
        nodeD->right = nodeF;
        nodeC->parent = nodeD;
        nodeF->parent = nodeD;
        nodeG->left = nodeB;
        nodeB->rank = 1;
        nodeB->parent = nodeG;
        nodeB->flag = 0;
        nodeB->balance = (nodeE == NULL ? 0 : +1);
        nodeB->left = NULL;
        nodeB->right = nodeE;

        if (nodeE != NULL)
            nodeE->parent = nodeB;
    }

SKIP:
    /* Now the specified node [p] has at least one empty subtree;
    go up to the root and adjust the rank field of all nodes
    affected by deletion */
    nodeC = nodeB;
    nodeA = nodeC->parent;
    while (nodeA != NULL)
    {
        if (nodeC->flag == 0)
            nodeA->rank--;
        nodeC = nodeA;
        nodeA = nodeC->parent;
    }

    /* delete the specified node from the tree */
    nodeA = nodeB->parent;
    flag = nodeB->flag;
    nodeC = nodeB->left != NULL ? nodeB->left : nodeB->right;

    if (nodeA == NULL)
        tree->root = nodeC;
    else if (flag == 0)
        nodeA->left = nodeC;
    else
        nodeA->right = nodeC;
    if (nodeC != NULL)
    {
        nodeC->parent = nodeA;
        nodeC->flag = flag;
    }

    /* Go up to the root and correct all subtrees affected by
    deletion */
    while (nodeA != NULL)
    {
        if (flag == 0)
        { /* the height of the left subtree of [f] is decreased */
            if (nodeA->balance == 0)
            {
                nodeA->balance = +1;
                break;
            }
            if (nodeA->balance < 0)
                nodeA->balance = 0;
            else
            {
                nodeA = _avl_rotate(tree, nodeA);
                if (nodeA->balance < 0)
                    break;
            }
            flag = nodeA->flag;
            nodeA = nodeA->parent;
        }
        else
        { /* the height of the right subtree of [f] is decreased */
            if (nodeA->balance == 0)
            {
                nodeA->balance = -1;
                break;
            }
            if (nodeA->balance > 0)
                nodeA->balance = 0;
            else
            {
                nodeA = _avl_rotate(tree, nodeA);
                if (nodeA->balance > 0)
                    break;
            }
            flag = nodeA->flag;
            nodeA = nodeA->parent;
        }
    }

    /* Free node. */
    _avl_node_free(tree, nodeB);

    return 0;
}

/* Delete node containing data. */
int32_t avl_remove(struct avl_tree *tree, void *data)
{
    struct avl_node *node;
    int32_t ret = 0;

    node = avl_search(tree, data);
    if (node)
    {
        /* Delete node. */
        ret = _avl_delete_node(tree, node);
    }
    else
        ret = -1;

    return ret;
}

/* Delete node from AVL tree. */
int32_t avl_delete_node(struct avl_tree *avl_tree, struct avl_node *node)
{
    return _avl_delete_node(avl_tree, node);
}

/* Get  AVL tree node count. */
int32_t avl_get_tree_size(struct avl_tree *p_avl_tree)
{
    return p_avl_tree->count;
}

/*
tree free function.
*/
int32_t avl_tree_free(struct avl_tree **pp_avl_tree, void (*avl_data_free)(void *ptr))
{
    struct avl_node *tmp_node;
    struct avl_node *node;

    if (*pp_avl_tree == NULL)
        return (0);

    node = avl_top(*pp_avl_tree);

    while (node)
    {
        if (node->left)
        {
            node = node->left;
            continue;
        }

        if (node->right)
        {
            node = node->right;
            continue;
        }

        tmp_node = node;
        node = node->parent;

        if (node != NULL)
        {
            if (node->left == tmp_node)
                node->left = NULL;
            else
                node->right = NULL;

            /* Free node data */
            if (avl_data_free)
                (*avl_data_free)(tmp_node->info);

            /* Move node to a free list.*/
            _avl_node_free(*pp_avl_tree, tmp_node);
        }
        else
        {
            /* Free node data */
            if (avl_data_free)
                (*avl_data_free)(tmp_node->info);

            /* Move node to a free list.*/
            _avl_node_free(*pp_avl_tree, tmp_node);
            break;
        }
    }

    /* Delete tree. */
    avl_finish(*pp_avl_tree);
    *pp_avl_tree = NULL;
    return 0;
}

/*
  Tree cleanup. Remove all nodes from tree but do not free the tree.
*/
int32_t avl_tree_cleanup(struct avl_tree *avl_tree, void (*avl_data_free)(void *ptr))
{
    struct avl_node *tmp_node;
    struct avl_node *node;

    if (avl_tree == NULL)
        return -1;

    node = avl_top(avl_tree);

    while (node)
    {
        if (node->left)
        {
            node = node->left;
            continue;
        }

        if (node->right)
        {
            node = node->right;
            continue;
        }

        tmp_node = node;
        node = node->parent;

        if (node != NULL)
        {
            if (node->left == tmp_node)
                node->left = NULL;
            else
                node->right = NULL;

            /* Free node data */
            if (avl_data_free)
                (*avl_data_free)(tmp_node->info);

            /* Move node to a free list.*/
            _avl_node_free(avl_tree, tmp_node);
        }
        else
        {
            /* Free node data */
            if (avl_data_free)
                (*avl_data_free)(tmp_node->info);

            /* Move node to a free list.*/
            _avl_node_free(avl_tree, tmp_node);
            break;
        }
    }

    avl_tree->root = NULL;
    return 0;
}
