#ifndef UI_H
#define UI_H
#include "main.h"
#define CONTENT_Y_START  40
#define MAX_VISIBLE_ITEMS 7   // 280/40 = 7
#define TAB_COUNT     3
#define TAB_ICON_W    32
#define TAB_ICON_H    32
#define TOP_BAR_H     40
#define MAX_MENU_DEPTH 8

typedef void (*Action)(void);

// 单个子菜单项
typedef struct  {
     char   *text;
     uint16_t *icon;       // 16x16 小图标，可为 NULL
    Action        action;       // 点击后执行的函数
    struct MenuPage *subpage;   // 若有子菜单，指向下一级，否则为 NULL
} SubItem;

// 一个菜单页面（可以是一个设置页、子菜单等）
typedef struct  {
     char *title;          // 页面标题（可显示在顶部栏）
    SubItem    *items;
    uint8_t     itemCount;
    uint8_t     itemHeight;     // 本页每一项的高度，通常固定为 ITEM_HEIGHT
} MenuPage;

// 主 Tab（顶部图标）
typedef struct  {
     char   *name;
     uint16_t *icon;       // 32x32 主图标
    const uint16_t *iconSel;    // 选中时的图标（高亮版）
    MenuPage     *rootPage;     // 该 Tab 的根菜单页
} MainTab;
void testUi(void) ;
#endif

