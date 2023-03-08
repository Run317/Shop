// ��ʾ�˵�,�˵�ҳ�������Ʒ��Ϣ�����ﳵ������ҳ��

#include "../inc/menu.h"
#include "../inc/linklist.h"
#include "../inc/font.h"

// ��ʼ��lcd��Ļ
LCD *lcd_init(void)
{
    LCD *lcd = malloc(sizeof(LCD));
    // ��Һ��������ͼƬ�ļ���
    lcd->lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd->lcd_fd == -1)
    {
        perror("open lcd");
        return NULL;
    }
    // �Դ�ӳ��
    lcd->p = mmap(
        NULL,                   // ϵͳĬ�ϸ����Ƿ���һ��ӳ���ַ
        800 * 480 * 4,          // ӳ��Ŀռ��С
        PROT_READ | PROT_WRITE, // �ռ�ʹ��Ȩ��
        MAP_SHARED,             // �ռ����ݵ�ͬ������
        lcd->lcd_fd,            // ӳ����ļ����ļ�������
        0                       // ƫ����
    );
    if (lcd->p == MAP_FAILED)
    {
        perror("mmap");
        return NULL;
    }
    return lcd;
}

// �ر���Ļ
void close_lcd(LCD *lcd)
{
    close(lcd->lcd_fd);
    munmap(lcd->p, 800 * 480 * 4);
}

// ��ʾ���
void showAdd(LCD *lcd)
{
    // �򿪹��ͼƬ
    int adFd = open("../pic/ad.bmp", O_RDONLY);
    if (adFd == -1)
    {
        perror("ad.bmp open failed:");
        return;
    }

    // ��ȡͼƬ��ɫ����
    char head[54];
    read(adFd, head, 54);
    int w = head[18] | head[19] << 8 | head[20] << 16 | head[21] << 24;
    int h = head[22] | head[23] << 8 | head[24] << 16 | head[25] << 24;

    // ͼƬԤ����
    char buf[w * h * 3];
    // ��ͼƬ�Ŀ���4����������ƫ�Ƶ�ÿһ�жಹ�����Ŀհ��ַ�
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
            // ��bgrת��rgb������ص��ʱ�򣬽�������ص�ӳ���ڴ�
            *(lcd->p + x + 10 + (h - 1 - y + 10) * 800) = color; // ͼƬ��ʾ��ʼ���꣨10��10��
        }
    }
    close(adFd);
    return;
}

// ��ʾ�˵�
void ShowMenu(LCD *lcd)
{
    // �򿪲˵�ͼƬ

    int menu_fd = open("../pic/menu.bmp", O_RDONLY);
    if (menu_fd == -1)
    {
        perror("menu.bmp open failed:");
        return;
    }

    // // �򿪹��ﳵ
    // int ShoppingCart_fd = open("../pic/ShoppingCart.bmp", O_RDWR);
    // if (ShoppingCart_fd == -1)
    // {
    //     perror("ShoppingCart.bmp open failed:");
    //     return;
    // }

    // ��ȡͼƬ����ɫ����
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
            // ��bgrת��rgb������ص��ʱ�򣬽�������ص�ӳ���ڴ�
            *(lcd->p + x + ((h - 1 - y) * 800)) = color;
        }
    }

    close(menu_fd);
}

