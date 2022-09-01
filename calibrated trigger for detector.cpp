#include <math.h>

// входные данные
struct input {
	const int m = 3; // порядок полинома
	const int n = 20; // количество рассматриваемых точек при аппроксимации
	double x[21], time[21]; // время, приведенное к интервалу [0; 100], и просто время
	long double y_el[21], y_gam[21]; // интенсивности по электронам и по гамма
	int trigger_value; // пороговое значение для триггера
};

// то, что считается
struct result {
	double approx_el_I, approx_gam_I; // Приближенно посчитанные значения для интенсивности
	double el_sigm, gam_sigm; // стандартные отклонения
	double num_sigm_el, num_sigm_gam; // количество стандартных отклонений
	double time, relation; // время и отношение количества стандартных отклонений по гамма к количеству стандартных отклонений по электронам
	double c_el[4], c_gam[4]; // коэффициенты в полиноме
};

// Кубическая аппроксимация
void CubicApprox(double x[], long double y[], const int n, double C[])
{
	double x0 = x[0]; // Нулевое значение времени
	const int m = 3; // порядок полинома
	double xtimes[m + 1][m + 1]; // коэффициент при C[строчка][столбец] в СЛАУ
	double yxtimes[m + 1]; // правые значения в СЛАУ

	for (int i = 0; i < n; i++) // Приводим время к интервалу [0; 100]
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
	}	// Найдем n, summ(x), summ(x^2), ...

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
	} // делаем нули слева от диагонали

	for (int i = m; i > (0 - 1); i--) // i — строчка, с которой работаем (номер коэффициента, который ищем)
	{
		for (int j = i; j < m; j++) // j-m — количество элементов, которые вычитаем перед делением
		{
			yxtimes[i] -= C[j + 1] * xtimes[i][j + 1];
		}
		C[i] = yxtimes[i] / xtimes[i][i];
	}

	C[0] += -C[1] * x0 + C[2] * x0 * x0 - C[3] * pow(x0, 3);
	C[1] += -2 * C[2] * x0 + 3 * C[3] * pow(x0, 2);
	C[2] += -3 * C[3] * x0;

	for (int i = 0; i < n; i++) // возвращаем первоначальные значения времени
	{
		x[i] += x0;
	}
}

double ApproximatedValue(double x, double C[], int m) // значение для 21-ой точки, посчитанное из аппроксимации
{
	double Value = 0;
	for (int i = 0; i < m + 1; i++)
	{
		Value += C[i] * pow(x, i);
	}

	return Value;
}

double Sigma(double x[], long double y[], const int n, double C[], const int m) // считает стандартное отклонение
{
	double sigm = 0;
	double u = 0;

	for (int i = 0; i < n;i++)
	{
		for (int j = 0; j < m + 1; j++)
		{
			u += C[j] * pow(x[i], j);
		}
		sigm += pow(y[i] - u, 2);
		u = 0;
	}

	sigm = pow(sigm / (n - 1), 0.5);

	return sigm;
}

// считает количество стандартных отклонение в разнице между фактическим и предсказанным значениями
double NumOfSigma(long double y, double sigm, double Approximated)
{
	double Num;
	Num = (y - Approximated) / sigm;

	return Num;
}

bool Calculate(struct input in, struct result* R) // ОСНОВНАЯ ФУНКЦИЯ / в struct result будут все посчитанные значения
{
	CubicApprox(in.x, in.y_gam, in.n, R->c_gam);
	CubicApprox(in.x, in.y_el, in.n, R->c_el);

	R->approx_gam_I = ApproximatedValue(in.x[20], R->c_gam, in.m);
	R->approx_el_I = ApproximatedValue(in.x[20], R->c_el, in.m);

	R->gam_sigm = Sigma(in.x, in.y_gam, in.n, R->c_gam, in.m);
	R->el_sigm = Sigma(in.x, in.y_el, in.n, R->c_el, in.m);

	R->num_sigm_gam = NumOfSigma(in.y_gam[20], R->gam_sigm, R->approx_gam_I);
	R->num_sigm_el = NumOfSigma(in.y_el[20], R->el_sigm, R->approx_el_I);
	R->relation = R->num_sigm_gam / R->num_sigm_el;
	R->time = in.x[20];

	return trigger(in.y_gam[15], in.trigger_value, R.num_sigm_el, R.num_sigm_gam)
}

bool trigger(double gam_15, int trigger_value, double el_num_sigm, double gam_num_sigm)
{
	if (gam_15 < 200)
	{
		if (el_num_sigm < 0 && gam_num_sigm > trigger_value)
			return true;
		else
			if (gam_num_sigm > trigger_value && (gam_num_sigm / el_num_sigm > trigger_value))
				return true;
			else 
				return false;
	}
	else
		return false;
}

int main()
{
	struct input in;
	struct result out;
	// Пример, как использовать
	return Calculate(in, &out); // Вернет true или false;
	
	
	//  Все посчитанные значения хранятся в out
}
