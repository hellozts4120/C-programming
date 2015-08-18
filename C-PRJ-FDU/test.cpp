#include <stdio.h>

//这一段是为了播放音乐,控制屏幕的颜色和光标的位置
#pragma comment(lib,"winmm.lib")
#include <windows.h>


void main() {
PlaySound(TEXT("sound\\start1.wav"),NULL,SND_FILENAME|SND_ASYNC);

getchar();


}