//  В2

#include <iostream>
#include <windows.h>
#include "conio.h"

using namespace std;

int counter1 = 0;
int counter2 = 0;
int counter3 = 0;

DWORD WINAPI thread1_F(LPVOID p)
{
	while (true)
	{
		counter1++;
		cout << "поток 1: " << counter1 << endl;
		Sleep(200);
	}
	return 1;
}


DWORD WINAPI thread2_F(LPVOID p)
{
	int checker = 0;
	while (true)
	{
		if (counter2 > 80 && checker == 0)
		{
			ResumeThread(p);
			checker = 1;
		}
		counter2++;
		cout << "поток 2: " << counter2 << endl;
		Sleep(100);
	}
	return 1;
}


DWORD WINAPI thread3_F(LPVOID p)
{
	while (true)
	{
		counter3++;
		cout << "поток 3: " << counter3 << endl;
		Sleep(400);
	}
	return 1;
}


int main()
{
	setlocale(LC_ALL, "rus");
	HANDLE thread1 = CreateThread(NULL, 0, thread1_F, NULL, NULL, 0);
	HANDLE thread2 = CreateThread(NULL, 0, thread2_F, (void*)thread1, NULL, 0);
	HANDLE thread3 = CreateThread(NULL, 0, thread3_F, NULL, NULL, 0);
	SuspendThread(thread1);
	char ch;

	while ((ch = _getche()) != 'q')
	{
	}

	CloseHandle(thread1);
	CloseHandle(thread2);
	CloseHandle(thread3);
}
