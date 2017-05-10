#include <QString>
#include <QTextStream>
#include <QVector>

#include <QtMath>

#include <iostream>

QTextStream cin(stdin);

double f(double x)
{
    return qLn(x) - 4 * x * x;
}

double f_der1(double x)
{
    return 1 / x - 8 * x;
}

double f_der2(double x)
{
    return -1 / (x * x) - 8;
}

double newtonMethod(double u0, double eps)
{
    double d1 =f_der1(u0);
    while (qAbs(d1) > eps) {
        u0 = u0 - d1 / f_der2(u0);
        d1 = f_der1(u0);
    }
    return u0;
}

double parabola(double a, double b, double eps)
{
    double u = (a + b) / 2;
    double w, f_a, f_b, f_u, f_w, delta_a, delta_b;
    do {
        f_a = f(a);
        f_b = f(b);
        f_u = f(u);
        delta_a = f_a - f_u;
        delta_b = f_b - f_u;
        w = u + ((b - u) * (b - u) * delta_a - (a - u) * (a - u) * delta_b) /
            (2 * ((b - u) * delta_a + (u - a) * delta_b));
        f_w = f(w);
        if (w < u) {
            if (f_w < f_u) {
                b = u;
                u = w;
            } else if (f_w > f_u) {
                a = w;
            } else if (f_a > f_u) {
                b = u;
                u = w;
            } else if (f_u > f_b) {
                a = w;
            }
        } else if (w > u) {
            if (f_w < f_u) {
                a = u;
                u = w;
            } else if (f_w > f_u) {
                b = w;
            } else if (f_b > f_u) {
                a = u;
                u = w;
            } else if (f_a > f_b) {
                b = w;
            }
        } else {
            u -= eps;
        }
    } while (b - a >= eps);
    return (a + b) / 2;
}

double goldenSection(double a, double b, double eps)
{
    const double goldenConst = 1.618;
    double temp_a, temp_b;
    do {
        temp_a = b - (b - a) / goldenConst;
        temp_b = a + (b - a) / goldenConst;
        if (f(temp_a) < f(temp_b)) {
            b = temp_b;
        } else if (f(temp_a) > f(temp_b)) {
            a = temp_a;
        } else {
            a = temp_a;
            b = temp_b;
        }
    } while (b - a >= eps);
    return (a + b) / 2;
}

double binSearch(double a, double b, double eps)
{
    double u, v;
    double temp_a = a;
    double temp_b = b;
    do {
        const double sigma = (temp_b - temp_a) / 4;
        double mid = (temp_a + temp_b) / 2;
        u = mid - sigma;
        v = mid + sigma;
        if (f(u) < f(v)) {
            temp_b = v;
        }
        else if (f(u) > f(v)) {
            temp_a = u;
        } else {
            temp_a = u;
            temp_b = v;
        }
    } while (temp_b - temp_a >= eps);

    return (temp_a + temp_b) / 2;
}

enum SearchParameters {
    IntervalStart,
    IntervalEnd,
    Epsilon
};

int main()
{
    QString input;
    do {
        std::cout << "Введите границы отрезка и погрешность: ";
        input = cin.readLine();
        if (input.isEmpty()) {
            break;
        }
        QStringList args = input.split(' ');
        if (args.size() > 3) {
            std::cout << "Слишком большое количество параметров" << std::endl;
            continue;
        }
        QVector<double> params(args.size());
        int8_t i = 0;
        bool cast = true;
        for (auto s = args.begin(); s != args.end() && cast; ++s) {
            s->replace(",", ".");
            params[i++] = s->toDouble(&cast);
        }
        if (!cast) {
            std::cout << "Некорректный параметр" << std::endl;
            continue;
        }
        if (params[IntervalStart] >= params[IntervalEnd]) {
            std::cout << "Начало отрезка должно быть меньше конца" << std::endl;
            continue;
        }
        if (params[Epsilon] <= 0) {
            std::cout << "Погрешность должна быть больше нуля" << std::endl;
            continue;
        }
        std::cout << "Точка минимума: "
                  << newtonMethod(binSearch(params[IntervalStart], params[IntervalEnd], params[Epsilon]), params[Epsilon] / 100000)
                  << std::endl;
    } while (!input.isEmpty());

    return 0;
}

