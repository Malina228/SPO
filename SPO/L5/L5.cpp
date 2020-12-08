// В2

#include <iostream>
#include <windows.h>
#include "time.h"
using namespace std;


DWORD WINAPI thread3_F(LPVOID mass)
{
	HANDLE thread_mutex = OpenMutex(SYNCHRONIZE, FALSE, L"Mutex");
	WaitForSingleObject(thread_mutex, INFINITE);
	int summa = 0;
	cout << "загрузился поток 3" << endl;

	for (int i = 0; i < 10; i++)
	{
		summa += *((int*)mass + i);
	}
	cout << "сумма: " << summa << endl;
	cout << endl << endl;
	ReleaseMutex(thread_mutex);
	return 1;
}

DWORD WINAPI thread2_F(LPVOID mass)
{
	HANDLE thread_mutex = OpenMutex(SYNCHRONIZE, FALSE, L"Mutex");
	WaitForSingleObject(thread_mutex, INFINITE);
	if (*(int*)mass > *((int*)mass + 9))
	{
		int temp;
		temp = *((int*)mass + 9);
		*((int*)mass + 9) = *(int*)mass;
		*(int*)mass = temp;
	}
	cout << "загрузился поток 2" << endl;
	cout << "измененный массив: ";

	for (int i = 0; i < 10; i++)
	{
		cout << *((int*)mass + i) << "  ";
	}
	cout << endl << endl;
	ReleaseMutex(thread_mutex);
	return 1;
}


DWORD WINAPI thread1_F(LPVOID mass)
{
	HANDLE thread_mutex = OpenMutex(SYNCHRONIZE, FALSE, L"Mutex");
	WaitForSingleObject(thread_mutex, INFINITE);
	srand(time(0));
	Sleep(1000);
	for (int i = 0; i < 10; i++)
	{
		*((int*)mass + i) = rand() % 100;
	}
	cout << "загрузился поток 1" << endl;
	cout << "исходный массив: ";

	for (int i = 0; i < 10; i++)
	{
		cout << *((int*)mass + i) << "  ";
	}
	cout << endl;
	ReleaseMutex(thread_mutex);
	return 1;
}



int main()
{
	HANDLE Mutex;
	setlocale(LC_ALL, "rus");
	int mass[10];
	Mutex = CreateMutex(NULL, FALSE, L"Mutex");
	HANDLE thread1 = CreateThread(NULL, 0, thread1_F, (void*)mass, NULL, 0);
	HANDLE thread2 = CreateThread(NULL, 0, thread2_F, (void*)mass, NULL, 0);
	HANDLE thread3 = CreateThread(NULL, 0, thread3_F, (void*)mass, NULL, 0);
	HANDLE Threads[3] = { thread1, thread2, thread3 };
	WaitForMultipleObjects(3, Threads, TRUE, INFINITE);
	CloseHandle(Mutex);
	for (int i = 0; i < 3; i++)
	{
		CloseHandle(Threads[i]);
	}
	return 1;
}
