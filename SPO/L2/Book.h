#include <iostream>

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

	const int get_year() const { return iYear; }

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
