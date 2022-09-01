#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace::std;

int CharToInt(char c)
{
	return c - '0';
}

double StringToDoubleGamma(string s, int type)
{
	if (s.find("N/A") != string::npos)
	{
		return 0;
	}

	double number = 0;

	if (type == 1)
	{
		int posDot = 0, pos2 = 0, pos1;
		pos2 = s.find(',');
		posDot = s.find('.', pos2 + 1);
		int posComma2 = s.find(',', pos2 + 1);

		pos1 = pos2 - 1;
		while ((pos2 - pos1) == 1)
		{
			pos1 = pos2;
			pos2 = s.find(' ', pos1 + 1);
		}

		for (int i = pos1 + 1; i < pos2; i++)
		{
			if (posComma2 > posDot)
			{
				if (i != posDot)
				{
					if (s[i] == 'e' || s[i] == '+')
					{
						s[i] = '0';
					}

					int pow10;
					if (i < posDot)
						pow10 = posDot - (i + 1);
					else pow10 = posDot - i;

					number += CharToInt(s[i]) * pow(10, pow10);
				}
			}
			else
			{
				int pow10 = pos2 - (i + 1);
				number += CharToInt(s[i]) * pow(10, pow10);
			}
		}
	}

	if (type == 0)
	{
		int pos1 = s.rfind('-');
		int dist = s.length();

		if (pos1 != string::npos)
		{
			for (int i = pos1 + 2; i < dist; i++)
			{
				number += CharToInt(s[i]) * pow(10, dist - i - 1);
			}
		}

	}

	return number;
}

double StringToDoubleElectrons(string s, int type)
{
	if (s.find("N/A") != string::npos)
	{
		return 0;
	}

	double number = 0;

	if (type == 1)
	{
		int posDot = 0, pos2 = 0, pos1;

		pos2 = s.rfind(',');
		posDot = s.find('.', pos2 + 1);

		pos1 = pos2 - 1;
		while (pos2 != string::npos)
		{
			pos1 = pos2;
			pos2 = s.find(' ', pos1 + 1);
		}
		pos2 = s.rfind(',');
		if (pos2 != string::npos)
		{
			pos2 += 13;
		}
		for (int i = pos1 + 1; i < s.length(); i++)
		{
			if (posDot != string::npos)
			{
				if (i != posDot)
				{
					if (s[i] == 'e' || s[i] == '+')
					{
						s[i] = '0';
					}

					int pow10;
					if (i < posDot)
						pow10 = posDot - (i + 1);
					else pow10 = posDot - i;

					number += CharToInt(s[i]) * pow(10, pow10);
				}
			}
			else
			{
				int pow10 = s.length() - (i + 1);
				number += CharToInt(s[i]) * pow(10, pow10);
			}
		}
	}
	if (type == 0)
	{
		int pos1 = s.find('-') + 1;
		int pos2 = s.find('-', pos1 + 1);

		if (pos1 != string::npos)
		{
			for (int i = pos1 + 2; i < pos2 - 1; i++)
			{
				number += CharToInt(s[i]) * pow(10, pos2 - i - 2);
			}
		}
	}

	return number;
}

double StringToDoubleTime(string s, int Date, int DateZero, int type, bool typemilliseconds, double previous)
{
	double number = 0;

	if (type == 1)
	{
		int pos2, pos1;

		pos1 = s.find(':');
		pos2 = s.find(':', pos1 + 1);

		if (pos2 != string::npos)
		{
			number = (CharToInt(s[pos1 - 2]) * 10 + CharToInt(s[pos1 - 1])) * 3600 + (CharToInt(s[pos1 + 1]) * 10 +
				CharToInt(s[pos2 - 1])) * 60 + CharToInt(s[pos2 + 1]) * 10 + CharToInt(s[pos2 + 2]);
		}
		number += (Date - DateZero) * 86400;
	}
	if (type == 0)
	{
		int pos1 = s.find('-');
		int pos2 = s.find(',');

		if (pos1 != string::npos)
		{
			for (int i = 0; i < pos1 - 1; i++)
			{
				if (pos2 != string::npos)
				{
					if (i < pos2)
					{
						number += CharToInt(s[i]) * pow(10, pos2 - i - 1);
					}
					else if (i > pos2)
					{
						number += CharToInt(s[i]) * pow(10, pos2 - i);
					}
				}
				else
				{
					number += CharToInt(s[i]) * pow(10, pos1 - i - 2);
				}
			}
		}
	}

	if (typemilliseconds) {
		if (number == previous)
			number += 0.5;
	}

	return number;
}

