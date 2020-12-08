#include <Windows.h>
#include <iostream>

#include "Book.h"

using namespace std;

DWORD WINAPI GetProcessIDbyProcessHandle(HANDLE hProcess)
{
    // [in]  process handle
    // [out] process ID, or 0xffffffff in case of failure
    
    if (hProcess == NULL)    return 0xffffffff;
    LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)
        GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "GetCurrentProcessId");
    if (lpStartAddress == NULL) return 0xffffffff;
    // We do not know, whether process handle already has required access rights;

    // thus we have to duplicate it
    HANDLE hProcessAccAdj;
    BOOL bRes = DuplicateHandle(GetCurrentProcess(), 
                                hProcess, GetCurrentProcess(), &hProcessAccAdj, 
                                PROCESS_QUERY_INFORMATION|PROCESS_CREATE_THREAD|
                                PROCESS_VM_OPERATION|PROCESS_VM_WRITE, 
                                FALSE, 0);
    if (!bRes || hProcessAccAdj == NULL)
    {
        UINT unError = GetLastError();
        return 0xffffffff;
    }
    // Create a remote thread; as its starting address 

    // we specify GetCurrentProcessId() address,
    // which is the same for all processes. Note that GetCurrentProcessId() has no input
    // parameters, and we don't care about our thread stack cleanup,
    // as it will be destroyed right after this call

    DWORD dwThreadID;
    HANDLE hRemoteThread = CreateRemoteThread(hProcessAccAdj, NULL, 
        0, lpStartAddress, 0, 0, &dwThreadID);
    CloseHandle(hProcessAccAdj);
    if (hRemoteThread == NULL) return 0xffffffff;
    // Wait until process ID is obtained

    // (replace INFINITE value below to a smaller value to avoid deadlocks);
    // then get the thread exit code, which is a value returned by GetCurrentProcessId()
    // in the context of the remote process
    WaitForSingleObject(hRemoteThread, INFINITE);
    DWORD dwExitCode;
    if (GetExitCodeThread(hRemoteThread, &dwExitCode) == 0)    dwExitCode = 0xffffffff;
    CloseHandle(hRemoteThread);
	return dwExitCode;
}

int main(int argc, char* argv[])
{
  cout << "Param: " << argv[1] << endl;
  cout << "GetCurrentProcess: " << GetProcessIDbyProcessHandle(GetCurrentProcess()) << endl;
  cout << "Reenter the " << argv[2] << " element of database:" << endl;
  Book tmp;
  cin >> tmp;
  int position;
  position = atoi(argv[2]);
  HANDLE hFile = (HANDLE)atoi(argv[1]);
  SetFilePointer(hFile, (position - 1) * sizeof(Book), 0, FILE_BEGIN);
  DWORD cBytes;
  WriteFile(hFile, &tmp, sizeof(tmp), &cBytes, NULL);
  if(cBytes == sizeof(tmp))
    cout << "Infomation was changed" << endl;
  else
    cerr << "Information was not changed correctly" << endl;
  FlushFileBuffers(hFile);
  system("pause");
  return 0;
}
