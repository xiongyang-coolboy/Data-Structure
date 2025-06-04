#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "linklist.h"

/*简单定义端口信息*/
typedef struct
{
    uint32_t ifIndex;
#define LINK_UP   1
#define LINK_DOWN 2
    int32_t linkState;
} tIfInfo;

typedef enum
{
    IF_CREATE = 0,
    IF_UPDATE = 1,
    IF_READ   = 2,
    IF_DELETE = 3,
}IF_OPER_TYPE_E;

typedef struct
{
    linklist_t *ifList;
}tGLobalData;

tGLobalData globalData;

#define OK 0
#define ERROR ~0

int32_t ifAdd(tIfInfo *pIf)
{
    listnode_t *listnode = NULL;
    /*直接尾插*/
    // listnode = (globalData.ifList, pIf);
    /*用注册的cmp比较函数，进行有序排列*/
    listnode = listnode_add_sort(globalData.ifList, pIf);
    if (NULL == listnode)
    {
        return ERROR;
    }

    return OK;
}

int32_t ifReadAll()
{
    listnode_t *node = NULL;
    tIfInfo *pIf = NULL;

    LIST_LOOP(globalData.ifList, pIf, node)
    {
        printf("ifIndex is %u, if linkstate is %d\n", pIf->ifIndex, pIf->linkState);
    }

    return OK;
}

/**
 * @func: ifDelNode
 * @description:  删除单个结点
 * @param {tIfInfo} *pIf
 * @return int32_t
 * @author: Mr.Xiong
 */
int32_t ifDelNode(tIfInfo *pIf)
{
    listnode_delete(globalData.ifList, pIf);
    free(pIf);
    pIf = NULL;

    return OK;
}

/*删除所有结点*/
int32_t ifDelAllNode()
{
    listnode_t *node = NULL;
    listnode_t *nextNode = NULL;
    tIfInfo *pIf = NULL;

    LIST_LOOP_DEL(globalData.ifList, pIf, node, nextNode)
    {
        listnode_delete_node(globalData.ifList, node);
        free(pIf);
        pIf = NULL;
    }

    return OK;
}

/*curd操作句柄*/
int32_t ifHandle(tIfInfo *pIf, IF_OPER_TYPE_E operType)
{
    if (NULL == pIf)
    {
        return ERROR;
    }

    switch (operType)
    {
        case IF_CREATE:
            ifAdd(pIf);
            break;
        case IF_UPDATE:
            /* code */
            break;
        case IF_READ:
            break;
        case IF_DELETE:
            ifDelNode(pIf);
            break;
        default:
            break;
    }

    return OK;
}

int32_t if_index_cmp(void *data1, void *data2)
{
    tIfInfo *ifInfoData1 = NULL;
    tIfInfo *ifInfoData2 = NULL;

    if ((NULL == data1) || (NULL == data2))
    {
        return 0;
    }

    ifInfoData1 = (tIfInfo *)data1;
    ifInfoData2 = (tIfInfo *)data2;

    return memcmp(&ifInfoData1->ifIndex, &ifInfoData2->ifIndex, sizeof(uint32_t));
}

int main(int argc, char const *argv[])
{
    tIfInfo *pIf = NULL;

    globalData.ifList = list_create(if_index_cmp, NULL);

    for(int i = 0; i < 50; i++)
    {
        pIf = (tIfInfo *)malloc(sizeof(tIfInfo));
        if (NULL == pIf)
        {
            return ERROR;
        }

        pIf->ifIndex = rand()%200; 
        pIf->linkState = rand()%2 + 1;

        ifHandle(pIf, IF_CREATE);
    }

    printf("the linlist has %d node.", globalData.ifList->count);

    /*遍历操作*/
    ifReadAll();
    /*删除操作*/
    ifDelAllNode();

    /*释放链表*/
    if (0 == LISTCOUNT(globalData.ifList))
    {
        list_delete(globalData.ifList);
        printf("delete linklist\n");
        globalData.ifList = NULL;
    }

    return 0;
}