// ��ʾ��ƷͼƬ
void showBmp(node *showHead, LCD *lcd, node *linkHead)
{
    // ����ڵ��������������ͼƬ����ȡ���ļ�������
    node *pos = showHead;
    // �򿪵�һ��ͼƬ
    int fd1 = open(pos->datatype.picPath, O_RDONLY);
    if (fd1 == -1)
    {
        perror("commodity1Bmp open failed:");
        return;
    }

    // ��ȡͼƬ��ɫ����
    char head1[54];
    read(fd1, head1, 54);
    int w1 = head1[18] | head1[19] << 8 | head1[20] << 16 | head1[21] << 24;
    int h1 = head1[22] | head1[23] << 8 | head1[24] << 16 | head1[25] << 24;

    // ͼƬԤ����
    char buf1[w1 * h1 * 3];
    // ��ͼƬ�Ŀ���4����������ƫ�Ƶ�ÿһ�жಹ�����Ŀհ��ַ�
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
            // ��bgrת��rgb������ص��ʱ�򣬽�������ص�ӳ���ڴ�
            *(lcd->p + x1 + 10 + (h1 - 1 - y1 + 10) * 800) = color1; // ͼƬ��ʾ��ʼ���꣨10��10��
        }
    }

    // �򿪹�����
    int buyFd = open("../pic/buy.bmp", O_RDONLY);
    if (buyFd == -1)
    {
        perror("commodity1Bmp open failed:");
        return;
    }
    // ��ȡͼƬ��ɫ����
    char head[54];
    read(buyFd, head, 54);
    int w = head[18] | head[19] << 8 | head[20] << 16 | head[21] << 24;
    int h = head[22] | head[23] << 8 | head[24] << 16 | head[25] << 24;

    // ͼƬԤ����
    char buf[w * h * 3];
    // ��ͼƬ�Ŀ���4����������ƫ�Ƶ�ÿһ�жಹ�����Ŀհ��ַ�
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
            // ��bgrת��rgb������ص��ʱ�򣬽�������ص�ӳ���ڴ�
            *(lcd->p + x + 10 + (h - 1 - y + 370) * 800) = color; // ͼƬ��ʾ��ʼ���꣨10��10��
            *(lcd->p + x + 320 + (h - 1 - y + 370) * 800) = color;
        }
    }

    // ��ʾ��һ��ͼƬ

    // ���ʣ��ͼƬ����������ҳ����ʾռλͼƬ
    int fd2;
    if (pos->next == linkHead)
    // ����һ����ƷͼƬ
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
    // ��ȡͼƬ��ɫ����
    char head2[54];
    read(fd2, head2, 54);
    int w2 = head2[18] | head2[19] << 8 | head2[20] << 16 | head2[21] << 24;
    int h2 = head2[22] | head2[23] << 8 | head2[24] << 16 | head2[25] << 24;

    // ͼƬԤ����
    char buf2[w2 * h2 * 3];
    // ��ͼƬ�Ŀ���4����������ƫ�Ƶ�ÿһ�жಹ�����Ŀհ��ַ�
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
            // ��bgrת��rgb������ص��ʱ�򣬽�������ص�ӳ���ڴ�
            *(lcd->p + x2 + 320 + (h2 - 1 - y2 + 10) * 800) = color2; // ͼƬ��ʾ��ʼ���꣨10��10��
        }
    }

    // ��ʾ�۸񼰹�������
    Display_characterX(55,                  // x������ʼ��
                       395,                 // y������ʼ��
                       pos->datatype.price, // GB2312 �����ַ���
                       0x00,                // ������ɫֵ
                       4);                  // ����Ĵ�С

    // ��ʾ�۸񼰹�������
    Display_characterX(365,                       // x������ʼ��
                       395,                       // y������ʼ��
                       pos->next->datatype.price, // GB2312 �����ַ���
                       0x00,                      // ������ɫֵ
                       4);                        // ����Ĵ�С

    // �ر�ͼƬ
    close(fd1);
    close(fd2);
    close(buyFd);

    return;
}

// �����¼�
void touch(int *x, int *y, int td)
{
    // ��ȡ�¼��ṹ��
    struct input_event ie;
    while (1)
    {
        read(td, &ie, sizeof(struct input_event));
        // ��3���ж��¼��ǲ��Ǵ����¼� if
        if (ie.type == EV_ABS) // �����¼�
        {
            // ����code������x����y������value����ȡ�����ֵ
            if (ie.code == ABS_X)
            {
                *x = ie.value * 800 / 1024; // ��ɫ
            }
            if (ie.code == ABS_Y)
            {
                *y = ie.value * 480 / 600; // ��ɫ
            }
        }

        if (ie.type == EV_KEY && ie.code == BTN_TOUCH && ie.value == 0)
        {
            break;
        }
    }
}

