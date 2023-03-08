#include "../inc/linklist.h"
#include "../inc/menu.h"
// #include "linklist.h"

// 初始化商品信息,将商品信息导入文件
void initInfo(void)
{
    // 1.初始化商品信息
    info cola = {
        .name = "可乐",
        .data = "2023年1月28日",
        .price = "3",
        .address = "湖北荆州",
        .netContent = "350ml",
        .ID = "GB/T20980",
        .type = "drink",
        .picPath = "../pic/cola.bmp",
        .shopnum = 0,
        .finder = "kele"};
    info aoliao = {
        .name = "奥利奥",
        .data = "2023年1月13日",
        .price = "8",
        .address = "江苏苏州",
        .netContent = "500g",
        .ID = "GB/T15476",
        .type = "food",
        .picPath = "../pic/aoliao.bmp",
        .shopnum = 0,
        .finder = "aoliao"};
    info dove = {
        .name = "德芙巧克力",
        .data = "2022年12月25日",
        .price = "7.5",
        .address = "云南保山",
        .netContent = "500g",
        .ID = "GB/T27158",
        .type = "food",
        .picPath = "../pic/dove.bmp",
        .shopnum = 0,
        .finder = "defu"};
    info lays = {
        .name = "乐事薯片",
        .data = "2022年10月15日",
        .price = "6.5",
        .address = "湖南长沙",
        .netContent = "250g",
        .ID = "GB/T12138",
        .type = "food",
        .picPath = "../pic/lays.bmp",
        .shopnum = 0,
        .finder = "leshi"};
    info guazi = {
        .name = "瓜子",
        .data = "2023年2月6日",
        .price = "4",
        .address = "河南郑州",
        .netContent = "360g",
        .ID = "GB/T27149",
        .type = "food",
        .picPath = "../pic/guazi.bmp",
        .shopnum = 0,
        .finder = "guazi"};
    info ksf = {
        .name = "康师傅方便面",
        .data = "2022年8月23日",
        .price = "5.5",
        .address = "河北石家庄",
        .netContent = "650g",
        .ID = "GB/T47819",
        .type = "food",
        .picPath = "../pic/ksf.bmp",
        .shopnum = 0,
        .finder = "fangbianmian"};

    // 2.将文件写入文档
    // 2.1打开本地文档
    FILE *fp = fopen("../txt/commodity.txt", "w");
    if (fp == NULL)
    {
        perror("initInfo failed:");
        return;
    }
    // 2.2 将商品信息写入文档
    fprintf(fp, "%s %s %s %s %s %s %s %s %d %s\n", cola.name, cola.data, cola.price, cola.address, cola.netContent, cola.ID, cola.picPath, cola.type, cola.shopnum, cola.finder);
    fprintf(fp, "%s %s %s %s %s %s %s %s %d %s\n", aoliao.name, aoliao.data, aoliao.price, aoliao.address, aoliao.netContent, aoliao.ID, aoliao.picPath, aoliao.type, aoliao.shopnum, aoliao.finder);
    fprintf(fp, "%s %s %s %s %s %s %s %s %d %s\n", dove.name, dove.data, dove.price, dove.address, dove.netContent, dove.ID, dove.picPath, dove.type, dove.shopnum, dove.finder);
    fprintf(fp, "%s %s %s %s %s %s %s %s %d %s\n", guazi.name, guazi.data, guazi.price, guazi.address, guazi.netContent, guazi.ID, guazi.picPath, guazi.type, guazi.shopnum, guazi.finder);
    fprintf(fp, "%s %s %s %s %s %s %s %s %d %s\n", lays.name, lays.data, lays.price, lays.address, lays.netContent, lays.ID, lays.picPath, lays.type, lays.shopnum, lays.finder);
    fprintf(fp, "%s %s %s %s %s %s %s %s %d %s\n", ksf.name, ksf.data, ksf.price, ksf.address, ksf.netContent, ksf.ID, ksf.picPath, ksf.type, ksf.shopnum, ksf.finder);

    // 2.3关闭文件
    fclose(fp);
    return;
}

// 将数据从文档中读取，写入链表
int inputInfo(node *head)
{
    // 记录商品数量
    int num = 0;
    // 读写方式打开目标txt文件
    FILE *fp = fopen("../txt/commodity.txt", "r+");
    if (fp == NULL)
    {
        perror("inputInfo failed:");
        return 0;
    }

    // 暂存写入数据
    info temp;

    // 将文件数据存储进入链表
    while (!feof(fp))
    {
        // 清空缓存
        bzero(&temp, sizeof(temp));
        // 从文件中读取数据
        fscanf(fp, "%s %s %s %s %s %s %s %s %d %s\n", temp.name, temp.data, temp.price, temp.address, temp.netContent, temp.ID, temp.picPath, temp.type, &temp.shopnum, temp.finder);
        // 数据插入链表中
        printf("%s %s %s %s %s %s %s %s %d %s\n", temp.name, temp.data, temp.price, temp.address, temp.netContent, temp.ID, temp.picPath, temp.type, temp.shopnum, temp.finder);
        linklistAdd(head, temp);
        // 每插入一个节点，商品数量加一
        num++;
    }
    // 遍历链表，显示商品数量
    linklistShow(head);
    printf("商品数量:%d\n", num);

    return num;
}

