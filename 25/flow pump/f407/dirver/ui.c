#include "st7789.h"
#include "font_lib.h"
#include "image_display.h"
#include "spiFileProc.h"
#include "ui.h"

MenuPage *menuStack[MAX_MENU_DEPTH];
uint8_t   menuDepth = 0;          // 当前栈深度（0 为根菜单）

int8_t scrollTopIndex = 0;   // 当前显示的第一项
int8_t highlightIndex = 0;   // 当前高亮项的索引（绝对索引）
// 当前显示的页面
MenuPage *currentPage = NULL;

void drawSubItem(const MenuPage *page, uint8_t itemIndex, uint16_t baseY, uint8_t isHighlighted) {
    SubItem *item = &page->items[itemIndex];
    uint16_t y = baseY + (itemIndex - scrollTopIndex) * page->itemHeight;
    // y 已经加上了内容区起始偏移 40，传入时应保证 baseY = 40
    uint16_t bg = isHighlighted ? GREEN : BLACK;
    uint16_t fg = WHITE;

    // 绘制背景
     ST7789_DrawFilledRectangle(0, y, 240, page->itemHeight, bg);
    // 图标（靠左）
    if (item->icon) {
		show_image("YXZT1", 10, y + (page->itemHeight - 16) / 2);//run stop 0 60 34	 TJ
//        LCD_DrawBitmap(10, y + (page->itemHeight - 16) / 2, item->icon, 16, 16);
    }
    // 文字（图标右侧）
    draw_string_ex(page->itemHeight+10, y + (page->itemHeight - 16) / 2, BLACK, 24, "测试1", WHITE);
//    LCD_DrawString(40, y + (page->itemHeight - 16) / 2, item->text, fg, bg);
    // 底部分割线
    if (itemIndex < page->itemCount - 1) {
//        LCD_FillRect(0, y + page->itemHeight - 1, 240, 1, DARKGREY);
		ST7789_DrawFilledRectangle(0, y + page->itemHeight - 1+10, 240, 1, bg);
    }
}



void drawContentArea(const MenuPage *page, int8_t topIdx, int8_t hlIdx) {
    if (!page || page->itemCount == 0) {
        // 清空内容区
//        LCD_SetWindow(0, CONTENT_Y_START, 239, 319);
//        LCD_Fill(BLACK);
	ST7789_SetAddressWindow(0, CONTENT_Y_START, 239, 319);
	ST7789_Fill_Color_Var(BLACK);
        return;
    }

    int8_t visible = (page->itemCount - topIdx) > MAX_VISIBLE_ITEMS ? MAX_VISIBLE_ITEMS : (page->itemCount - topIdx);
    if (visible < 0) visible = 0;

    // 先全部清空内容区
    
//    LCD_SetWindow(0, CONTENT_Y_START, 239, 319);
//    LCD_Fill(BLACK);
	ST7789_SetAddressWindow(0, CONTENT_Y_START, 239, 319);
	ST7789_Fill_Color_Var(BLACK);

    for (int8_t i = 0; i < visible; i++) {
        int8_t idx = topIdx + i;
        uint8_t isHL = (idx == hlIdx) ? 1 : 0;
        uint16_t y = CONTENT_Y_START + i * page->itemHeight;
        drawSubItem(page, idx, CONTENT_Y_START, isHL);
    }
    // 如果可见项之后还有剩余空间，会保持黑色（已清屏）
}
void moveHighlight(const MenuPage *page, int8_t oldHL, int8_t newHL) {
    if (oldHL == newHL) return;
    // 擦除旧项高亮（重绘为普通状态）
    if (oldHL >= scrollTopIndex && oldHL < scrollTopIndex + MAX_VISIBLE_ITEMS) {
        drawSubItem(page, oldHL, CONTENT_Y_START, 0);
    }
    // 绘制新项高亮
    if (newHL >= scrollTopIndex && newHL < scrollTopIndex + MAX_VISIBLE_ITEMS) {
        drawSubItem(page, newHL, CONTENT_Y_START, 1);
    }
}
void onEncoderRotate(int8_t direction) {
    if (!currentPage || currentPage->itemCount == 0) return;

    int8_t newHL = highlightIndex + direction;
    if (newHL < 0) newHL = 0;
    if (newHL >= currentPage->itemCount) newHL = currentPage->itemCount - 1;
    if (newHL == highlightIndex) return;

    // 计算新的 scrollTopIndex
    int8_t newTop = scrollTopIndex;
    // 若向上移动且新项在显示区之上，则需要向上滚动
    if (newHL < scrollTopIndex) {
        newTop = newHL;
    }
    // 若向下移动且新项在显示区之下，则需要向下滚动
    else if (newHL >= scrollTopIndex + MAX_VISIBLE_ITEMS) {
        newTop = newHL - MAX_VISIBLE_ITEMS + 1;
    }

    // 如果 scrollTopIndex 改变了，重绘整个内容区
    if (newTop != scrollTopIndex) {
        scrollTopIndex = newTop;
        drawContentArea(currentPage, scrollTopIndex, newHL);
    } else {
        // 否则只局部刷新高亮项
        moveHighlight(currentPage, highlightIndex, newHL);
    }

    highlightIndex = newHL;
}

void loadPage(MenuPage *page) {
    currentPage = page;
    if (!page) return;

    // 重置滚动和高亮
    scrollTopIndex = 0;
    highlightIndex = 0;

    // 如果内容高度大于可见区域，启用滚动标志
    _Bool needScroll = (page->itemCount * page->itemHeight) > (320 - TOP_BAR_H);
    // 实际绘制
    drawContentArea(page, scrollTopIndex, highlightIndex);
}
void testUi()
{
	MenuPage page;
	page.itemCount = 4;
	page.itemHeight = 48;
	page.items->action = 0;
	(char *)page.items->icon = "YXZT1";
	//page.items->subpage->itemCount = 3;
	//page.items.subpage.itemHeight = 48;
	page.title = "test";
	//page.title = "main";
	loadPage(&page) ;
}

