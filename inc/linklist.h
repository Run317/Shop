#ifndef __LINKLIST_H__
#define __LINKLIST_H__

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

// ������Ʒ��Ϣ�ṹ��
typedef struct CommodityInfo
{
    char name[256];       // ��Ʒ��
    char data[256];       // ��������
    char price[256];      // �۸�
    char address[256];    // ������ַ
    char netContent[256]; // ������
    char ID[256];         // ��Ʒ��׼��
    char picPath[256];    // ͼƬ·��
    char type[256];       // ��Ʒ����
    int shopnum;          // ��������
    char finder[256];     // ������
} info;

// ����˫��ѭ��������ƽڵ�
typedef struct link_list
{
    // ������
    info datatype;
    // ָ����
    struct link_list *prev;
    struct link_list *next;
} node;

// ��ʼ����Ʒ��Ϣ,����Ʒ��Ϣ�����ļ�
void initInfo(void);
// ������д��txt�ĵ�
int inputInfo(node *head);

// �����ʼ��
node *linklistInit(void);
// ����ͷ��
void linklistAdd(node *head, info newdata);
// ��������
void linklistShow(node *head);
// �ڵ����
_Bool linklistFind(node *head, node *dest);
// ��ӵ����ﳵ
void addToshopcart(node *head, node *new);
// ��������
void payNumadd(node *head, node *dest);
// ����
void checkout(node *head);
// ������Ʒ
void payNumsub(node *head, node *dest);

// ��Ʒ����
int productCategory(node *head, node *destHead, char *name);

#endif