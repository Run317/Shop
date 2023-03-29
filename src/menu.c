// 显示菜单,菜单页面包括商品信息，购物车，查找页面

#include "../inc/menu.h"
#include "../inc/linklist.h"
#include "../inc/font.h"

// 初始化lcd屏幕
LCD *lcd_init(void)
{
    LCD *lcd = malloc(sizeof(LCD));
    // 打开液晶屏，打开图片文件的
    lcd->lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd->lcd_fd == -1)
    {
        perror("open lcd");
        return NULL;
    }
    // 显存映射
    lcd->p = mmap(
        NULL,                   // 系统默认给我们分配一个映射地址
        800 * 480 * 4,          // 映射的空间大小
        PROT_READ | PROT_WRITE, // 空间使用权限
        MAP_SHARED,             // 空间数据的同步共享
        lcd->lcd_fd,            // 映射的文件的文件描述符
        0                       // 偏移量
    );
    if (lcd->p == MAP_FAILED)
    {
        perror("mmap");
        return NULL;
    }
    return lcd;
}

// 关闭屏幕
void close_lcd(LCD *lcd)
{
    close(lcd->lcd_fd);
    munmap(lcd->p, 800 * 480 * 4);
}

// 显示广告
void showAdd(LCD *lcd)
{
    // 打开广告图片
    int adFd = open("../pic/ad.bmp", O_RDONLY);
    if (adFd == -1)
    {
        perror("ad.bmp open failed:");
        return;
    }

    // 读取图片颜色数据
    char head[54];
    read(adFd, head, 54);
    int w = head[18] | head[19] << 8 | head[20] << 16 | head[21] << 24;
    int h = head[22] | head[23] << 8 | head[24] << 16 | head[25] << 24;

    // 图片预处理
    char buf[w * h * 3];
    // 当图片的宽不是4的整数倍，偏移掉每一行多补出来的空白字符
    switch (4 - (w * 3 % 4)) // 3  ==  bit_count/8
    // switch((w%4))
    {
    case 1:
        for (int i = 0; i < h; ++i)
        {
            read(adFd, buf + w * 3 * i, w * 3);
            lseek(adFd, 1, SEEK_CUR);
        }
        break;
    case 2:
        for (int i = 0; i < h; ++i)
        {
            read(adFd, buf + w * 3 * i, w * 3);
            lseek(adFd, 2, SEEK_CUR);
        }
        break;
    case 3:
        for (int i = 0; i < h; ++i)
        {
            read(adFd, buf + w * 3 * i, w * 3);
            lseek(adFd, 3, SEEK_CUR);
        }
        break;
    default:
        read(adFd, buf, w * h * 3);
    }

    int x, y;
    char b, g, r;
    int color;
    int i = 0;
    for (y = 0; y < h; ++y)
    {
        for (x = 0; x < w; ++x)
        {
            b = buf[i++]; // b
            g = buf[i++]; // g
            r = buf[i++]; // r
            color = b | g << 8 | r << 16;
            // 将bgr转成rgb这个像素点的时候，将这个像素点映射内存
            *(lcd->p + x + 10 + (h - 1 - y + 10) * 800) = color; // 图片显示起始坐标（10，10）
        }
    }
    close(adFd);
    return;
}

