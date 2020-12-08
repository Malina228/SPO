//  В6

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <sstream>

#include "Book.h"

using namespace std;

int main()
{
    SECURITY_ATTRIBUTES saFile;
    saFile.nLength = sizeof(SECURITY_ATTRIBUTES);
    saFile.lpSecurityDescriptor = NULL;
    saFile.bInheritHandle = TRUE;

    char file_name[] = "data_file.dat";
    HANDLE hFile = CreateFile(LPCWSTR(file_name), GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, &saFile, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    cout << "Information from bat file:" << endl;
    DWORD cBytes;
    Book tmp;
    while (ReadFile(hFile, &tmp, sizeof(tmp), &cBytes, NULL) && cBytes != 0)
    {
        cout << tmp << endl;
    }

    cout << "Enter the number of structure that should be changed:" << endl;
    int struct_number;
    cin >> struct_number;
    cin.ignore(32767, '\n');
    cin.clear();

    char cmd[124];
    wsprintf(LPWSTR(cmd), TEXT("DaughterProc.exe %d %d"), hFile, struct_number);

    PROCESS_INFORMATION pi;
    STARTUPINFO         si;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    if (!CreateProcess(NULL, LPWSTR(cmd), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
    {
        cerr << "There is an error while daughter process creating: " << GetLastError() << endl;
        CloseHandle(hFile);
        return 0;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    SetFilePointer(hFile, 0, 0, FILE_BEGIN);
    system("cls");
    cout << "Changed information:\n" << endl;
    while (ReadFile(hFile, &tmp, sizeof(tmp), &cBytes, NULL) && cBytes != 0)
    {
        cout << tmp << endl;
    }

    CloseHandle(hFile);
    return 0;
}
