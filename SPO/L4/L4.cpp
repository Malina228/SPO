//  В2

#include <iostream>
#include <windows.h>
#include "time.h"
#include "conio.h"


using namespace std;
LONG mass[10];

DWORD WINAPI thread1_F(LPVOID p)
{
	srand(time(NULL));
	LONG random;
	while (1)
	{
		Sleep(1000);
		for (int i = 0; i < 10; i++)
		{
			random = rand() % 300 - 150;
			InterlockedExchange(mass + i, random);
		}
		for (int i = 0; i < 10; i++)
		{
			cout << mass[i] << "  ";
		}
		cout << endl;
	}
	return 1;
}


DWORD WINAPI thread2_F(LPVOID)
{
	while (true)
	{
		for (int i = 0; i < 10; i++)
		{
			if (mass[i] == 0)
			{
				LONG temp = NULL;
				for (int j = 0; j < 10; j++)
				{
					if (mass[j] != 0)
						temp = mass[j];
				}
				InterlockedExchange(mass + i, temp);
			}
		}
	}
	return 1;
}

int main()
{
	setlocale(LC_ALL, "rus");
	HANDLE thread1 = CreateThread(NULL, 0, thread1_F, NULL, NULL, 0);
	HANDLE thread2 = CreateThread(NULL, 0, thread2_F, NULL, NULL, 0);
	while (!_kbhit())
	{
	}
	CloseHandle(thread1);
	CloseHandle(thread2);
	return 1;
}