// 显示菜单
void ShowMenu(LCD *lcd)
{
    // 打开菜单图片

    int menu_fd = open("../pic/menu.bmp", O_RDONLY);
    if (menu_fd == -1)
    {
        perror("menu.bmp open failed:");
        return;
    }

    // // 打开购物车
    // int ShoppingCart_fd = open("../pic/ShoppingCart.bmp", O_RDWR);
    // if (ShoppingCart_fd == -1)
    // {
    //     perror("ShoppingCart.bmp open failed:");
    //     return;
    // }

    // 读取图片的颜色数据
    char head[54];
    read(menu_fd, head, 54);
    int w = head[18] | head[19] << 8 | head[20] << 16 | head[21] << 24;
    int h = head[22] | head[23] << 8 | head[24] << 16 | head[25] << 24;

    char bmp_buf[w * h * 3];
    read(menu_fd, bmp_buf, w * h * 3);

    int x, y;
    char b, g, r;
    int color;
    int i = 0;
    for (y = (480 - h) / 2; y < h; ++y)
    {
        for (x = (800 - w) / 2; x < w; ++x)
        {
            b = bmp_buf[i++]; // b
            g = bmp_buf[i++]; // g
            r = bmp_buf[i++]; // r
            color = b | g << 8 | r << 16;
            // 将bgr转成rgb这个像素点的时候，将这个像素点映射内存
            *(lcd->p + x + ((h - 1 - y) * 800)) = color;
        }
    }

    close(menu_fd);
}

