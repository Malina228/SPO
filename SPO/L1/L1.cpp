//  В6

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

class Book
{
private:
	char		szAuthor[20];
	char		szName[20];
	unsigned	cPages;
	unsigned	iYear;

public:
	Book() : szAuthor(""), szName(""), cPages(0), iYear(0) {}
	Book(const Book& source) : cPages(source.cPages), iYear(source.iYear)
	{
		strcpy(szAuthor, source.szAuthor);
		strcpy(szName, source.szName);
	}

	const int GetYear() const { return iYear; }

	friend ostream& operator<<(ostream&, const Book&);
	friend istream& operator>>(istream&, Book&);
};

ostream& operator<<(ostream& out, const Book& tmp)
{
	out << "Author name: " << tmp.szAuthor << "\nBook name: " << tmp.szName
		<< "\nNumber of pages: " << tmp.cPages << "\nPublication year: " << tmp.iYear << endl;
	return out;
}

istream& operator>>(istream& in, Book& tmp)
{
	cout << "Enter the author name:" << endl;
	in.get(tmp.szAuthor, 20);
	cin.ignore(32767, '\n');
	cout << "Enter the book name:" << endl;
	in.get(tmp.szName, 20);
	cin.ignore(32767, '\n');
	cout << "Enter the number of pages:" << endl;
	in >> tmp.cPages;
	cin.ignore(32767, '\n');
	cout << "Enter the publication year:" << endl;
	cin >> tmp.iYear;
	cin.ignore(32767, '\n');
	return in;
}

int main()
{
	DWORD dwBytes;
	LPCWSTR szFileName = TEXT("data_file.dat");
	HANDLE hDataFile = CreateFile(
		szFileName,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hDataFile == INVALID_HANDLE_VALUE)
	{
		cerr << "Error while file creating!" << endl;
		cout << GetLastError() << endl;
		exit(1);
	}
	else
		cout << "File was created." << endl;

	char choice = ' ';
	Book tmp;
	do
	{
		cout << "Enter the information about book" << endl;
		cin >> tmp;
		WriteFile(hDataFile, &tmp, sizeof(tmp), &dwBytes, NULL);
		if (dwBytes != sizeof(tmp))
		{
			cerr << "There is an error while writing: " << GetLastError() << endl;
			break;
		}
		do
		{
			cout << "Continue information filling? (y/n)" << endl;
			cin >> choice;
			cin.ignore(32767, '\n');

		} while ((tolower(choice) != 'y') && (tolower(choice) != 'n'));
	} while (tolower(choice) == 'y');

	system("cls");
	FlushFileBuffers(hDataFile);
	SetEndOfFile(hDataFile);

	SetFilePointer(hDataFile, 0, 0, FILE_BEGIN);
	cout << "Enter key-year:" << endl;
	int iKeyYear;
	cin >> iKeyYear;
	cin.ignore(32767, '\n');
	system("cls");

	ofstream resultFile("result.txt", std::ofstream::out);

	while (ReadFile(hDataFile, &tmp, sizeof(tmp), &dwBytes, NULL) && dwBytes != 0)
	{
		if (iKeyYear < tmp.GetYear())
		{
			cout << "##################################" << endl << tmp << endl;
			resultFile << "##################################" << endl << tmp << endl;
		}
	}

	resultFile.close();
	CloseHandle(hDataFile);
	return 0;
}