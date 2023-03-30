#ifndef __LINKLIST_H__
#define __LINKLIST_H__

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

// 定义商品信息结构体
typedef struct CommodityInfo
{
    char name[256];       // 商品名
    char data[256];       // 生产日期
    char price[256];      // 价格
    char address[256];    // 生产地址
    char netContent[256]; // 净含量
    char ID[256];         // 产品标准号
    char picPath[256];    // 图片路径
    char type[256];       // 商品类型
    int shopnum;          // 购买数量
    char finder[256];     // 搜索名
} info;

// 定义双向循环链表，设计节点
typedef struct link_list
{
    // 数据域
    info datatype;
    // 指针域
    struct link_list *prev;
    struct link_list *next;
} node;

// 初始化商品信息,将商品信息导入文件
void initInfo(void);
// 将数据写入txt文档
int inputInfo(node *head);

// 链表初始化
node *linklistInit(void);
// 链表头插
void linklistAdd(node *head, info newdata);
// 遍历链表
void linklistShow(node *head);
// 节点查找
_Bool linklistFind(node *head, node *dest);
// 添加到购物车
void addToshopcart(node *head, node *new);
// 购买数量
void payNumadd(node *head, node *dest);
// 结账
void checkout(node *head);
// 减少商品
void payNumsub(node *head, node *dest);

// 商品分类
int productCategory(node *head, node *destHead, char *name);

#endif