// 显示商品图片
void showBmp(node *showHead, LCD *lcd, node *linkHead)
{
    // 定义节点遍历链表，打开所有图片，获取其文件描述符
    node *pos = showHead;
    // 打开第一个图片
    int fd1 = open(pos->datatype.picPath, O_RDONLY);
    if (fd1 == -1)
    {
        perror("commodity1Bmp open failed:");
        return;
    }

    // 读取图片颜色数据
    char head1[54];
    read(fd1, head1, 54);
    int w1 = head1[18] | head1[19] << 8 | head1[20] << 16 | head1[21] << 24;
    int h1 = head1[22] | head1[23] << 8 | head1[24] << 16 | head1[25] << 24;

    // 图片预处理
    char buf1[w1 * h1 * 3];
    // 当图片的宽不是4的整数倍，偏移掉每一行多补出来的空白字符
    switch (4 - (w1 * 3 % 4)) // 3  ==  bit_count/8
    // switch((w%4))
    {
    case 1:
        for (int i = 0; i < h1; ++i)
        {
            read(fd1, buf1 + w1 * 3 * i, w1 * 3);
            lseek(fd1, 1, SEEK_CUR);
        }
        break;
    case 2:
        for (int i = 0; i < h1; ++i)
        {
            read(fd1, buf1 + w1 * 3 * i, w1 * 3);
            lseek(fd1, 2, SEEK_CUR);
        }
        break;
    case 3:
        for (int i = 0; i < h1; ++i)
        {
            read(fd1, buf1 + w1 * 3 * i, w1 * 3);
            lseek(fd1, 3, SEEK_CUR);
        }
        break;
    default:
        read(fd1, buf1, w1 * h1 * 3);
    }

    int x1, y1;
    char b1, g1, r1;
    int color1;
    int i = 0;
    for (y1 = 0; y1 < h1; ++y1)
    {
        for (x1 = 0; x1 < w1; ++x1)
        {
            b1 = buf1[i++]; // b
            g1 = buf1[i++]; // g
            r1 = buf1[i++]; // r
            color1 = b1 | g1 << 8 | r1 << 16;
            // 将bgr转成rgb这个像素点的时候，将这个像素点映射内存
            *(lcd->p + x1 + 10 + (h1 - 1 - y1 + 10) * 800) = color1; // 图片显示起始坐标（10，10）
        }
    }

    // 打开购买栏
    int buyFd = open("../pic/buy.bmp", O_RDONLY);
    if (buyFd == -1)
    {
        perror("commodity1Bmp open failed:");
        return;
    }
    // 读取图片颜色数据
    char head[54];
    read(buyFd, head, 54);
    int w = head[18] | head[19] << 8 | head[20] << 16 | head[21] << 24;
    int h = head[22] | head[23] << 8 | head[24] << 16 | head[25] << 24;

    // 图片预处理
    char buf[w * h * 3];
    // 当图片的宽不是4的整数倍，偏移掉每一行多补出来的空白字符
    switch (4 - (w * 3 % 4)) // 3  ==  bit_count/8
    // switch((w%4))
    {
    case 1:
        for (int i = 0; i < h; ++i)
        {
            read(buyFd, buf + w * 3 * i, w * 3);
            lseek(buyFd, 1, SEEK_CUR);
        }
        break;
    case 2:
        for (int i = 0; i < h; ++i)
        {
            read(buyFd, buf + w * 3 * i, w * 3);
            lseek(buyFd, 2, SEEK_CUR);
        }
        break;
    case 3:
        for (int i = 0; i < h; ++i)
        {
            read(buyFd, buf + w * 3 * i, w * 3);
            lseek(buyFd, 3, SEEK_CUR);
        }
        break;
    default:
        read(buyFd, buf, w * h * 3);
    }

    int x, y;
    char b, g, r;
    int color;
    int k = 0;
    for (y = 0; y < h; ++y)
    {
        for (x = 0; x < w; ++x)
        {
            b = buf[k++]; // b
            g = buf[k++]; // g
            r = buf[k++]; // r
            color = b | g << 8 | r << 16;
            // 将bgr转成rgb这个像素点的时候，将这个像素点映射内存
            *(lcd->p + x + 10 + (h - 1 - y + 370) * 800) = color; // 图片显示起始坐标（10，10）
            *(lcd->p + x + 320 + (h - 1 - y + 370) * 800) = color;
        }
    }

    // 显示下一张图片

    // 如果剩余图片不够放满整页，显示占位图片
    int fd2;
    if (pos->next == linkHead)
    // 打开下一个商品图片
    {
        fd2 = open("../pic/zhanwei.bmp", O_RDONLY);
        if (fd2 == -1)
        {
            perror("zhanwei.bmp open failed:");
            return;
        }
    }
    else
    {
        fd2 = open(pos->next->datatype.picPath, O_RDONLY);
        if (fd2 == -1)
        {
            perror("commodity2Bmp open failed:");
            return;
        }
    }
    // 读取图片颜色数据
    char head2[54];
    read(fd2, head2, 54);
    int w2 = head2[18] | head2[19] << 8 | head2[20] << 16 | head2[21] << 24;
    int h2 = head2[22] | head2[23] << 8 | head2[24] << 16 | head2[25] << 24;

    // 图片预处理
    char buf2[w2 * h2 * 3];
    // 当图片的宽不是4的整数倍，偏移掉每一行多补出来的空白字符
    switch (4 - (w2 * 3 % 4)) // 3  ==  bit_count/8
    // switch((w%4))
    {
    case 1:
        for (int i = 0; i < h; ++i)
        {
            read(fd2, buf2 + w2 * 3 * i, w2 * 3);
            lseek(fd2, 1, SEEK_CUR);
        }
        break;
    case 2:
        for (int i = 0; i < h2; ++i)
        {
            read(fd2, buf2 + w2 * 3 * i, w2 * 3);
            lseek(fd2, 2, SEEK_CUR);
        }
        break;
    case 3:
        for (int i = 0; i < h2; ++i)
        {
            read(fd2, buf2 + w2 * 3 * i, w2 * 3);
            lseek(fd2, 3, SEEK_CUR);
        }
        break;
    default:
        read(fd2, buf2, w2 * h2 * 3);
    }

    int x2, y2;
    char b2, g2, r2;
    int color2;
    int j = 0;
    for (y2 = 0; y2 < h2; ++y2)
    {
        for (x2 = 0; x2 < w2; ++x2)
        {
            b2 = buf2[j++]; // b
            g2 = buf2[j++]; // g
            r2 = buf2[j++]; // r
            color2 = b2 | g2 << 8 | r2 << 16;
            // 将bgr转成rgb这个像素点的时候，将这个像素点映射内存
            *(lcd->p + x2 + 320 + (h2 - 1 - y2 + 10) * 800) = color2; // 图片显示起始坐标（10，10）
        }
    }

    // 显示价格及购买数量
    Display_characterX(55,                  // x坐标起始点
                       395,                 // y坐标起始点
                       pos->datatype.price, // GB2312 中文字符串
                       0x00,                // 字体颜色值
                       4);                  // 字体的大小

    // 显示价格及购买数量
    Display_characterX(365,                       // x坐标起始点
                       395,                       // y坐标起始点
                       pos->next->datatype.price, // GB2312 中文字符串
                       0x00,                      // 字体颜色值
                       4);                        // 字体的大小

    // 关闭图片
    close(fd1);
    close(fd2);
    close(buyFd);

    return;
}