int StringToDoubleDate(string s)
{
	int pos2, pos1;
	int month, day = 0;

	pos1 = s.find('-');
	pos2 = s.find('-', pos1 + 1);
	if (pos2 != string::npos)
	{
		month = CharToInt(s[pos1 + 1]) * 10 + CharToInt(s[pos1 + 2]);
		day = CharToInt(s[pos2 + 1]) * 10 + CharToInt(s[pos2 + 2]);

		for (int i = 0; i < month; i++)
		{
			if (i == 1)	day += 31;
			if (i == 2) day += 28;
			if (i == 3) day += 31;
			if (i == 4) day += 30;
			if (i == 5) day += 31;
			if (i == 6) day += 30;
			if (i == 7) day += 31;
			if (i == 8) day += 31;
			if (i == 9) day += 30;
			if (i == 10) day += 31;
			if (i == 11) day += 30;
			if (i == 12) day += 31;
		}
	}

	return day;
}

void CubicApprox(double x[], double y[], const int n, double C[])
{
	double x0 = x[0]; // ������� �������� �������
	const int m = 3;
	double xtimes[m + 1][m + 1]; // ����������� ��� C[�������][�������]
	double yxtimes[m + 1]; // ������ ��������

	for (int i = 0; i < n; i++) // �������� ����� � ��������� [0; 10]
	{
		x[i] -= x0;
	}

	for (int i = 0; i < (m + 1); i++)
	{
		for (int j = 0; j < (m + 1); j++)
		{
			xtimes[i][j] = 0;
		}
		yxtimes[i] = 0;
	}

	for (int i = 0; i < (m + 1); i++)
	{
		for (int j = 0; j < (m + 1); j++)
		{
			for (int k = 0; k < n; k++)
			{
				xtimes[i][j] += pow(x[k], j + i);
				if (j == 0)
				{
					yxtimes[i] += y[k] * pow(x[k], i);
				}
			}
		}
	}	// ������ n, summ(x), summ(x^2), ...

	for (int i = 0; i < m; i++)
	{
		int k, j;
		double M = 0;
		for (j = 0; j < m + 1; j++)
		{
			if (j > i)
			{
				M = xtimes[j][i] / xtimes[i][i];
				for (k = 0; k < m + 1; k++)
				{
					xtimes[j][k] -= xtimes[i][k] * M;
				}
				yxtimes[j] -= yxtimes[i] * M;
			}
		}
	} // ������ ���� ����� �� ���������

	for (int i = m; i > (0 - 1); i--) // i � �������, � �������� (����� ������������, ������� ����)
	{
		for (int j = i; j < m; j++) // j-m � ���������� ���������, ������� �������� ����� ��������
		{
			yxtimes[i] -= C[j + 1] * xtimes[i][j + 1];
		}
		C[i] = yxtimes[i] / xtimes[i][i];
	}
	/*
	C[0] += -C[1] * x0 + C[2] * x0 * x0 - C[3] * pow(x0, 3);
	C[1] += -2 * C[2] * x0 + 3 * C[3] * pow(x0, 2);
	C[2] += -3 * C[3] * x0;
	*/
	for (int i = 0; i < n; i++)
	{
		x[i] += x0;
	}
}

double ApproximatedValue(double x_0, double x_21, double C[], int m)
{
	double Value = 0;
	double x = x_21 - x_0;
	for (int i = 0; i < m + 1; i++)
	{
		Value += C[i] * pow(x, i);
	}

	return Value;
}

