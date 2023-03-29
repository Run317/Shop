#include "../inc/linklist.h"

// 链表初始化
node *linklistInit(void)
{
    node *p = malloc(sizeof(node));
    if (p == NULL)
    {
        perror("linklistInit failed:");
        return NULL;
    }

    // 清空数据域
    bzero(&p->datatype, sizeof(info));
    // 设置指针指向
    p->next = p;
    p->prev = p;

    return p;
}

// 链表头插
void linklistAdd(node *head, info newdata)
{
    // 新增数据节点
    node *new = linklistInit();
    // 数据域赋值
    new->datatype = newdata;

    // 修改新节点指针指向
    new->next = head->next;
    new->prev = head;
    // 修改链表指针指向
    head->next->prev = new;
    head->next = new;
}

// 遍历链表
void linklistShow(node *head)
{
    node *p = head->next;
    while (p != head)
    {
        printf("%s %s %s %s %s %s %s %s %d %s\n", p->datatype.name, p->datatype.data, p->datatype.price, p->datatype.address, p->datatype.netContent, p->datatype.ID, p->datatype.picPath, p->datatype.type, p->datatype.shopnum, p->datatype.finder);
        p = p->next;
    }
}

// 商品分类
int productCategory(node *head, node *destHead, char *name)
{
    int productNum = 0;
    // 从商品链表中遍历查找该类型节点
    node *pos = head->next;
    // 将节点插入目标头节点
    while (pos != head)
    {
        if (strcmp(name, pos->datatype.type) == 0)
        {
            linklistAdd(destHead, pos->datatype);
            productNum++;
        }
        pos = pos->next;
    }

    return productNum;
}

// 节点查找
_Bool linklistFind(node *head, node *dest)
{
    for (node *pos = head->next; pos != head; pos = pos->next)
    {
        if (strcmp(pos->datatype.name, dest->datatype.name) == 0)
            return 0;
    }
    return 1;
}

// 购买数量
void payNumadd(node *head, node *dest)
{
    for (node *pos = head->next; pos != head; pos = pos->next)
    {
        if (strcmp(pos->datatype.name, dest->datatype.name) == 0)
        {
            pos->datatype.shopnum++;
            return;
        }
    }
    return;
}
// 减少商品
void payNumsub(node *head, node *dest)
{
    for (node *pos = head->next; pos != head; pos = pos->next)
    {
        if (strcmp(pos->datatype.name, dest->datatype.name) == 0)
        {
            // 如果该商品只有一个，从链表中删除该商品
            if (pos->datatype.shopnum == 1)
            {
                pos->prev->next = pos->next;
                pos->next->prev = pos->prev;
                free(pos);
                break;
            }
            pos->datatype.shopnum--;
            return;
        }
    }
    return;
}