// 结账
void checkout(node *head)
{
    // 打开购物信息文档
    FILE *fp = fopen("../txt/purchaseinfo.txt", "a+");
    if (fp == NULL)
    {
        perror("initInfo failed:");
        return;
    }
    fprintf(fp, "商品名\t\t 价格\t 数目\n");
    // 将购物信息写入文档
    for (node *pos = head->next; pos != head; pos = pos->next)
    {
        fprintf(fp, "%s\t %s \t %d\n", pos->datatype.name, pos->datatype.price, pos->datatype.shopnum);
        printf("%s\t %s \t %d\n", pos->datatype.name, pos->datatype.price, pos->datatype.shopnum);
    }
    fseek(fp, 0, SEEK_SET);
    // 清空链表
    node *pos = head;
    while (pos->next != head)
    {
        node *del = pos->next;

        pos->next = del->next;
        del->next->prev = pos;
        free(del);
    }
    fclose(fp);
    return;
}
char findname[256];
// 查找商品
int keyboard(int td)
{
    // 查找字符串
    bzero(findname, sizeof(findname));
    int x1, y1;
    while (1)
    {
        touch(&x1, &y1, td);
        printf("find:(%d,%d)\n", x1, y1);
        if (x1 > 15 && x1 < 70 && y1 > 242 && y1 < 310)
            strcat(findname, "q");
        if (x1 > 75 && x1 < 130 && y1 > 242 && y1 < 310)
            strcat(findname, "w");
        if (x1 > 136 && x1 < 190 && y1 > 242 && y1 < 310)
            strcat(findname, "e");
        if (x1 > 198 && x1 < 254 && y1 > 242 && y1 < 310)
            strcat(findname, "r");
        if (x1 > 260 && x1 < 315 && y1 > 242 && y1 < 310)
            strcat(findname, "t");
        if (x1 > 322 && x1 < 376 && y1 > 242 && y1 < 310)
            strcat(findname, "y");
        if (x1 > 384 && x1 < 437 && y1 > 242 && y1 < 310)
            strcat(findname, "u");
        if (x1 > 445 && x1 < 500 && y1 > 242 && y1 < 310)
            strcat(findname, "i");
        if (x1 > 508 && x1 < 561 && y1 > 242 && y1 < 310)
            strcat(findname, "o");
        if (x1 > 570 && x1 < 623 && y1 > 242 && y1 < 310)
            strcat(findname, "p");
        //=====
        if (x1 > 15 && x1 < 70 && y1 > 327 && y1 < 398)
            strcat(findname, "a");
        if (x1 > 75 && x1 < 130 && y1 > 327 && y1 < 398)
            strcat(findname, "s");
        if (x1 > 136 && x1 < 190 && y1 > 327 && y1 < 398)
            strcat(findname, "d");
        if (x1 > 198 && x1 < 254 && y1 > 327 && y1 < 398)
            strcat(findname, "f");
        if (x1 > 260 && x1 < 315 && y1 > 327 && y1 < 398)
            strcat(findname, "g");
        if (x1 > 322 && x1 < 376 && y1 > 327 && y1 < 398)
            strcat(findname, "h");
        if (x1 > 384 && x1 < 437 && y1 > 327 && y1 < 398)
            strcat(findname, "j");
        if (x1 > 445 && x1 < 500 && y1 > 327 && y1 < 398)
            strcat(findname, "k");
        if (x1 > 508 && x1 < 561 && y1 > 327 && y1 < 398)
            strcat(findname, "l");
        if (x1 > 570 && x1 < 623 && y1 > 327 && y1 < 398)
            return 0;
        //=====
        if (x1 > 15 && x1 < 99 && y1 > 415 && y1 < 485)
            return 1;
        if (x1 > 106 && x1 < 161 && y1 > 415 && y1 < 485)
            strcat(findname, "z");
        if (x1 > 168 && x1 < 222 && y1 > 415 && y1 < 485)
            strcat(findname, "x");
        if (x1 > 230 && x1 < 283 && y1 > 415 && y1 < 485)
            strcat(findname, "c");
        if (x1 > 290 && x1 < 345 && y1 > 415 && y1 < 485)
            strcat(findname, "v");
        if (x1 > 352 && x1 < 407 && y1 > 415 && y1 < 485)
            strcat(findname, "b");
        if (x1 > 415 && x1 < 470 && y1 > 415 && y1 < 485)
            strcat(findname, "n");
        if (x1 > 477 && x1 < 530 && y1 > 415 && y1 < 485)
            strcat(findname, "m");
        if (x1 > 538 && x1 < 623 && y1 > 415 && y1 < 485)
        {
            int len = strlen(findname);
            printf("len:%d\n", len);
            len--;
            findname[len] = '\0';
        }
        printf("%s\n", findname);
    }
}