// 触摸事件
void touch(int *x, int *y, int td)
{
    // 读取事件结构体
    struct input_event ie;
    while (1)
    {
        read(td, &ie, sizeof(struct input_event));
        // （3）判断事件是不是触摸事件 if
        if (ie.type == EV_ABS) // 触摸事件
        {
            // 根据code来区别x还是y，根据value来获取具体的值
            if (ie.code == ABS_X)
            {
                *x = ie.value * 800 / 1024; // 黑色
            }
            if (ie.code == ABS_Y)
            {
                *y = ie.value * 480 / 600; // 黑色
            }
        }

        if (ie.type == EV_KEY && ie.code == BTN_TOUCH && ie.value == 0)
        {
            break;
        }
    }
}

// 显示商品详细信息
void showDitail(node *commodity, LCD *lcd)
{
    // 打开广告图片
    int fd1 = open(commodity->datatype.picPath, O_RDONLY);
    if (fd1 == -1)
    {
        perror("commodityBmp open failed:");
        return;
    }

    // 读取图片颜色数据
    char head1[54];
    read(fd1, head1, 54);
    int w1 = head1[18] | head1[19] << 8 | head1[20] << 16 | head1[21] << 24;
    int h1 = head1[22] | head1[23] << 8 | head1[24] << 16 | head1[25] << 24;

    // 图片预处理
    char buf1[w1 * h1 * 3];
    // 当图片的宽不是4的整数倍，偏移掉每一行多补出来的空白字符
    switch (4 - (w1 * 3 % 4)) // 3  ==  bit_count/8
    // switch((w%4))
    {
    case 1:
        for (int i = 0; i < h1; ++i)
        {
            read(fd1, buf1 + w1 * 3 * i, w1 * 3);
            lseek(fd1, 1, SEEK_CUR);
        }
        break;
    case 2:
        for (int i = 0; i < h1; ++i)
        {
            read(fd1, buf1 + w1 * 3 * i, w1 * 3);
            lseek(fd1, 2, SEEK_CUR);
        }
        break;
    case 3:
        for (int i = 0; i < h1; ++i)
        {
            read(fd1, buf1 + w1 * 3 * i, w1 * 3);
            lseek(fd1, 3, SEEK_CUR);
        }
        break;
    default:
        read(fd1, buf1, w1 * h1 * 3);
    }

    int x1, y1;
    char b1, g1, r1;
    int color1;
    int i = 0;
    for (y1 = 0; y1 < h1; ++y1)
    {
        for (x1 = 0; x1 < w1; ++x1)
        {
            b1 = buf1[i++]; // b
            g1 = buf1[i++]; // g
            r1 = buf1[i++]; // r
            color1 = b1 | g1 << 8 | r1 << 16;
            // 将bgr转成rgb这个像素点的时候，将这个像素点映射内存
            *(lcd->p + x1 + 10 + (h1 - 1 - y1 + 10) * 800) = color1; // 图片显示起始坐标（10，10）
        }
    }
    // 打开占位图
    int adFd = open("../pic/zhanwei.bmp", O_RDONLY);
    if (adFd == -1)
    {
        perror("commodity2Bmp open failed:");
        return;
    }
    // 读取图片颜色数据
    char head[54];
    read(adFd, head, 54);
    int w = head[18] | head[19] << 8 | head[20] << 16 | head[21] << 24;
    int h = head[22] | head[23] << 8 | head[24] << 16 | head[25] << 24;

    // 图片预处理
    char buf[w * h * 3];
    // 当图片的宽不是4的整数倍，偏移掉每一行多补出来的空白字符
    switch (4 - (w * 3 % 4)) // 3  ==  bit_count/8
    // switch((w%4))
    {
    case 1:
        for (int i = 0; i < h; ++i)
        {
            read(adFd, buf + w * 3 * i, w * 3);
            lseek(adFd, 1, SEEK_CUR);
        }
        break;
    case 2:
        for (int i = 0; i < h; ++i)
        {
            read(adFd, buf + w * 3 * i, w * 3);
            lseek(adFd, 2, SEEK_CUR);
        }
        break;
    case 3:
        for (int i = 0; i < h; ++i)
        {
            read(adFd, buf + w * 3 * i, w * 3);
            lseek(adFd, 3, SEEK_CUR);
        }
        break;
    default:
        read(adFd, buf, w * h * 3);
    }

    int x, y;
    char b, g, r;
    int color;
    int j = 0;
    for (y = 0; y < h; ++y)
    {
        for (x = 0; x < w; ++x)
        {
            b = buf[j++]; // b
            g = buf[j++]; // g
            r = buf[j++]; // r
            color = b | g << 8 | r << 16;
            // 将bgr转成rgb这个像素点的时候，将这个像素点映射内存
            *(lcd->p + x + 320 + (h - 1 - y + 10) * 800) = color; // 图片显示起始坐标（320，10）
        }
    }
    // 商品名
    Display_characterX(320,                      // x坐标起始点
                       10,                       // y坐标起始点
                       commodity->datatype.name, // GB2312 中文字符串
                       0x00,                     // 字体颜色值
                       2);                       // 字体的大小

    // 生产日期
    Display_characterX(320,                      // x坐标起始点
                       82,                       // y坐标起始点
                       commodity->datatype.data, // GB2312 中文字符串
                       0x00,                     // 字体颜色值
                       2);                       // 字体的大小
                                                 // 显示详细信息
    // 生产地址
    Display_characterX(320,                         // x坐标起始点
                       154,                         // y坐标起始点
                       commodity->datatype.address, // GB2312 中文字符串
                       0x00,                        // 字体颜色值
                       2);                          // 字体的大小
                                                    // 显示详细信息
    // 净含量
    Display_characterX(320,                            // x坐标起始点
                       226,                            // y坐标起始点
                       commodity->datatype.netContent, // GB2312 中文字符串
                       0x00,                           // 字体颜色值
                       2);                             // 字体的大小

    // 产品标准号
    Display_characterX(320,                    // x坐标起始点
                       298,                    // y坐标起始点
                       commodity->datatype.ID, // GB2312 中文字符串
                       0x00,                   // 字体颜色值
                       2);                     // 字体的大小

    // 关闭图片
    close(fd1);
    close(adFd);

    return;
}
// 向上翻页
void pageUp(node *showHead, int linkSize, node *linkHead)
{
    node *pos = showHead;
}

