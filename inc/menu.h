#ifndef __MENU_H__
#define __MENU_H__

#include <linux/input.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include "../inc/linklist.h"

// 定义LCD结构体，包含LCD文件描述符及显存映射地址
typedef struct LCDINIT
{
    int lcd_fd;
    int *p;
} LCD;

// 初始化lcd屏幕
LCD *lcd_init(void);
// 关闭屏幕
void close_lcd(LCD *lcd);

// 显示菜单
void ShowMenu(LCD *lcd);
// 显示商品图片
void showBmp(node *showHead, LCD *lcd, node *linkHead);

// 显示广告
void showAdd(LCD *lcd);
// 显示商品详细信息
void showDitail(node *commodity, LCD *lcd);
// 购物车
void shopCart(node *head, LCD *lcd);
// 搜索界面
void findPage(LCD *lcd);
// 查找商品
int keyboard(int td);

// 触摸事件
void touch(int *x, int *y, int td);

#endif