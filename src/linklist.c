#include "../inc/linklist.h"

// �����ʼ��
node *linklistInit(void)
{
    node *p = malloc(sizeof(node));
    if (p == NULL)
    {
        perror("linklistInit failed:");
        return NULL;
    }

    // ���������
    bzero(&p->datatype, sizeof(info));
    // ����ָ��ָ��
    p->next = p;
    p->prev = p;

    return p;
}

// ����ͷ��
void linklistAdd(node *head, info newdata)
{
    // �������ݽڵ�
    node *new = linklistInit();
    // ������ֵ
    new->datatype = newdata;

    // �޸��½ڵ�ָ��ָ��
    new->next = head->next;
    new->prev = head;
    // �޸�����ָ��ָ��
    head->next->prev = new;
    head->next = new;
}

// ��������
void linklistShow(node *head)
{
    node *p = head->next;
    while (p != head)
    {
        printf("%s %s %s %s %s %s %s %s %d %s\n", p->datatype.name, p->datatype.data, p->datatype.price, p->datatype.address, p->datatype.netContent, p->datatype.ID, p->datatype.picPath, p->datatype.type, p->datatype.shopnum, p->datatype.finder);
        p = p->next;
    }
}

// ��Ʒ����
int productCategory(node *head, node *destHead, char *name)
{
    int productNum = 0;
    // ����Ʒ�����б������Ҹ����ͽڵ�
    node *pos = head->next;
    // ���ڵ����Ŀ��ͷ�ڵ�
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

// �ڵ����
_Bool linklistFind(node *head, node *dest)
{
    for (node *pos = head->next; pos != head; pos = pos->next)
    {
        if (strcmp(pos->datatype.name, dest->datatype.name) == 0)
            return 0;
    }
    return 1;
}

// ��������
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
// ������Ʒ
void payNumsub(node *head, node *dest)
{
    for (node *pos = head->next; pos != head; pos = pos->next)
    {
        if (strcmp(pos->datatype.name, dest->datatype.name) == 0)
        {
            // �������Ʒֻ��һ������������ɾ������Ʒ
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
