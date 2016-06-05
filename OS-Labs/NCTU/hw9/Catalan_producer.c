#include<windows.h>
#include<stdio.h>

int main(int argc, char *argv[]) {
    HANDLE hMapFile, hFile;
    LPVOID lpMapAddress;
    int n, value;
	int i, j;

    hFile = CreateFile("temp.txt", GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr,"Could not open file temp.txt (%d).\n",GetLastError());
        return -1;
    }
    
    hMapFile = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, TEXT("SharedObject"));

    if (hMapFile == NULL) {
        printf("Could not open file mapping object (%d).\n", GetLastError());
        return -1;
    }

    lpMapAddress = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);

    if (lpMapAddress == NULL) {
        printf("Could not map view of file (%d).\n", GetLastError());
        return -1;
    }

    printf("Enter a number:");
    scanf("%d", &n);
    
    memcpy(lpMapAddress, (void *)n, sizeof(n));
    lpMapAddress += sizeof(n);
    
    for (i = 1; i <= n; i++) {
        value = 1;
        for (j = 1; j <= 2 * i; j++) {
            value *= j;
        }
        for (j = 1; j <= i; j++) {
            value /= j;
        }
        for (j = 1; j <= i + 1; j++) {
            value /= j;
        }
        
        memcpy(lpMapAddress, (void *)value, sizeof(value));
        lpMapAddress += sizeof(value);
    }

    UnmapViewOfFile(lpMapAddress);
    CloseHandle(hFile);
    CloseHandle(hMapFile);

}