double Sigma(double x[], double y[], const int n, double C[], const int m)
{
	double sigm = 0;
	double u = 0;
	double x0 = x[0];

	for (int i = 0; i < n; i++)
	{
		x[i] -= x0;
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m + 1; j++)
		{
			u += C[j] * pow(x[i], j);
		}
		sigm += pow(y[i] - u, 2);
		u = 0;
	}

	sigm = pow(sigm / (n - 1), 0.5);

	for (int i = 0; i < n; i++)
	{
		x[i] += x0;
	}

	return sigm;
}

double NumOfSigma(long double y, double sigm, double Approximated)
{
	double Num;
	Num = (y - Approximated) / sigm;

	return Num;
}

int main()
{
	setlocale(LC_ALL, "rus");

	string path_out = "D:\\������� ������\\������\\�����\\������-���\\������. 24 ���. ����������.txt";
	//string path_numOfTriggers = "D:\\������� ������\\������\\�����\\������-����\\���������� ������������ ��������. ������-����.txt";
	string path_in = "D:\\������� ������\\������\\�����\\������-���\\������. 24 ���. ��������������.txt";

	//string path_approx = "D:\\������� ������\\������\\������\\������-���\\�����. ������������� 3D(1).txt";
	//ofstream fout_approx(path_approx, ofstream::trunc);

	string path_NumOfTriggers = "D:\\������� ������\\������\\�����\\������-���\\������. 24 ���. ���������� ������������.txt";
	ofstream fout_NumOfTriggers(path_NumOfTriggers, ofstream::trunc);

	ofstream foutCubic(path_out, ofstream::trunc);
	foutCubic << "I_el" << '\t' << "I_gam" << '\t' << "Aprx_I_El" << '\t' << "Aprx_I_gam" << '\t' << "sigm_el" << '\t' <<
		"sigm_gam" << '\t' << "N_sigm_el" << '\t' << "N_sigm_gam" << '\t' << "Time" << '\t' << "N_gam / N_el" << endl;
	foutCubic.close();

	ifstream fin(path_in, ifstream::in);

	if (fin.is_open())
	{
		cout << "���� ��� ������ ������� ������" << endl;

		int count_time = 0;

		string buff;
		int count = 0;
		const int n = 20;
		double electrons[n + 1];
		double gamma[n + 1];
		double Time[n + 1];
		double TimeZero = 0;
		int DateZero = 0;
		int Date = 0;
		int trigger = 12;
		int countTrigger[21];
		bool typemilliseconds = true;

		for (int i = 0; i < n + 1; i++)
		{
			electrons[i] = 0;
			gamma[i] = 0;
			Time[i] = 0;
			countTrigger[i] = 0;
		}

		ofstream foutCubic(path_out, ofstream::app);

		while (!fin.eof())
		{
			count++;
			getline(fin, buff);

			if (count == 10)
			{
				DateZero = StringToDoubleDate(buff);
				TimeZero = StringToDoubleTime(buff, DateZero, DateZero, 1, typemilliseconds, 0);
			}
			if (count > 10)
			{
				double ApproxElectronsCubic = 0, ApproxGammaCubic = 0;

				const int mCubic = 3; // ��� ���������� �������������
				double CelectronsCubic[mCubic + 1]; double CgammaCubic[mCubic + 1]; // ������������ �������� C[0] + C[1]*x + C[2]*x^2 + C[3]*x^3

				double electronsSigmaCubic = 0, gammaSigmaCubic = 0;

				double electronsNumOfSigmaCubic = 0, gammaNumOfSigmaCubic = 0;

				double X[n + 1];

				for (int i = 0; i < n; i++)
				{
					electrons[i] = electrons[i + 1];
					gamma[i] = gamma[i + 1];
					Time[i] = Time[i + 1];
					X[i] = Time[i] - TimeZero;
				}

				CubicApprox(X, electrons, n, CelectronsCubic);
				CubicApprox(X, gamma, n, CgammaCubic);

				if (gamma[n] - 0 > 0.00001)
				{
					count_time += 1;
				}

				electronsSigmaCubic = Sigma(X, electrons, n, CelectronsCubic, mCubic);
				gammaSigmaCubic = Sigma(X, gamma, n, CgammaCubic, mCubic);

				double previous = Time[n - 1];

				electrons[n] = StringToDoubleElectrons(buff, 1);
				gamma[n] = StringToDoubleGamma(buff, 1);
				Date = StringToDoubleDate(buff);
				Time[n] = StringToDoubleTime(buff, Date, DateZero, 1, typemilliseconds, previous);
				X[n] = Time[n] - TimeZero;

				/*if (abs(X[n] - ) < 0.001)  //������������� ���������
				{
					fout_approx << "Time" << '\t' << "Apprx_gam" << '\t' << "gam" << '\t' << "Apprx_el" << '\t' << "el" << endl;
					for (int i = 0; i < n + 1; i++)
					{
						double intens_gam = 0;
						double intens_el = 0;
						for (int j = 0; j < mCubic + 1; j++)
						{
							intens_el += pow(X[i] - X[0], j) * CelectronsCubic[j];
							intens_gam += pow(X[i] - X[0], j) * CgammaCubic[j];
						}
						fout_approx << fixed << X[i] << '\t' << intens_gam << '\t' << gamma[i] << '\t' << intens_el << '\t' << electrons[i] << endl;
					}
				}*/

				ApproxElectronsCubic = ApproximatedValue(X[0], X[n], CelectronsCubic, mCubic);
				ApproxGammaCubic = ApproximatedValue(X[0], X[n], CgammaCubic, mCubic);

				electronsNumOfSigmaCubic = NumOfSigma(electrons[n], electronsSigmaCubic, ApproxElectronsCubic);
				gammaNumOfSigmaCubic = NumOfSigma(gamma[n], gammaSigmaCubic, ApproxGammaCubic);

				double triggerLevel = trigger * gammaSigmaCubic + ApproxGammaCubic;

				if (count > 40 && Time[n] > 0)
				{
					foutCubic << fixed << electrons[n] << '\t' << gamma[n] << '\t' << ApproxElectronsCubic << '\t' << ApproxGammaCubic << '\t'
						<< electronsSigmaCubic << '\t' << gammaSigmaCubic << '\t' << electronsNumOfSigmaCubic << '\t' << gammaNumOfSigmaCubic
						<< '\t' << X[n] << '\t' << gammaNumOfSigmaCubic / electronsNumOfSigmaCubic <<
						'\t' << triggerLevel;


					if (Time[n] - Time[0] < 106) // �������
					{
						if (gamma[n - 5] < 200)
						{
							for (int i_trigger = 0; i_trigger < n + 1; i_trigger++)
							{
								if (electronsNumOfSigmaCubic < 0 && gammaNumOfSigmaCubic > i_trigger)
								{
									countTrigger[i_trigger] += 1;

									if (i_trigger == trigger)
									{
										foutCubic << '\t' << gammaNumOfSigmaCubic;

										if (electronsNumOfSigmaCubic < 0)
											foutCubic << '\t' << gammaNumOfSigmaCubic;
									}
								}
								else
								{
									if ((gammaNumOfSigmaCubic > i_trigger) && (gammaNumOfSigmaCubic / electronsNumOfSigmaCubic > i_trigger))
									{
										countTrigger[i_trigger] += 1;
										if (i_trigger == trigger)
											foutCubic << '\t' << gammaNumOfSigmaCubic;
									}
									else
									{
										if (i_trigger == trigger)
											foutCubic << '\t' << '0' << '\t' << '0';
									}
								}
							}
						}
					}
					else
					{
						foutCubic << '\t' << '0' << '\t' << '0';
					}
					foutCubic << endl;
				}

			}
		}


		cout << count_time << endl;
		foutCubic.close();
		fin.close();
		//fout_approx.close();

		for (int i = 0; i < 21; i++)
		{
			fout_NumOfTriggers << i << '\t' << countTrigger[i] << endl;
		}
		fout_NumOfTriggers.close();

		cout << "���������� ������������ " << countTrigger[trigger] << endl;
	}
	else
	{
		cout << "��� �������� ����� ��������� ������" << endl;
	}

	return 0;
}
