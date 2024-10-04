/*
 * @Author: Mr.Xiong
 * @Date: 2023-11-27 08:50:57
 * @LastEditTime: 2024-08-31 21:30:39
 * @LastEditors: Mr.Xiong
 * @Description: 
 * @FilePath: /Data-Structure/LinkList/linklist.c
 * 世上无难事,只怕有心人
 * Copyright (c) 2023 by Mr.Xiong, All Rights Reserved. 
 */


#include "linklist.h"


struct linklist *list_new ()
{
  struct linklist *new;
  new = (struct linklist *) malloc(sizeof(struct linklist));

    if(new != NULL)
    {
      memset(new,0,sizeof(struct linklist ));
    }
  return new;
}

struct linklist *list_create(list_cmp_cb_t cmp, list_del_cb_t del)
{
  struct linklist *new;

  new = (struct linklist *)malloc(sizeof(struct linklist));

  if(NULL == new)
  {
  	return NULL;
  }
   memset(new,0,sizeof(struct linklist ));

  new->cmp = cmp;
  new->del = del;
  return new;
}

/* Free list. */
void
list_free (struct linklist *l)
{
    mem_free (l);
    l = NULL;

}

/* Allocate new listnode.  Internal use only. */
static struct listnode *
listnode_new ()
{
      struct listnode *node;
      node = (struct listnode *)malloc(sizeof(struct listnode));
      node?  memset(node,0,sizeof(struct listnode )):NULL;

  return node;
}

/* Free listnode. */
static void
listnode_free (struct listnode *node)
{
  mem_free ( node);
  node = NULL;
}

/* Add new data to the list. */
struct listnode *
listnode_add (struct linklist *list, void *val)
{
  struct listnode *node;

  if ( (!list) || (!val) )
    return NULL;

  node = listnode_new ();
  if ( !node )
    return NULL;

  node->prev = list->tail;
  node->data = val;

  if (list->head == NULL)
    list->head = node;
  else
    list->tail->next = node;
  list->tail = node;

  list->count++;

  return node;
}

/* Add new node with sort function. */
struct listnode *
listnode_add_sort (struct linklist *list, void *val)
{
  struct listnode *node;
  struct listnode *new;

  new = listnode_new ();
  if (! new)
    return NULL;

  new->data = val;

  if (list->cmp)
    {
      for (node = list->head; node; node = node->next)
	{
	  if ((list->cmp(val, node->data)) < 0)
	    {
	      new->next = node;
	      new->prev = node->prev;

	      if (node->prev)
		node->prev->next = new;
	      else
		list->head = new;
	      node->prev = new;
	      list->count++;
	      return new;
	    }
	}
    }

  new->prev = list->tail;

  if (list->tail)
    list->tail->next = new;
  else
    list->head = new;

  list->tail = new;
  list->count++;
  return new;
}

struct listnode *listnode_add_head (struct linklist *list, void *val)
{
  struct listnode *node;

  node = listnode_new ();
  if( !node )
  {
    return NULL;
  }

  node->data = val;


  if (list->head)
      list->head->prev = node;
  else
      list->tail = node;

  node->next = list->head;
  node->prev = NULL;

  list->head = node;

  list->count++;

  return node;
}
struct listnode *listnode_add_tail (struct linklist *list, void *val)
{

    struct listnode *node = NULL;

    if ( (!list) || (!val) )
      return NULL;

    node = listnode_new ();
    if ( !node )
      return NULL;

    node->prev = list->tail;
    node->data = val;

    if (list->head == NULL)
        list->head = node;
    else
        list->tail->next = node;
    list->tail = node;

    list->count++;

    return node;
}

/* Delete specific date pointer from the list. */
void
listnode_delete (struct linklist *list, void *val)
{
  struct listnode *node;

  if ( (!list) || (!val) )
    return;

  for (node = list->head; node; node = node->next)
  {
      if (node->data == val)
      {
          if (node->prev)
              node->prev->next = node->next;
          else
              list->head = node->next;

          if (node->next)
              node->next->prev = node->prev;
          else
              list->tail = node->prev;

          list->count--;

          listnode_free (node);

          return;
      }
  }
}

