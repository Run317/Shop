#include "../inc/menu.h"
#include "../inc/linklist.h"
#include "../inc/font.h"

extern char findname[256];

int main(int argc, char const *argv[])
{
    // 打开触摸屏
    int td = open("/dev/input/event0", O_RDWR);
    if (td == -1)
    {
        perror("/dev/input/event0");
        return 0;
    }

    // 初始化lcd
    LCD *lcd = lcd_init();
    // 打开字库
    Init_Font();
    printf("初始化LCD\n");
    // 初始化商品头节点
    node *commodityHead = linklistInit();
    printf("初始化商品头节点\n");
    // 初始化商品信息
    initInfo();
    printf("初始化商品信息\n");
    // 将文件信息导入链表
    int commodityNum = inputInfo(commodityHead);
    printf("将文件信息导入链表\n");

    // 显示主菜单
    ShowMenu(lcd);
    printf("显示主菜单\n");

    // 初始化商品头皮节点
    // 零食
    // 初始化零食头节点
    node *foodHead = linklistInit();
    char food[128] = "food";
    // 将商品分类，将类型为food的节点新增到foodhead
    int foodNum = productCategory(commodityHead, foodHead, food);
    printf("\n");
    printf("food head:\n");
    linklistShow(foodHead);

    // 饮料
    node *drinkHead = linklistInit();
    char drink[128] = "drink";
    // 将商品分类，将类型为drink的节点新增到drinkhead
    int drinkNum = productCategory(commodityHead, drinkHead, drink);
    printf("\n");
    printf("drink head:\n");
    linklistShow(drinkHead);

    // 初始化购物车头节点
    node *shopcartHead = linklistInit();
    // 初始化查找页面头节点
    node *findPagehead = linklistInit();

    // 显示广告
    showAdd(lcd);
    sleep(5); // 广告显示五秒

    // 显示商品界面
    node *pos = commodityHead->next;
    node *commodityPos = commodityHead->next;
    node *foodPos = foodHead->next;
    node *drinkPos = drinkHead->next;
    showBmp(pos, lcd, commodityHead);
    //**************测试***********
    // while (pos != commodityHead)
    // {
    //     showBmp(pos, lcd, commodityHead);
    //     sleep(1);
    //     pos = pos->next->next;
    // }
    //********************************************

    // 触摸事件x，y坐标
    int x, y;

    while (1)
    {

        touch(&x, &y, td);
        printf("(%d,%d)\n", x, y);
        //*********************显示商品*********************
        // 所有商品
        if (x > 640 && x < 690 && y > 100 && y < 180)
        {
            pos = commodityHead->next;
            commodityPos = commodityHead->next;
            showBmp(pos, lcd, commodityHead);
        }
        // 零食类
        if (x > 690 && x < 740 && y > 100 && y < 180)
        {
            pos = foodHead->next;
            foodPos = foodHead->next;
            showBmp(pos, lcd, foodHead);
        }
        // 饮料类
        if (x > 740 && x < 790 && y > 100 && y < 180)
        {
            pos = drinkHead->next;
            drinkPos = drinkHead->next;
            showBmp(pos, lcd, drinkHead);
        }
        //********************************************************

        //*********************购物车*********************
        if (x > 640 && x < 790 && y > 300 && y < 380)
        {
            pos = shopcartHead;
            shopCart(shopcartHead, lcd);
            linklistShow(shopcartHead);
        }

        //********************************************************

        //*********************翻页*********************

        if (x > 640 && x < 715 && y > 400 && y < 470) // 向前翻页
        {
            // 判断在哪个节点
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
        if (x > 715 && x < 790 && y > 400 && y < 470) // 向后翻页
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

        //*********************显示商品详细信息*********************
        if (x > 10 && x < 320 && y > 10 && y < 370)
        {
            printf("显示商品详细信息\n");
            if (pos == commodityHead->next)
                showDitail(commodityPos, lcd);
            if (pos == foodHead->next)
                showDitail(foodPos, lcd);
            if (pos == drinkHead->next)
                showDitail(drinkPos, lcd);
        }
        if (x > 320 && x < 630 && y > 10 && y < 370)
        {
            printf("显示商品详细信息\n");
            if (pos == commodityHead->next)
                showDitail(commodityPos->next, lcd);
            if (pos == foodHead->next)
                showDitail(foodPos->next, lcd);
            if (pos == drinkHead->next)
                showDitail(drinkPos->next, lcd);
        }
        //********************************************************

        //*********************增加，减少商品数量*********************
        // 增加商品数量
        if (x > 270 && x < 320 && y > 370 && y < 470)
        {
            printf("增加商品\n");
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
            printf("增加商品\n");
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
        // 减少商品数量
        if (x > 170 && x < 220 && y > 370 && y < 470)
        {
            if (pos == commodityHead->next)
            {
                if (linklistFind(shopcartHead, commodityPos) == 0)
                {
                    payNumsub(shopcartHead, commodityPos);
                }
                else
                    printf("购物车中没有该商品\n");
                linklistShow(shopcartHead);
            }
            if (pos == foodHead->next)
            {
                if (linklistFind(shopcartHead, foodPos) == 0)
                {
                    payNumsub(shopcartHead, foodPos);
                }
                else
                    printf("购物车中没有该商品\n");
                linklistShow(shopcartHead);
            }
            if (pos == drinkPos->next)
            {
                if (linklistFind(shopcartHead, drinkPos) == 0)
                {
                    payNumsub(shopcartHead, drinkPos);
                }
                else
                    printf("购物车中没有该商品\n");
                linklistShow(shopcartHead);
            }
            if (pos == findPagehead)
            {
                if (linklistFind(shopcartHead, findPagehead->next) == 0)
                {
                    payNumsub(shopcartHead, findPagehead->next);
                }
                else
                    printf("购物车中没有该商品\n");
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
                    printf("购物车中没有该商品\n");
                linklistShow(shopcartHead);
            }
            if (pos == foodHead->next)
            {
                if (linklistFind(shopcartHead, foodPos->next) == 0)
                {
                    payNumsub(shopcartHead, foodPos->next);
                }
                else
                    printf("购物车中没有该商品\n");
                linklistShow(shopcartHead);
            }
            if (pos == drinkPos->next)
            {
                if (linklistFind(shopcartHead, drinkPos->next) == 0)
                {
                    payNumsub(shopcartHead, drinkPos->next);
                }
                else
                    printf("购物车中没有该商品\n");
                linklistShow(shopcartHead);
            }
        }
        //********************************************************

        //*********************结账*********************

        if (x > 500 && x < 630 && y > 10 && y < 90)
        {
            printf("商品名\t 价格\t 数目\n");
            if (pos = shopcartHead->next)
            {
                if (shopcartHead->next != shopcartHead)
                    checkout(shopcartHead);
            }
            printf("结账成功\n");
            pos = commodityHead->next;
            commodityPos = commodityHead->next;
            showBmp(commodityPos, lcd, commodityHead);
        }

        //********************************************************

        //*********************查找*********************
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
                                       "没有该商品", // GB2312 ?????????
                                       0x00,         // ????????
                                       2);           // ??????С
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

    // 关闭触摸屏
    close(td);
    // ???lcd
    close_lcd(lcd);
    // ??????
    UnInit_Font();

    return 0;
}
