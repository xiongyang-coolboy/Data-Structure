/****************************************************************************
 *file linklist.h

 *author  Copyright (C) 2011 Centec Networks Inc.  All rights reserved.

 *date 2009-11-26

 *version v2.0

  This file contains  single/double linklist arithmetic lib
 ****************************************************************************/

#ifndef _LINKLIST_H_
#define _LINKLIST_H_

#include <stdint.h>
#include <stdlib.h>

/****************************************************************
*
* Defines and Macros
*
****************************************************************/
/*
 * ************************  Double-linked List   ***************************
 */
/*定义删除操作和比较操作的函数指针，具体实现由用户自定义*/
typedef void (*list_del_cb_t) (void *val); 
typedef int32_t (*list_cmp_cb_t) (void *val1, void *val2);

/**
 @brief  Define list node
*/
typedef struct listnode
{
    struct listnode *next;      /**< pointer on struct type listnode */
    struct listnode *prev;      /**< pointer on struct type listnode */
    void *data;                     /**< void type pointer */
} listnode_t;

/**
 @brief  Define linklist
*/
typedef struct linklist
{
    struct listnode *head;  /**< pointer on struct type listnode */
    struct listnode *tail;  /**< pointer on struct type listnode */
    uint32_t count;               /**< unsigned 32-bit variable */
    list_cmp_cb_t cmp;      /**< list_cmp_cb_t type variable */
    list_del_cb_t del;      /**< list_cmp_cb_t type variable */
} linklist_t;

#define NEXTNODE(X) ((X) = (X)->next)
#define LISTHEAD(X) (((X) != NULL) ? ((X)->head) : NULL)
#define LISTTAIL(X) (((X) != NULL) ? ((X)->tail) : NULL)
#define LISTCOUNT(X) (((X) != NULL) ? ((X)->count) : 0)
#define listcount(X) (((X) != NULL) ? ((X)->count) : 0)
#define LIST_ISEMPTY(X) ((X)->head == NULL && (X)->tail == NULL)
#define list_isempty(X) ((X)->head == NULL && (X)->tail == NULL)
#define GETDATA(X) (((X) != NULL) ? (X)->data : NULL)

/* Prototypes. */
struct linklist *list_new(void);
struct linklist *list_create(list_cmp_cb_t cmp_cb, list_del_cb_t del_cb);

void list_free (struct linklist *);

struct listnode *listnode_add (struct linklist *, void *);
struct listnode *listnode_add_sort (struct linklist *, void *);
struct listnode *listnode_add_head (struct linklist *, void *);
struct listnode *listnode_add_tail (struct linklist *, void *);

void listnode_delete (struct linklist *, void *);
void listnode_delete_node (struct linklist *, listnode_t *);
struct listnode *listnode_lookup (struct linklist *, void *);
void list_delete (struct linklist *);
void list_delete_all_node (struct linklist *);


/* List iteration macro. */
#define LIST_LOOP(L,V,N) \
  if (L) \
    for ((N) = (L)->head; (N); (N) = (N)->next) \
      if (((V) = (N)->data) != NULL)

/* List reverse iteration macro. */
#define LIST_REV_LOOP(L,V,N) \
  if (L) \
    for ((N) = (L)->tail; (N); (N) = (N)->prev) \
      if (((V) = (N)->data) != NULL)

/* List reverse iteration macro. */
#define LIST_REV_LOOP_DEL(L,V,N,NN) \
  if (L) \
    for ((N) = (L)->tail, NN = ((N)!=NULL) ? (N)->prev : NULL; \
         (N); \
         (N) = (NN), NN = ((N)!=NULL) ? (N)->prev : NULL) \
      if (((V) = (N)->data) != NULL)

/* List iteration macro.
 * It allows to delete N and V in the middle of the loop
 */
#define LIST_LOOP_DEL(L,V,N,NN) \
  if (L) \
    for ((N) = (L)->head, NN = ((N)!=NULL) ? (N)->next : NULL;  \
         (N);                                                   \
         (N) = (NN), NN = ((N)!=NULL) ? (N)->next : NULL)       \
      if (((V) = (N)->data) != NULL)


/* List node add macro.  */
#define LISTNODE_ADD(L,N) \
  { \
    (N)->next = NULL; \
    (N)->prev = (L)->tail; \
    if ((L)->head == NULL) \
      (L)->head = (N); \
    else \
      (L)->tail->next = (N); \
    (L)->tail = (N); \
  }

/* List node delete macro.  */
#define LISTNODE_DELETE(L,N) \
  { \
    if ((N)->prev) \
      (N)->prev->next = (N)->next; \
    else \
      (L)->head = (N)->next; \
    if ((N)->next) \
      (N)->next->prev = (N)->prev; \
    else \
      (L)->tail = (N)->prev; \
  }


/*
 * ************************  Double-linked List ,and don't store none data   ***************************
 */
 /**
 @brief  Define ipuc list pointer node
*/
 typedef struct list_pointer_node_s
 {
     struct list_pointer_node_s* p_next;    /**< list_pointer_node_s type pointer */
     struct list_pointer_node_s* p_prev;    /**< list_pointer_node_s type pointer */
 } list_pointer_node_t;