// 购物车
void shopCart(node *head, LCD *lcd)
{
    // 定义节点遍历链表，打开所有图片，获取其文件描述符
    node *pos = head;
    // 打开广告图片
    int fd1 = open("../pic/shopcart.bmp", O_RDONLY);
    if (fd1 == -1)
    {
        perror("commodityBmp open failed:");
        return;
    }

    // 读取图片颜色数据
    char head1[54];
    read(fd1, head1, 54);
    int w1 = head1[18] | head1[19] << 8 | head1[20] << 16 | head1[21] << 24;
    int h1 = head1[22] | head1[23] << 8 | head1[24] << 16 | head1[25] << 24;

    // 图片预处理
    char buf1[w1 * h1 * 3];
    // 当图片的宽不是4的整数倍，偏移掉每一行多补出来的空白字符
    switch (4 - (w1 * 3 % 4)) // 3  ==  bit_count/8
    // switch((w%4))
    {
    case 1:
        for (int i = 0; i < h1; ++i)
        {
            read(fd1, buf1 + w1 * 3 * i, w1 * 3);
            lseek(fd1, 1, SEEK_CUR);
        }
        break;
    case 2:
        for (int i = 0; i < h1; ++i)
        {
            read(fd1, buf1 + w1 * 3 * i, w1 * 3);
            lseek(fd1, 2, SEEK_CUR);
        }
        break;
    case 3:
        for (int i = 0; i < h1; ++i)
        {
            read(fd1, buf1 + w1 * 3 * i, w1 * 3);
            lseek(fd1, 3, SEEK_CUR);
        }
        break;
    default:
        read(fd1, buf1, w1 * h1 * 3);
    }

    int x1, y1;
    char b1, g1, r1;
    int color1;
    int i = 0;
    for (y1 = 0; y1 < h1; ++y1)
    {
        for (x1 = 0; x1 < w1; ++x1)
        {
            b1 = buf1[i++]; // b
            g1 = buf1[i++]; // g
            r1 = buf1[i++]; // r
            color1 = b1 | g1 << 8 | r1 << 16;
            // 将bgr转成rgb这个像素点的时候，将这个像素点映射内存
            *(lcd->p + x1 + 10 + (h1 - 1 - y1 + 10) * 800) = color1; // 图片显示起始坐标（10，10）
        }
    }
    int num = 0;
    for (node *shop = head->next; shop != head; shop = shop->next)
    {
        Display_characterX(10,                  // x坐标起始点
                           10 + num * 50,       // y坐标起始点
                           shop->datatype.name, // GB2312 中文字符串
                           0xFF,                // 字体颜色值
                           2);                  // 字体的大小
        num++;
    }
}

