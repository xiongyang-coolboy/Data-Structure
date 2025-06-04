#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct 
{
    struct list_head ifNodeList;
    int ifNodeCount;
    /* data */
}tGlobalInfo;

typedef struct 
{
    struct list_head ifNode;
    int ifIndex;
#define IF_PHY_TYPE_FE 1
#define IF_PHY_TYPE_GE 2
#define IF_PHY_TYPE_xGE 3
    int ifPhyType;
}tIfInfo;

#define ERROR 0
#define OK 1
#define IF_INDEX_BASE 1000

tGlobalInfo tIfGlobalInfo = {0};

int ifInfoAdd(int ifIndex, int ifPhyType)
{
    tIfInfo *pIfInfo = NULL;

    /*申请结点*/
    pIfInfo = (tIfInfo *)malloc(sizeof(tIfInfo));
    if (NULL == pIfInfo)
    {
        return ERROR;
    }

    memset(pIfInfo, 0, sizeof(tIfInfo));
    pIfInfo->ifIndex = ifIndex;
    pIfInfo->ifPhyType = ifPhyType;

    /* 加入链表 */
    list_add_tail(&pIfInfo->ifNode, &tIfGlobalInfo.ifNodeList);
    tIfGlobalInfo.ifNodeCount++;

    return OK;    
}

 tIfInfo *ifInfoSearch(int ifIndex)
{
    tIfInfo *pIfInfo = NULL;

    if (true == list_empty(&tIfGlobalInfo.ifNodeList))
    {
        return NULL;
    }


    list_for_each_entry(pIfInfo, &tIfGlobalInfo.ifNodeList, ifNode)
    {
        if (ifIndex == pIfInfo->ifIndex)
        {
            return pIfInfo;
        }
    }

    return NULL;
}

int ifInfoDel(int ifIndex)
{
    tIfInfo *pIfInfo = NULL;

    if (true == list_empty(&tIfGlobalInfo.ifNodeList))
    {
        return OK;
    }

    pIfInfo = ifInfoSearch(ifIndex);
    if (NULL == pIfInfo)
    {
        return OK;
    }

    /* 解链 */
    list_del(&pIfInfo->ifNode);
    /*释放内存*/
    free(pIfInfo);
    pIfInfo = NULL;
    tIfGlobalInfo.ifNodeCount--;

    return OK;
}

void infoPrint(tIfInfo *pIfInfo)
{
    if (NULL == pIfInfo)
    {
        return;
    }

    printf("ifindex = %d, ifPhytype = %d\n", pIfInfo->ifIndex, pIfInfo->ifPhyType);
}


int main(int argc, char const *argv[])
{
    int ifIndex = 0;
    int ifPhyType = 0;
    int num = 0;
    int ret = OK;
    tIfInfo *pIfInfo = NULL;
    tIfInfo *pIfInfoNext = NULL;
    /*初始化链表*/
    INIT_LIST_HEAD(&(tIfGlobalInfo.ifNodeList));

    /* 添加数据 */
    for ( num = 0; num < 50; num++)
    {
        ifIndex = IF_INDEX_BASE + num;
        ifPhyType = rand() % 3 + 1;
        
        ret = ifInfoAdd(ifIndex, ifPhyType);
        if (OK != ret)
        {
            printf("add ifNode error, ifIndex = %d \n", ifIndex);
        }  
    }

    /*查索引1005的表项*/
    pIfInfo = ifInfoSearch(1005);
    infoPrint(pIfInfo);

    /*删除索引1005*/
    ret = ifInfoDel(1005);
    if (ret != OK)
    {
        printf("del failed");
    }

    /* 遍历删除结点 */
    list_for_each_entry_safe(pIfInfo, pIfInfoNext, &tIfGlobalInfo.ifNodeList, ifNode)
    {
        infoPrint(pIfInfo);
        /* 解链 */
        list_del(&pIfInfo->ifNode);
        /*释放内存*/
        free(pIfInfo);
        pIfInfo = NULL;
        tIfGlobalInfo.ifNodeCount--;
    }
    
    
    /* code */
    return 0;
}