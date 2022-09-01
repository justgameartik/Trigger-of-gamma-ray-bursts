#include <iostream>
#include <fstream>
#include <string>

using namespace::std;

void AddTabulation()
{
	const int n1 = 60;
	const int n2 = 52;
}

string AddComma(string s)
{
	string result;
	bool iscomma = false;
	if (s.length() > 2)
	{
		for (int i = 0; i < s.length() - 1; i++)
		{
			if (s[i] == ' ' && s[i + 1] == ' ')
			{
				if (iscomma)
					result += "   ,  ";
				iscomma = false;
				continue;
			}
			else {
				result += s[i];
				iscomma = true;
			}
			if (i == s.length() - 2)
				result += s[i + 1];
		}
	}

	return result;
}


int main()
{
	setlocale(LC_ALL, "rus");
	
	string path_in = "D:\\������� ������\\������������� �������\\������. 24 ���. �� ����������.txt";
	string path_temp = "D:\\������� ������\\������������� �������\\������. 24 ���. �� ��������������.txt";
	string path_out = "D:\\������� ������\\������������� �������\\������. 24 ���. ��������������.txt";
	{
		ifstream fin(path_in, ifstream::in);
		ofstream fout(path_temp, ofstream::trunc);

		if (fin.is_open())
		{
			cout << "���� ��� ������ ������� ������" << endl;

			string buff;
			char ch;

			while (fin.get(ch))
			{
				fout << ch;
			}

		}
		else cout << "��������� ������ ��� ������ �����" << endl;

		fin.close();
		fout.close();
	}


	ifstream fin(path_temp, ifstream::in);
	ofstream fout(path_out, ofstream::trunc);
	
	if (fin.is_open())
	{
		cout << "������ ���� ��� ������ ������� ������" << endl;

		string buff;

		while (!fin.eof())
		{
			getline(fin, buff);
			fout << AddComma(buff) << endl;
		}
	}
	else cout << "��������� ������ ��� ������ ������� �����" << endl;

	fin.close();
	fout.close();

	return 1;
}