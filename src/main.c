#include "../inc/menu.h"
#include "../inc/linklist.h"
#include "../inc/font.h"

extern char findname[256];

int main(int argc, char const *argv[])
{
    // �򿪴�����
    int td = open("/dev/input/event0", O_RDWR);
    if (td == -1)
    {
        perror("/dev/input/event0");
        return 0;
    }

    // ��ʼ��lcd
    LCD *lcd = lcd_init();
    // ���ֿ�
    Init_Font();
    printf("��ʼ��LCD\n");
    // ��ʼ����Ʒͷ�ڵ�
    node *commodityHead = linklistInit();
    printf("��ʼ����Ʒͷ�ڵ�\n");
    // ��ʼ����Ʒ��Ϣ
    initInfo();
    printf("��ʼ����Ʒ��Ϣ\n");
    // ���ļ���Ϣ��������
    int commodityNum = inputInfo(commodityHead);
    printf("���ļ���Ϣ��������\n");

    // ��ʾ���˵�
    ShowMenu(lcd);
    printf("��ʾ���˵�\n");

    // ��ʼ����ƷͷƤ�ڵ�
    // ��ʳ
    // ��ʼ����ʳͷ�ڵ�
    node *foodHead = linklistInit();
    char food[128] = "food";
    // ����Ʒ���࣬������Ϊfood�Ľڵ�������foodhead
    int foodNum = productCategory(commodityHead, foodHead, food);
    printf("\n");
    printf("food head:\n");
    linklistShow(foodHead);

    // ����
    node *drinkHead = linklistInit();
    char drink[128] = "drink";
    // ����Ʒ���࣬������Ϊdrink�Ľڵ�������drinkhead
    int drinkNum = productCategory(commodityHead, drinkHead, drink);
    printf("\n");
    printf("drink head:\n");
    linklistShow(drinkHead);

    // ��ʼ�����ﳵͷ�ڵ�
    node *shopcartHead = linklistInit();
    // ��ʼ������ҳ��ͷ�ڵ�
    node *findPagehead = linklistInit();

    // ��ʾ���
    showAdd(lcd);
    sleep(5); // �����ʾ����

    // ��ʾ��Ʒ����
    node *pos = commodityHead->next;
    node *commodityPos = commodityHead->next;
    node *foodPos = foodHead->next;
    node *drinkPos = drinkHead->next;
    showBmp(pos, lcd, commodityHead);
    //**************����***********
    // while (pos != commodityHead)
    // {
    //     showBmp(pos, lcd, commodityHead);
    //     sleep(1);
    //     pos = pos->next->next;
    // }
    //********************************************

    // �����¼�x��y����
    int x, y;

    while (1)
    {

        touch(&x, &y, td);
        printf("(%d,%d)\n", x, y);
        //*********************��ʾ��Ʒ*********************
        // ������Ʒ
        if (x > 640 && x < 690 && y > 100 && y < 180)
        {
            pos = commodityHead->next;
            commodityPos = commodityHead->next;
            showBmp(pos, lcd, commodityHead);
        }
        // ��ʳ��
        if (x > 690 && x < 740 && y > 100 && y < 180)
        {
            pos = foodHead->next;
            foodPos = foodHead->next;
            showBmp(pos, lcd, foodHead);
        }
        // ������
        if (x > 740 && x < 790 && y > 100 && y < 180)
        {
            pos = drinkHead->next;
            drinkPos = drinkHead->next;
            showBmp(pos, lcd, drinkHead);
        }
        //********************************************************

        //*********************���ﳵ*********************
        if (x > 640 && x < 790 && y > 300 && y < 380)
        {
            pos = shopcartHead;
            shopCart(shopcartHead, lcd);
            linklistShow(shopcartHead);
        }

        //********************************************************

        //*********************��ҳ*********************

        if (x > 640 && x < 715 && y > 400 && y < 470) // ��ǰ��ҳ
        {
            // �ж����ĸ��ڵ�
            if (pos == commodityHead->next)
            {

                if (commodityPos->prev == commodityHead)
                {
                    if (commodityNum % 2)
                    {
                        commodityPos = commodityHead->prev;
                        showBmp(commodityPos, lcd, commodityHead);
                    }
                    else
                    {
                        commodityPos = commodityHead->prev->prev;
                        showBmp(commodityPos, lcd, commodityHead);
                    }
                }
                else
                {
                    commodityPos = commodityPos->prev->prev;
                    showBmp(commodityPos, lcd, commodityHead);
                }
            }
            if (pos == foodHead->next)
            {
                if (foodPos->prev == foodHead)
                {
                    if (foodNum % 2)
                    {
                        foodPos = foodHead->prev;
                        showBmp(foodPos, lcd, foodHead);
                    }
                    else
                    {
                        foodPos = foodHead->prev->prev;
                        showBmp(foodPos, lcd, foodHead);
                    }
                }
                else
                {
                    foodPos = foodPos->prev->prev;
                    showBmp(foodPos, lcd, foodHead);
                }
            }
            if (pos == drinkHead->next)
            {
                if (drinkPos->prev == drinkHead)
                {
                    if (drinkNum % 2)
                    {
                        drinkPos = drinkHead->prev;
                        showBmp(drinkPos, lcd, drinkHead);
                    }
                    else
                    {
                        drinkPos = drinkHead->prev->prev;
                        showBmp(drinkPos, lcd, drinkHead);
                    }
                }
                else
                {
                    drinkPos = drinkPos->prev->prev;
                    showBmp(drinkPos, lcd, drinkHead);
                }
            }
        }
        if (x > 715 && x < 790 && y > 400 && y < 470) // ���ҳ
        {
            if (pos == commodityHead->next)
            {
                if (commodityPos->next->next == commodityHead)
                {
                    commodityPos = commodityHead->next;
                    showBmp(commodityPos, lcd, commodityHead);
                }
                else
                {
                    commodityPos = commodityPos->next->next;
                    showBmp(commodityPos, lcd, commodityHead);
                }
            }
            if (pos == foodHead->next)
            {
                if (foodPos->next->next == foodHead)
                {
                    foodPos = foodHead->next;
                    showBmp(foodPos, lcd, foodHead);
                }
                else
                {
                    foodPos = foodPos->next->next;
                    showBmp(foodPos, lcd, foodHead);
                }
            }
            if (pos == drinkHead->next)
            {
                if (drinkPos->next->next == drinkHead)
                {
                    drinkPos = drinkHead->next;
                    showBmp(drinkPos, lcd, drinkHead);
                }
                else
                {
                    drinkPos = drinkPos->next->next;
                    showBmp(drinkPos, lcd, drinkHead);
                }
            }
        }
        //********************************************************

        //*********************��ʾ��Ʒ��ϸ��Ϣ*********************
        if (x > 10 && x < 320 && y > 10 && y < 370)
        {
            printf("��ʾ��Ʒ��ϸ��Ϣ\n");
            if (pos == commodityHead->next)
                showDitail(commodityPos, lcd);
            if (pos == foodHead->next)
                showDitail(foodPos, lcd);
            if (pos == drinkHead->next)
                showDitail(drinkPos, lcd);
        }
        if (x > 320 && x < 630 && y > 10 && y < 370)
        {
            printf("��ʾ��Ʒ��ϸ��Ϣ\n");
            if (pos == commodityHead->next)
                showDitail(commodityPos->next, lcd);
            if (pos == foodHead->next)
                showDitail(foodPos->next, lcd);
            if (pos == drinkHead->next)
                showDitail(drinkPos->next, lcd);
        }
        //********************************************************

        //*********************���ӣ�������Ʒ����*********************
        // ������Ʒ����
        if (x > 270 && x < 320 && y > 370 && y < 470)
        {
            printf("������Ʒ\n");
            if (pos == commodityHead->next)
            {
                if (linklistFind(shopcartHead, commodityPos))
                {
                    linklistAdd(shopcartHead, commodityPos->datatype);
                }
                payNumadd(shopcartHead, commodityPos);
                linklistShow(shopcartHead);
            }
            if (pos == foodHead->next)
            {
                if (linklistFind(shopcartHead, foodPos))
                {
                    linklistAdd(shopcartHead, foodPos->datatype);
                }
                payNumadd(shopcartHead, foodPos);
                linklistShow(shopcartHead);
            }
            if (pos == drinkHead->next)
            {
                if (linklistFind(shopcartHead, drinkPos))
                {
                    linklistAdd(shopcartHead, drinkPos->datatype);
                }
                payNumadd(shopcartHead, drinkPos);
                linklistShow(shopcartHead);
            }
            if (pos == findPagehead)
            {
                if (linklistFind(shopcartHead, findPagehead->next))
                {
                    linklistAdd(shopcartHead, findPagehead->next->datatype);
                }
                payNumadd(shopcartHead, findPagehead->next);
                linklistShow(shopcartHead);
            }
        }
        if (x > 580 && x < 630 && y > 370 && y < 470)
        {
            printf("������Ʒ\n");
            if (pos == commodityHead->next)
            {
                if (linklistFind(shopcartHead, commodityPos->next))
                {
                    linklistAdd(shopcartHead, commodityPos->next->datatype);
                }
                payNumadd(shopcartHead, commodityPos->next);
                linklistShow(shopcartHead);
            }
            if (pos == foodHead->next)
            {
                if (linklistFind(shopcartHead, foodPos->next))
                {
                    linklistAdd(shopcartHead, foodPos->next->datatype);
                }
                payNumadd(shopcartHead, foodPos->next);
                linklistShow(shopcartHead);
            }
            if (pos == drinkHead->next)
            {
                if (linklistFind(shopcartHead, drinkPos->next))
                {
                    linklistAdd(shopcartHead, drinkPos->next->datatype);
                }
                payNumadd(shopcartHead, drinkPos->next);
                linklistShow(shopcartHead);
            }
        }
        // ������Ʒ����
        if (x > 170 && x < 220 && y > 370 && y < 470)
        {
            if (pos == commodityHead->next)
            {
                if (linklistFind(shopcartHead, commodityPos) == 0)
                {
                    payNumsub(shopcartHead, commodityPos);
                }
                else
                    printf("���ﳵ��û�и���Ʒ\n");
                linklistShow(shopcartHead);
            }
            if (pos == foodHead->next)
            {
                if (linklistFind(shopcartHead, foodPos) == 0)
                {
                    payNumsub(shopcartHead, foodPos);
                }
                else
                    printf("���ﳵ��û�и���Ʒ\n");
                linklistShow(shopcartHead);
            }
            if (pos == drinkPos->next)
            {
                if (linklistFind(shopcartHead, drinkPos) == 0)
                {
                    payNumsub(shopcartHead, drinkPos);
                }
                else
                    printf("���ﳵ��û�и���Ʒ\n");
                linklistShow(shopcartHead);
            }
            if (pos == findPagehead)
            {
                if (linklistFind(shopcartHead, findPagehead->next) == 0)
                {
                    payNumsub(shopcartHead, findPagehead->next);
                }
                else
                    printf("���ﳵ��û�и���Ʒ\n");
                linklistShow(shopcartHead);
            }
        }
        if (x > 480 && x < 530 && y > 370 && y < 470)
        {
            if (pos == commodityHead->next)
            {
                if (linklistFind(shopcartHead, commodityPos->next) == 0)
                {
                    payNumsub(shopcartHead, commodityPos->next);
                }
                else
                    printf("���ﳵ��û�и���Ʒ\n");
                linklistShow(shopcartHead);
            }
            if (pos == foodHead->next)
            {
                if (linklistFind(shopcartHead, foodPos->next) == 0)
                {
                    payNumsub(shopcartHead, foodPos->next);
                }
                else
                    printf("���ﳵ��û�и���Ʒ\n");
                linklistShow(shopcartHead);
            }
            if (pos == drinkPos->next)
            {
                if (linklistFind(shopcartHead, drinkPos->next) == 0)
                {
                    payNumsub(shopcartHead, drinkPos->next);
                }
                else
                    printf("���ﳵ��û�и���Ʒ\n");
                linklistShow(shopcartHead);
            }
        }
        //********************************************************

        //*********************����*********************

        if (x > 500 && x < 630 && y > 10 && y < 90)
        {
            printf("��Ʒ��\t �۸�\t ��Ŀ\n");
            if (pos = shopcartHead->next)
            {
                if (shopcartHead->next != shopcartHead)
                    checkout(shopcartHead);
            }
            printf("���˳ɹ�\n");
            pos = commodityHead->next;
            commodityPos = commodityHead->next;
            showBmp(commodityPos, lcd, commodityHead);
        }

        //********************************************************

        //*********************����*********************
        if (x > 640 && x < 790 && y > 190 && y < 290)
        {
            pos = findPagehead;
            findPage(lcd);
            int cmd = keyboard(td);
            printf("%s", findname);

            if (cmd == 0)
            {
                int flag = 1;
                for (node *pos = commodityHead->next; pos != commodityHead; pos = pos->next)
                {
                    if (strcmp(pos->datatype.finder, findname) == 0)
                    {
                        linklistAdd(findPagehead, pos->datatype);
                        showBmp(findPagehead->next, lcd, findPagehead);
                        showDitail(pos, lcd);
                        flag = 0;
                        break;
                    }
                }
                if (flag)
                {
                    Display_characterX(120,          // x?????????
                                       120,          // y?????????
                                       "û�и���Ʒ", // GB2312 ?????????
                                       0x00,         // ????????
                                       2);           // ??????��
                    sleep(2);                        //
                    pos = commodityHead->next;
                    commodityPos = commodityHead->next;
                    showBmp(pos, lcd, commodityHead);
                }
            }
            if (cmd == 1)
            {
                pos = commodityHead->next;
                commodityPos = commodityHead->next;
                showBmp(pos, lcd, commodityHead);
            }
        }
        //********************************************************
    }

    // �رմ�����
    close(td);
    // ???lcd
    close_lcd(lcd);
    // ??????
    UnInit_Font();

    return 0;
}
