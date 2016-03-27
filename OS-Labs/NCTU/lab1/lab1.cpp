#include<stdio.h>
#include<windows.h>
#include<iostream>
#define BUF_SIZE 256
using namespace std;

int main(int argc, LPTSTR argv[]){
	HANDLE handleIn, handleOut;
	DWORD dwordIn, dwordOut;
	char Buffer[BUF_SIZE];
	char input[BUF_SIZE], output[BUF_SIZE];

	cout << "Input file name: ";
	cin >> input;
	cout << "Output file name: ";
	cin >> output;

	//read from the filename
	handleIn = CreateFile(input, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handleIn == INVALID_HANDLE_VALUE) {
		printf("There's something wrong with the input filename you give! Error: %x, Please check out again!\n", GetLastError());
		return 2;
	}

	//get the output file
	handleOut = CreateFile(output, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handleOut == INVALID_HANDLE_VALUE) {
		printf("There's something wrong with the output filename you give! Error: %x, Please check out again!\n", GetLastError());
		return 3;
	}

	//write file
	DWORD dRemainSize = GetFileSize(handleIn,NULL);
	while (dRemainSize > 0) {
		DWORD dActualRead = 0;
		ReadFile(handleIn,Buffer,256, &dActualRead, NULL);
		dRemainSize -= dActualRead;
		DWORD dActualWrote=0;
		DWORD dOnceWrote=0;  
		WriteFile(handleOut,Buffer,dActualRead,&dOnceWrote,NULL);
	}

	//close the handle
	CloseHandle(handleIn);
	CloseHandle(handleOut);
	printf("Read and Write successfully complete!\n");
	return 0;
}