void listnode_delete_node (struct linklist * list, listnode_t * node)
{
     if ( (!list) || (!node) )
         return;

     if (node->prev)
         node->prev->next = node->next;
     else
         list->head = node->next;

     if (node->next)
         node->next->prev = node->prev;
     else
         list->tail = node->prev;

     list->count--;

     listnode_free (node);
}

/* Delete all listnode from the list. */
void
list_delete_all_node (struct linklist *list)
{
  struct listnode *node;
  struct listnode *next;

  for (node = list->head; node; node = next)
    {
      next = node->next;
      if (list->del)
	list->del(node->data);
      listnode_free (node);
    }
  list->head = list->tail = NULL;
  list->count = 0;
}

/* Delete all listnode then free list itself. */
void
list_delete (struct linklist *list)
{
  list_delete_all_node (list);
  list_free (list);
}

void
list_delete_list (struct linklist *list)
{
  struct listnode *node;
  struct listnode *next;

  if (!list)
    return;

  for (node = list->head; node; node = next)
    {
      next = node->next;
      if (list->del)
	list->del(node->data);
      listnode_free (node);
    }
  list_free (list);
}

/* Lookup the node which has given data. */
struct listnode *
listnode_lookup (struct linklist *list, void *data)
{
  struct listnode *node;

  for (node = list->head; node; NEXTNODE (node))
    if (data == GETDATA (node))
      return node;
  return NULL;
}



 void
list_pointer_init(list_pointer_t* p_list)
{
    _LHEAD(p_list) = NULL;
    _LTAIL(p_list) = &p_list->head;
    p_list->count = 0;
}

  void
list_pointer_insert_head(list_pointer_t* p_list, list_pointer_node_t* p_node)
{
    if ((p_node->p_next = _LHEAD(p_list)) != NULL)
        p_node->p_next->p_prev = p_node;
    else
        _LTAIL(p_list) = p_node;
    _LHEAD(p_list) = p_node;
    p_node->p_prev = &p_list->head;

       p_list->count ++;
}

void
list_pointer_insert_tail(list_pointer_t* p_list, list_pointer_node_t* p_node)
{
    p_node->p_next = NULL;
    p_node->p_prev = _LTAIL(p_list);
    _LTAIL(p_list)->p_next = p_node;
    _LTAIL(p_list) = p_node;
     p_list->count ++;
}

void
list_pointer_insert_after(list_pointer_t* p_list, list_pointer_node_t* p_node1, list_pointer_node_t* p_node2)
{
      if ((p_node2->p_next = p_node1->p_next) != NULL)
          p_node2->p_next->p_prev = p_node2;
      else
          _LTAIL(p_list) = p_node2;
      p_node1->p_next = p_node2;
      p_node2->p_prev = p_node1;
      p_list->count ++;
}

void
list_pointer_insert_before(list_pointer_t* p_list, list_pointer_node_t* p_node1, list_pointer_node_t* p_node2)
{
    p_node2->p_next = p_node1;
    p_node2->p_prev = p_node1->p_prev;
    p_node1->p_prev->p_next = p_node2;
    p_node1->p_prev = p_node2;
    p_list->count ++;
}

list_pointer_node_t*
list_pointer_delete_head(list_pointer_t* p_list)
{
    list_pointer_node_t* p_node = _LHEAD(p_list);

    if ((_LHEAD(p_list) = p_node->p_next) != NULL)
        _LHEAD(p_list)->p_prev = &p_list->head;
    else
        _LTAIL(p_list) = &p_list->head;

    if ( p_list->count != 0)
    {
        p_list->count --;
    }
    return p_node;
}

list_pointer_node_t*
list_pointer_delete_tail(list_pointer_t* p_list)
{
    list_pointer_node_t* p_node = _LTAIL(p_list);

    _LTAIL(p_list) = p_node->p_prev;
    _LTAIL(p_list)->p_next = NULL;
    if ( p_list->count != 0)
    {
        p_list->count --;
    }

    return p_node;
}