// ��ʾ��Ʒ��ϸ��Ϣ
void showDitail(node *commodity, LCD *lcd)
{
    // �򿪹��ͼƬ
    int fd1 = open(commodity->datatype.picPath, O_RDONLY);
    if (fd1 == -1)
    {
        perror("commodityBmp open failed:");
        return;
    }

    // ��ȡͼƬ��ɫ����
    char head1[54];
    read(fd1, head1, 54);
    int w1 = head1[18] | head1[19] << 8 | head1[20] << 16 | head1[21] << 24;
    int h1 = head1[22] | head1[23] << 8 | head1[24] << 16 | head1[25] << 24;

    // ͼƬԤ����
    char buf1[w1 * h1 * 3];
    // ��ͼƬ�Ŀ���4����������ƫ�Ƶ�ÿһ�жಹ�����Ŀհ��ַ�
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
            // ��bgrת��rgb������ص��ʱ�򣬽�������ص�ӳ���ڴ�
            *(lcd->p + x1 + 10 + (h1 - 1 - y1 + 10) * 800) = color1; // ͼƬ��ʾ��ʼ���꣨10��10��
        }
    }
    // ��ռλͼ
    int adFd = open("../pic/zhanwei.bmp", O_RDONLY);
    if (adFd == -1)
    {
        perror("commodity2Bmp open failed:");
        return;
    }
    // ��ȡͼƬ��ɫ����
    char head[54];
    read(adFd, head, 54);
    int w = head[18] | head[19] << 8 | head[20] << 16 | head[21] << 24;
    int h = head[22] | head[23] << 8 | head[24] << 16 | head[25] << 24;

    // ͼƬԤ����
    char buf[w * h * 3];
    // ��ͼƬ�Ŀ���4����������ƫ�Ƶ�ÿһ�жಹ�����Ŀհ��ַ�
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
            // ��bgrת��rgb������ص��ʱ�򣬽�������ص�ӳ���ڴ�
            *(lcd->p + x + 320 + (h - 1 - y + 10) * 800) = color; // ͼƬ��ʾ��ʼ���꣨320��10��
        }
    }
    // ��Ʒ��
    Display_characterX(320,                      // x������ʼ��
                       10,                       // y������ʼ��
                       commodity->datatype.name, // GB2312 �����ַ���
                       0x00,                     // ������ɫֵ
                       2);                       // ����Ĵ�С

    // ��������
    Display_characterX(320,                      // x������ʼ��
                       82,                       // y������ʼ��
                       commodity->datatype.data, // GB2312 �����ַ���
                       0x00,                     // ������ɫֵ
                       2);                       // ����Ĵ�С
                                                 // ��ʾ��ϸ��Ϣ
    // ������ַ
    Display_characterX(320,                         // x������ʼ��
                       154,                         // y������ʼ��
                       commodity->datatype.address, // GB2312 �����ַ���
                       0x00,                        // ������ɫֵ
                       2);                          // ����Ĵ�С
                                                    // ��ʾ��ϸ��Ϣ
    // ������
    Display_characterX(320,                            // x������ʼ��
                       226,                            // y������ʼ��
                       commodity->datatype.netContent, // GB2312 �����ַ���
                       0x00,                           // ������ɫֵ
                       2);                             // ����Ĵ�С

    // ��Ʒ��׼��
    Display_characterX(320,                    // x������ʼ��
                       298,                    // y������ʼ��
                       commodity->datatype.ID, // GB2312 �����ַ���
                       0x00,                   // ������ɫֵ
                       2);                     // ����Ĵ�С

    // �ر�ͼƬ
    close(fd1);
    close(adFd);

    return;
}
// ���Ϸ�ҳ
void pageUp(node *showHead, int linkSize, node *linkHead)
{
    node *pos = showHead;
}

// ���ﳵ
void shopCart(node *head, LCD *lcd)
{
    // ����ڵ��������������ͼƬ����ȡ���ļ�������
    node *pos = head;
    // �򿪹��ͼƬ
    int fd1 = open("../pic/shopcart.bmp", O_RDONLY);
    if (fd1 == -1)
    {
        perror("commodityBmp open failed:");
        return;
    }

    // ��ȡͼƬ��ɫ����
    char head1[54];
    read(fd1, head1, 54);
    int w1 = head1[18] | head1[19] << 8 | head1[20] << 16 | head1[21] << 24;
    int h1 = head1[22] | head1[23] << 8 | head1[24] << 16 | head1[25] << 24;

    // ͼƬԤ����
    char buf1[w1 * h1 * 3];
    // ��ͼƬ�Ŀ���4����������ƫ�Ƶ�ÿһ�жಹ�����Ŀհ��ַ�
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
            // ��bgrת��rgb������ص��ʱ�򣬽�������ص�ӳ���ڴ�
            *(lcd->p + x1 + 10 + (h1 - 1 - y1 + 10) * 800) = color1; // ͼƬ��ʾ��ʼ���꣨10��10��
        }
    }
    int num = 0;
    for (node *shop = head->next; shop != head; shop = shop->next)
    {
        Display_characterX(10,                  // x������ʼ��
                           10 + num * 50,       // y������ʼ��
                           shop->datatype.name, // GB2312 �����ַ���
                           0xFF,                // ������ɫֵ
                           2);                  // ����Ĵ�С
        num++;
    }
}

// ��������
void findPage(LCD *lcd)
{
    int keyboardFd = open("../pic/keybord.bmp", O_RDONLY);
    if (keyboardFd == -1)
    {
        perror("keyboardBmp open failed:");
        return;
    }

    // ��ȡͼƬ��ɫ����
    char head[54];
    read(keyboardFd, head, 54);
    int w = head[18] | head[19] << 8 | head[20] << 16 | head[21] << 24;
    int h = head[22] | head[23] << 8 | head[24] << 16 | head[25] << 24;

    // ͼƬԤ����
    char buf[w * h * 3];
    // ��ͼƬ�Ŀ���4����������ƫ�Ƶ�ÿһ�жಹ�����Ŀհ��ַ�
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
            // ��bgrת��rgb������ص��ʱ�򣬽�������ص�ӳ���ڴ�
            *(lcd->p + x + 10 + ((h - 1 - y + 10) * 800)) = color; // ͼƬ��ʾ��ʼ���꣨10��10��
        }
    }
    close(keyboardFd);
    return;
}