#include<windows.h>
#include<stdio.h>

int main(int argc, char *argv[]) {
    HANDLE hMapFile;
    LPVOID lpMapAddress;
    int n, value, i;
    
    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, TEXT("SharedObject"));

    if (hMapFile == NULL) {
        printf("Could not open file mapping object (%d).\n", GetLastError());
        return -1;
    }

    lpMapAddress = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);

    if (lpMapAddress == NULL) {
        printf("Could not map view of file (%d).\n", GetLastError());
        return -1;
    }
    
    memcpy((void *)n, lpMapAddress, sizeof(n));
    lpMapAddress += sizeof(n);
    
    for (i = 1; i <= n; i++) {
        memcpy((void *)value, lpMapAddress, sizeof(value));
        lpMapAddress += sizeof(value);
        
        printf("%d", value);
    }
    
    UnmapViewOfFile(lpMapAddress);
    CloseHandle(hMapFile);
}