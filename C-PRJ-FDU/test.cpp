#include <stdio.h>

//��һ����Ϊ�˲�������,������Ļ����ɫ�͹���λ��
#pragma comment(lib,"winmm.lib")
#include <windows.h>


void main() {
PlaySound(TEXT("sound\\start1.wav"),NULL,SND_FILENAME|SND_ASYNC);

getchar();


}