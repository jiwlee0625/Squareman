

#include "mylib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, unsigned short color) {
	videoBuffer[OFFSET(row, col)] = color;
}
unsigned short getPixel(int row, int col) {
	return videoBuffer[OFFSET(row - 1, col - 1)];
}
void drawRect(int x, int y, int width, int height, u16 color) {
	for (int i = y; i <= y + height; i ++) {
		for (int j = x; j <= x + width; j ++) {
			setPixel(j, i, color);
		}
	}
/*
* for (int r = 0; r < height; r++) {
	REG_DMA3SAD = (u32)&color
	REG_DMA3DAD = (u32)(&videoBuffer[offset(row+r, col, 240)])
	REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_FIXED
}
*/
}
void drawImage3(int x, int y, int width, int height, const u16* image) { 
        for(int r=0; r < height; r++) {
		DMA[3].src = &image[OFFSETS(0, r, width)]; //(y)*(z)+(x)
		DMA[3].dst = &videoBuffer[OFFSET(x, r+y)]; //(y)*SCREEN_WIDTH+(x)
		DMA[3].cnt = (width) | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
        }
}
void fillScreen3(unsigned short color) {
    DMA[3].src = &color;
    DMA[3].dst = videoBuffer;
    DMA[3].cnt = (240 * 160) | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON;
}

void waitForVBlank() {
    while(SCANLINECOUNTER > 160);
    while(SCANLINECOUNTER < 160);
}
