#include <QString>
#include <QTextStream>
#include <QVector>

#include <iostream>

QTextStream cin(stdin);

double f(const double x)
{
    return x * x;
}

double binSearch(const double a, const double b, const double eps)
{
    const double sigma = (b - a) / 4;
    double u, v;
    double temp_a = a;
    double temp_b = b;
    do {
        u = (temp_a + temp_b - sigma) / 2;
        v = (temp_a + temp_b + sigma) / 2;
        if (f(u) < f(v)) {
            temp_b = v;
        }
        else {
            temp_a = u;
        }
    } while (f(temp_b) - f(temp_a) >= eps);

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
        QStringList args = input.split(' ');
        if (args.size() > 3) {
            std::cout << "Слишком большое количество параметров" << std::endl;
            continue;
        }
        QVector<double> params(args.size());
        int8_t i = 0;
        bool cast = true;
        for (auto s = args.begin(); s != args.end() && cast; ++s, ++i) {
            s->replace(",", ".");
            params[i] = s->toDouble(&cast);
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
                  << binSearch(params[IntervalStart],
                               params[IntervalEnd],
                               params[Epsilon])
                  << std::endl;
    } while (!input.isEmpty());

    return 0;
}