/**
 @brief  Define list pointer
*/
 typedef struct
 {
     list_pointer_node_t head;  /**< list_pointer_node_t type variable */
     uint32_t      count;             /**< unsigned 32-bit variable */
 } list_pointer_t;

/* Define a list */
#define _LIST_DEF(list) list_pointer_node_t list = {{NULL, &list.head}}

#define _LHEAD(p_list) ((p_list)->head.p_next)
#define _LTAIL(p_list) ((p_list)->head.p_prev)

void
list_pointer_init(list_pointer_t* p_list);

void
list_pointer_insert_head(list_pointer_t* p_list, list_pointer_node_t* p_node);
void
list_pointer_insert_tail(list_pointer_t* p_list, list_pointer_node_t* p_node);

void
list_pointer_insert_after(list_pointer_t* p_list, list_pointer_node_t* p_node1, list_pointer_node_t* p_node2);
void
list_pointer_insert_before(list_pointer_t* p_list, list_pointer_node_t* p_node1, list_pointer_node_t* p_node2);

list_pointer_node_t*
list_pointer_delete_head(list_pointer_t* p_list);

list_pointer_node_t*
list_pointer_delete_tail(list_pointer_t* p_list);

void
list_pointer_delete(list_pointer_t* p_list, list_pointer_node_t* p_node);
int
list_pointer_empty(list_pointer_t* p_list);
list_pointer_node_t*
list_pointer_head(list_pointer_t* p_list);


list_pointer_node_t*
list_pointer_node_tail(list_pointer_t* p_list);

list_pointer_node_t*
list_pointer_next(list_pointer_node_t* p_node);
list_pointer_node_t*
list_pointer_prev(list_pointer_node_t* p_node);

#define LIST_POINTER_ISEMPTY(X) (((X)->head.p_next == NULL) && ((X)->head.p_prev == &((X)->head)))

#define LIST_POINTER_COUNT(X) (((X) != NULL) ? ((X)->count) : 0)

#define LIST_POINTER_LOOP(p_node, p_list) \
  if (p_list) \
for (p_node = list_pointer_head(p_list); p_node; p_node = list_pointer_next(p_node))

/* List iteration macro.
 * It allows to delete N and V in the middle of the loop
 */
#define LIST_POINTER_LOOP_DEL(p_node,p_next_node, p_list) \
if (p_list) \
for (p_node = list_pointer_head(p_list), p_next_node = ((p_node)!=NULL) ? (p_node)->p_next : NULL;\
p_node;p_node = p_next_node, p_next_node = ((p_node)!=NULL) ? (p_node)->p_next : NULL)


#define LIST_POINTER_LOOP_R(p_node, p_list) \
if (p_list) \
for (p_node = list_pointer_node_tail(p_list); p_node; p_node = list_pointer_prev(p_node))

#define _container_of(p, type, member) \
(type *)((char *)p - (int)&((type *)0)->member)

/*
 * ************************  single-linked List   ***************************
 */

/**
 @brief  Define slist node
*/
typedef struct slistnode_s
{
    struct slistnode_s *next;   /**< slistnode_s type pointer */
} slistnode_t;

/**
 @brief  Define slist
*/
typedef struct slist_s
{
    struct slistnode_s *head;   /**< slistnode_s type pointer */
    struct slistnode_s *tail;   /**< slistnode_s type pointer */
    uint32_t count;                   /**< unsigned 32-bit variable */
} slist_t;

#define SLISTCOUNT(X) (((X) != NULL) ? ((X)->count) : 0)
#define SLISTHEAD(X) (((X) != NULL) ? ((X)->head) : NULL)
#define SLISTTAIL(X) (((X) != NULL) ? ((X)->tail) : NULL)
#define SLISTNEXTNODE(X) ((X) =((X) != NULL) ? ((X)->next) : NULL)


/* Prototypes. */
struct slist_s *slist_new(void);
void slist_free (struct slist_s *);

slistnode_t *slist_add_head (struct slist_s *, slistnode_t *);
slistnode_t *slist_add_tail (struct slist_s *, slistnode_t *);

void slist_delete_node(struct slist_s *, slistnode_t *);
void slist_delete_node2(struct slist_s *slist,slistnode_t *, slistnode_t *);

void slist_delete (struct slist_s *);
void slist_delete_all_node (struct slist_s *);

#define SLIST_ISEMPTY(X) (((X)->head == NULL) && ((X)->tail == NULL))

/* List iteration macro. */
#define SLIST_LOOP(L,N) \
  if (L) \
    for ((N) = (L)->head; (N); (N) = (N)->next) \



/* List iteration macro.
 * It allows to delete N and V in the middle of the loop
 */
#define SLIST_LOOP_DEL(L,N,NN) \
  if (L) \
    for ((N) = (L)->head, NN = ((N)!=NULL) ? (N)->next : NULL;  \
         (N);                                                   \
         (N) = (NN), NN = ((N)!=NULL) ? (N)->next : NULL)       \


#endif /* _SLINKLIST_H_ */