// 搜索界面
void findPage(LCD *lcd)
{
    int keyboardFd = open("../pic/keybord.bmp", O_RDONLY);
    if (keyboardFd == -1)
    {
        perror("keyboardBmp open failed:");
        return;
    }

    // 读取图片颜色数据
    char head[54];
    read(keyboardFd, head, 54);
    int w = head[18] | head[19] << 8 | head[20] << 16 | head[21] << 24;
    int h = head[22] | head[23] << 8 | head[24] << 16 | head[25] << 24;

    // 图片预处理
    char buf[w * h * 3];
    // 当图片的宽不是4的整数倍，偏移掉每一行多补出来的空白字符
    switch (4 - (w * 3 % 4)) // 3  ==  bit_count/8
    // switch((w%4))
    {
    case 1:
        for (int i = 0; i < h; ++i)
        {
            read(keyboardFd, buf + w * 3 * i, w * 3);
            lseek(keyboardFd, 1, SEEK_CUR);
        }
        break;
    case 2:
        for (int i = 0; i < h; ++i)
        {
            read(keyboardFd, buf + w * 3 * i, w * 3);
            lseek(keyboardFd, 2, SEEK_CUR);
        }
        break;
    case 3:
        for (int i = 0; i < h; ++i)
        {
            read(keyboardFd, buf + w * 3 * i, w * 3);
            lseek(keyboardFd, 3, SEEK_CUR);
        }
        break;
    default:
        read(keyboardFd, buf, w * h * 3);
    }

    int x, y;
    char b, g, r;
    int color;
    int i = 0;
    for (y = 0; y < h; ++y)
    {
        for (x = 0; x < w; ++x)
        {
            b = buf[i++]; // b
            g = buf[i++]; // g
            r = buf[i++]; // r
            color = b | g << 8 | r << 16;
            // 将bgr转成rgb这个像素点的时候，将这个像素点映射内存
            *(lcd->p + x + 10 + ((h - 1 - y + 10) * 800)) = color; // 图片显示起始坐标（10，10）
        }
    }
    close(keyboardFd);
    return;
}