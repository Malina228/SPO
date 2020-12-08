// В2

#include <iostream>
#include <windows.h>
#include "time.h"

using namespace std;

const int N = 10;
HANDLE semafor;


DWORD WINAPI thread1_F(LPVOID mass)
{
	srand(time(0));
	for (int i = 0; i < N; i++)
	{
		*((int*)mass + i) = rand() % 500;
		Sleep(1000);
		cout << *((int*)mass + i) << "  ";
		ReleaseSemaphore(semafor, 3, NULL);
		Sleep(200);
	}
	return 1;
}


DWORD WINAPI thread2_F(LPVOID mass)
{
	int sum = 0;
	int counter = 0;
	for (int i = 0; i < N; i++)
	{
		WaitForSingleObject(semafor, INFINITE);
		sum += *((int*)mass + i);
		counter++;
		cout << "(" << sum / counter << ") ";
	}
	return 1;
}

DWORD WINAPI thread3_F(LPVOID mass)
{
	int max = *((int*)mass + 0);
	for (int i = 0; i < N; i++)
	{
		WaitForSingleObject(semafor, INFINITE);
		if (*((int*)mass + i) > max)
			max = *((int*)mass + i);
		Sleep(500);
	}

	cout << "max: " << max << endl;

	return 1;
}

DWORD WINAPI thread4_F(LPVOID mass)
{
	int min = *((int*)mass + 0);
	for (int i = 0; i < N; i++)
	{
		WaitForSingleObject(semafor, INFINITE);
		if (*((int*)mass + i) < min)
			min = *((int*)mass + i);
		Sleep(500);
	}

	cout << "min: " << min << endl;

	return 1;
}


int main()
{
	int mass[N];
	setlocale(LC_ALL, "rus");
	semafor = CreateSemaphore(NULL, 0, 3, L"semafor");
	HANDLE thread1 = CreateThread(NULL, 0, thread1_F, (void*)mass, NULL, 0);
	HANDLE thread2 = CreateThread(NULL, 0, thread2_F, (void*)mass, NULL, 0);
	HANDLE thread3 = CreateThread(NULL, 0, thread3_F, (void*)mass, NULL, 0);
	HANDLE thread4 = CreateThread(NULL, 0, thread4_F, (void*)mass, NULL, 0);
	HANDLE Threads[4] = { thread1, thread2, thread3, thread4 };
	WaitForMultipleObjects(4, Threads, TRUE, INFINITE);
	for (int i = 0; i < 4; i++)
	{
		CloseHandle(Threads[i]);
	}
	return 1;
}