void
list_pointer_delete(list_pointer_t* p_list, list_pointer_node_t* p_node)
{
    if (p_node->p_next != NULL)
        p_node->p_next->p_prev = p_node->p_prev;
    else
        _LTAIL(p_list) = p_node->p_prev;
    p_node->p_prev->p_next = p_node->p_next;

     if ( p_list->count != 0)
    {
        p_list->count --;
    }
}

int
list_pointer_empty(list_pointer_t* p_list)
{
    return _LHEAD(p_list) == NULL;
}

list_pointer_node_t*
list_pointer_head(list_pointer_t* p_list)
{
    return _LHEAD(p_list);
}

list_pointer_node_t*
list_pointer_node_tail(list_pointer_t* p_list)
{
    return _LTAIL(p_list)->p_prev->p_next;
}

list_pointer_node_t*
list_pointer_next(list_pointer_node_t* p_node)
{
    return p_node->p_next;
};

list_pointer_node_t*
list_pointer_prev(list_pointer_node_t* p_node)
{
    return p_node->p_prev->p_prev->p_next;
}


# if 0
/*
 * ************************  single-linked List   ***************************
*/
struct slist_s *slist_new()
{
    struct slist_s *new;

    new = (struct slist_s *)malloc(sizeof(struct slist_s));
    if (new != NULL)
    {
        memset(new, 0, sizeof(struct slist_s ));
    }
    return new;

}
void slist_free (struct slist_s *slist)
{
    mem_free (slist);
    return ;
}

slistnode_t *slist_add_head (struct slist_s *slist, slistnode_t *node)
{

    if ( (!slist) || (!node) )
        return NULL;

    if (slist->head == NULL)
    {
        slist->head = node;
        slist->tail = node;
    }
    else
    {
       node->next = slist->head;
    }
    slist->head = node;

    slist->count++;
    return node;


}
slistnode_t *slist_add_tail (struct slist_s *slist,  slistnode_t *node)
{
    if ( (!slist) || (!node) )
        return NULL;

    if (slist->head == NULL)
    {
        slist->head = node;
        slist->tail = node;
    }
    else
        slist->tail->next = node;

    slist->tail = node;

    slist->count++;

    return node;

}
void slist_delete_node(struct slist_s *slist, slistnode_t *del_node)
{
     slistnode_t *node;
    slistnode_t *next_node;

    if ( (!slist) || (!del_node) )
        return;

    for (node = slist->head,next_node =node ?node->next:NULL;
           node; node = next_node,next_node =node ?node->next:NULL)
    {
        if ( node == del_node )
        { /*head node*/
           slist->head =  next_node;
           if(!next_node)
                slist->tail = NULL;
             slist->count--;
            break;
        }

        if (NULL != next_node && next_node == del_node)
        {
            node->next = next_node->next;

            if (!next_node->next)
               slist->tail = node;
             slist->count--;
            break;
        }
    }
    return ;

}

void slist_delete_node2(struct slist_s *slist,slistnode_t *pre_or_next_node, slistnode_t *del_node)
{

    if ( (!slist) || (!del_node) )
        return;


    if (slist->head == del_node)
    {
        /*pre_or_next_node is next node*/
        slist->head =  pre_or_next_node;
        if (!pre_or_next_node)
            slist->tail = pre_or_next_node;
        slist->count--;
    }
    else
    {
        if (!pre_or_next_node)
        {
            return ;
        }
        pre_or_next_node->next = del_node->next;

        if (!del_node->next)
            slist->tail = pre_or_next_node;
      slist->count--;
    }


}

void slist_delete (struct slist_s *slist)
{
  slist_delete_all_node (slist);
  slist_free (slist);

}
void slist_delete_all_node (struct slist_s * slist)
{
    if ( (!slist) )
        return;

    slist->head = NULL;
    slist->tail = NULL;
    return ;
}

#endif
