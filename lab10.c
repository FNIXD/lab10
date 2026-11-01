#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <locale.h>

#define PI 3.14159265358979
#define MAX_TERMS 1000
#define TABLE_ROWS 15

double f1(double x);
double f2(double x);
double tan_n(double x, int n);
double tan_eps(double x, double eps);
    int main(void)
{
    int choice;
    int i;
    double x, eps;
    double a, b;
    double exact, approx;

    setlocale(LC_ALL, ".UTF8");

    /*  Задание 1. Две функции и меню  */
    printf("=== Задание 1 ===\n");
    printf("F1(x) = x^2 - 3x + 9 при x <= 3, F1(x) = 1 / (x^3 + 3) при x > 3\n");
    printf("F2(x) = x * e^(sin(x^2))\n");
    do
    {
        printf("\nМеню:\n");
        printf(" 1 - значение F1(x)\n");
        printf(" 2 - значение F2(x)\n");
        printf(" 3 - произведение F1(x) * F2(x)\n");
        printf(" 4 - разность квадратов F1(x)^2 - F2(x)^2\n");
        printf(" 5 - удвоенная сумма 2 * (F1(x) + F2(x))\n");
        printf(" 6 - все значения сразу\n");
        printf(" 0 - переход к заданию 2\n");
        printf("Выберите пункт: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Ошибка: нужно ввести номер пункта\n");
            return 1;
        }

        if (choice >= 1 && choice <= 6)
        {
            printf("Введите x: ");
            if (scanf("%lf", &x) != 1)
            {
                printf("Ошибка: нужно ввести число\n");
                return 1;
            }
            a = f1(x);
            b = f2(x);

            switch (choice)
            {
            case 1:
                printf("F1(x) = %lf\n", a);
                break;
            case 2:
                printf("F2(x) = %lf\n", b);
                break;
            case 3:
                printf("произведение = %lf\n", a * b);
                break;
            case 4:
                printf("разность квадратов = %lf\n", a * a - b * b);
                break;
            case 5:
                printf("удвоенная сумма = %lf\n", 2 * (a + b));
                break;
            case 6:
                printf("F1(x) = %lf, F2(x) = %lf\n", a, b);
                printf("произведение = %lf, разность квадратов = %lf, удвоенная сумма = %lf\n",
                    a * b, a * a - b * b, 2 * (a + b));
                break;
            }
        }
        else if (choice != 0)
            printf("Ошибка: такого пункта нет\n");
    } while (choice != 0);
    printf("\n");

    /*  Задание 2. Вычисление tg(x) тремя способами (вариант 8)  */
    printf("=== Задание 2 ===\n");
    printf("tg(x) = x + x^3/3 + 2x^5/15 + 17x^7/315 + ..., |x| < pi/2\n");
    printf("Введите x: ");
    if (scanf("%lf", &x) != 1 || fabs(x) >= PI / 2)
    {
        printf("Ошибка: нужно ввести число, по модулю меньшее pi/2\n");
        return 1;
    }

    exact = tan(x);
    printf("tan(x) из стандартной библиотеки = %.10f\n\n", exact);

    printf("   N |  tan_N  | отклонение\n");
    for (i = 1; i <= TABLE_ROWS; i++)
    {
        approx = tan_n(x, i);
        printf("%4d | %7.4f | %+.4e\n", i, approx, approx - exact);
    }

    printf("\nВведите точность eps: ");
    if (scanf("%lf", &eps) != 1 || eps <= 0)
    {
        printf("Ошибка: eps должно быть положительным числом\n");
        return 1;
    }

    approx = tan_eps(x, eps);
    printf("tan_eps = %.10f, отклонение = %+.4e\n", approx, approx - exact);

    return 0;
}

double f1(double x)
{
    double y;

    if (x <= 3)
        y = x * x - 3 * x + 9;
    else
        y = 1 / (x * x * x + 3);

    return y;
}

double f2(double x)
{
    double y;
    y = x * exp(sin(x * x));
    return y;
}

/* tg(x): сумма первых n членов ряда Маклорена.
   Коэффициенты c[k] при x^(2k-1) находятся из равенства tg'(x) = 1 + tg(x)^2:
   c[1] = 1, c[k] = (c[1]*c[k-1] + c[2]*c[k-2] + ... + c[k-1]*c[1]) / (2k - 1) */
double tan_n(double x, int n)
{
    double c[MAX_TERMS + 1];
    double sum = 0;
    double power = x;
    double s;
    int k, p;

    if (n > MAX_TERMS)
        n = MAX_TERMS;

    for (k = 1; k <= n; k++)
    {
        if (k == 1)
            c[k] = 1;
        else
        {
            s = 0;
            for (p = 1; p < k; p++)
                s += c[p] * c[k - p];
            c[k] = s / (2 * k - 1);
        }

        sum += c[k] * power;
        power = power * x * x;
    }

    return sum;
}

/* tg(x): суммирование до тех пор, пока очередной член не станет меньше eps */
double tan_eps(double x, double eps)
{
    double c[MAX_TERMS + 1];
    double sum = 0;
    double power = x;
    double term;
    double s;
    int k = 1;
    int p;

    do
    {
        if (k == 1)
            c[k] = 1;
        else
        {
            s = 0;
            for (p = 1; p < k; p++)
                s += c[p] * c[k - p];
            c[k] = s / (2 * k - 1);
        }

        term = c[k] * power;
        sum += term;
        power = power * x * x;
        k++;
    } while (fabs(term) >= eps && k <= MAX_TERMS);

    return sum;
}