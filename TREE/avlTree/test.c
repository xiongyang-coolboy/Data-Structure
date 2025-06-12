#include <stdio.h>
#include <stdbool.h>
#include "avl_tree.h"

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
    avl_tree_t *ifTree;
}tGLobalData;

tGLobalData globalData;

#define OK 0
#define ERROR ~0

int32_t ifAdd(tIfInfo *pIf)
{
    avl_insert(globalData.ifTree, pIf);
    return OK;
}

int32_t printNode(tIfInfo *pIf, void *data) 
{
    if (NULL == pIf)
    {
        return ERROR;
    }

    printf("ifIndex is %u, if linkstate is %d\n", pIf->ifIndex, pIf->linkState);

    return 0; // 继续遍历
}

int32_t ifReadAll()
{   
    avl_node_t *pNode = NULL;
    tIfInfo *pIf = NULL;

#if 0
    /*遍历*/
    AVL_TREE_LOOP(globalData.ifTree, pIf, pNode)
    {
        printf("ifIndex is %u, if linkstate is %d\n", pIf->ifIndex, pIf->linkState);
    }     
#endif

    avl_traverse(globalData.ifTree, (avl_traversal_fn)printNode, NULL);
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
    avl_remove(globalData.ifTree, pIf);
    free(pIf);
    pIf = NULL;

    return OK;
}

/*删除所有结点*/
int32_t ifDelAllNode()
{
    tIfInfo *pIf = NULL;
    avl_node_t *pNode = NULL;

    AVL_LOOP_DEL(globalData.ifTree, pIf, pNode)
    {
        avl_remove(globalData.ifTree, pIf);
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

/**
 * @func: 
 * @description: 根据端口索引进行排序
 * @param {void} *data1
 * @param {void} *data2
 * @return {*}
 * @author: Mr.Xiong
 */
int32_t if_index_cmp(void *data1, void *data2)
{
    tIfInfo *ifInfoData1 = NULL;
    tIfInfo *ifInfoData2 = NULL;
    int32_t ret = 0;

    if ((NULL == data1) || (NULL == data2))
    {
        return 0;
    }

    ifInfoData1 = (tIfInfo *)data1;
    ifInfoData2 = (tIfInfo *)data2;

    if (ifInfoData1->ifIndex > ifInfoData2->ifIndex)
    {
        ret =  1;
    }
    else if (ifInfoData1->ifIndex < ifInfoData2->ifIndex)
    {
        ret =  -1;
    }
    else
    {
        ret = 0;
    }

    return ret;
}

int main(int argc, char const *argv[])
{
    tIfInfo *pIf = NULL;

    /*创建树*/
    if (avl_create(&globalData.ifTree, 0, if_index_cmp) < 0)
    {
        printf("Create IfTree failed.");
        return ERROR;
    }

    for(int i = 0; i < 50; i++)
    {
        pIf = (tIfInfo *)malloc(sizeof(tIfInfo));
        if (NULL == pIf)
        {
            printf("malloc interface node failed.");
            return ERROR;
        }

        pIf->ifIndex = rand()%200; 
        pIf->linkState = rand()%2 + 1;

        ifHandle(pIf, IF_CREATE);
    }

    printf("the Tree has %d node.", globalData.ifTree->count);

    /*遍历操作*/
    ifReadAll();
    /*删除操作*/
    ifDelAllNode();

    /*释放链表*/
    if (0 == AVL_COUNT(globalData.ifTree))
    {
        avl_tree_free(&globalData.ifTree, NULL);
        printf("delete Tree\n");
        globalData.ifTree = NULL;
    }

    return 0;
}
