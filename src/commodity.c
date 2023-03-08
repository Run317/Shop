#include "../inc/linklist.h"
#include "../inc/menu.h"
// #include "linklist.h"

// ��ʼ����Ʒ��Ϣ,����Ʒ��Ϣ�����ļ�
void initInfo(void)
{
    // 1.��ʼ����Ʒ��Ϣ
    info cola = {
        .name = "����",
        .data = "2023��1��28��",
        .price = "3",
        .address = "��������",
        .netContent = "350ml",
        .ID = "GB/T20980",
        .type = "drink",
        .picPath = "../pic/cola.bmp",
        .shopnum = 0,
        .finder = "kele"};
    info aoliao = {
        .name = "������",
        .data = "2023��1��13��",
        .price = "8",
        .address = "��������",
        .netContent = "500g",
        .ID = "GB/T15476",
        .type = "food",
        .picPath = "../pic/aoliao.bmp",
        .shopnum = 0,
        .finder = "aoliao"};
    info dove = {
        .name = "��ܽ�ɿ���",
        .data = "2022��12��25��",
        .price = "7.5",
        .address = "���ϱ�ɽ",
        .netContent = "500g",
        .ID = "GB/T27158",
        .type = "food",
        .picPath = "../pic/dove.bmp",
        .shopnum = 0,
        .finder = "defu"};
    info lays = {
        .name = "������Ƭ",
        .data = "2022��10��15��",
        .price = "6.5",
        .address = "���ϳ�ɳ",
        .netContent = "250g",
        .ID = "GB/T12138",
        .type = "food",
        .picPath = "../pic/lays.bmp",
        .shopnum = 0,
        .finder = "leshi"};
    info guazi = {
        .name = "����",
        .data = "2023��2��6��",
        .price = "4",
        .address = "����֣��",
        .netContent = "360g",
        .ID = "GB/T27149",
        .type = "food",
        .picPath = "../pic/guazi.bmp",
        .shopnum = 0,
        .finder = "guazi"};
    info ksf = {
        .name = "��ʦ��������",
        .data = "2022��8��23��",
        .price = "5.5",
        .address = "�ӱ�ʯ��ׯ",
        .netContent = "650g",
        .ID = "GB/T47819",
        .type = "food",
        .picPath = "../pic/ksf.bmp",
        .shopnum = 0,
        .finder = "fangbianmian"};

    // 2.���ļ�д���ĵ�
    // 2.1�򿪱����ĵ�
    FILE *fp = fopen("../txt/commodity.txt", "w");
    if (fp == NULL)
    {
        perror("initInfo failed:");
        return;
    }
    // 2.2 ����Ʒ��Ϣд���ĵ�
    fprintf(fp, "%s %s %s %s %s %s %s %s %d %s\n", cola.name, cola.data, cola.price, cola.address, cola.netContent, cola.ID, cola.picPath, cola.type, cola.shopnum, cola.finder);
    fprintf(fp, "%s %s %s %s %s %s %s %s %d %s\n", aoliao.name, aoliao.data, aoliao.price, aoliao.address, aoliao.netContent, aoliao.ID, aoliao.picPath, aoliao.type, aoliao.shopnum, aoliao.finder);
    fprintf(fp, "%s %s %s %s %s %s %s %s %d %s\n", dove.name, dove.data, dove.price, dove.address, dove.netContent, dove.ID, dove.picPath, dove.type, dove.shopnum, dove.finder);
    fprintf(fp, "%s %s %s %s %s %s %s %s %d %s\n", guazi.name, guazi.data, guazi.price, guazi.address, guazi.netContent, guazi.ID, guazi.picPath, guazi.type, guazi.shopnum, guazi.finder);
    fprintf(fp, "%s %s %s %s %s %s %s %s %d %s\n", lays.name, lays.data, lays.price, lays.address, lays.netContent, lays.ID, lays.picPath, lays.type, lays.shopnum, lays.finder);
    fprintf(fp, "%s %s %s %s %s %s %s %s %d %s\n", ksf.name, ksf.data, ksf.price, ksf.address, ksf.netContent, ksf.ID, ksf.picPath, ksf.type, ksf.shopnum, ksf.finder);

    // 2.3�ر��ļ�
    fclose(fp);
    return;
}

// �����ݴ��ĵ��ж�ȡ��д������
int inputInfo(node *head)
{
    // ��¼��Ʒ����
    int num = 0;
    // ��д��ʽ��Ŀ��txt�ļ�
    FILE *fp = fopen("../txt/commodity.txt", "r+");
    if (fp == NULL)
    {
        perror("inputInfo failed:");
        return 0;
    }

    // �ݴ�д������
    info temp;

    // ���ļ����ݴ洢��������
    while (!feof(fp))
    {
        // ��ջ���
        bzero(&temp, sizeof(temp));
        // ���ļ��ж�ȡ����
        fscanf(fp, "%s %s %s %s %s %s %s %s %d %s\n", temp.name, temp.data, temp.price, temp.address, temp.netContent, temp.ID, temp.picPath, temp.type, &temp.shopnum, temp.finder);
        // ���ݲ���������
        printf("%s %s %s %s %s %s %s %s %d %s\n", temp.name, temp.data, temp.price, temp.address, temp.netContent, temp.ID, temp.picPath, temp.type, temp.shopnum, temp.finder);
        linklistAdd(head, temp);
        // ÿ����һ���ڵ㣬��Ʒ������һ
        num++;
    }
    // ����������ʾ��Ʒ����
    linklistShow(head);
    printf("��Ʒ����:%d\n", num);

    return num;
}

// ����
void checkout(node *head)
{
    // �򿪹�����Ϣ�ĵ�
    FILE *fp = fopen("../txt/purchaseinfo.txt", "a+");
    if (fp == NULL)
    {
        perror("initInfo failed:");
        return;
    }
    fprintf(fp, "��Ʒ��\t\t �۸�\t ��Ŀ\n");
    // ��������Ϣд���ĵ�
    for (node *pos = head->next; pos != head; pos = pos->next)
    {
        fprintf(fp, "%s\t %s \t %d\n", pos->datatype.name, pos->datatype.price, pos->datatype.shopnum);
        printf("%s\t %s \t %d\n", pos->datatype.name, pos->datatype.price, pos->datatype.shopnum);
    }
    fseek(fp, 0, SEEK_SET);
    // �������
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
// ������Ʒ
int keyboard(int td)
{
    // �����ַ���
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
