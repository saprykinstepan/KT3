#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <math.h>
#include <locale.h>

#define PI 3.1415926535897932384

double f(double x); // Вычисление значения кусочной функции f(x)
double build_table(double start, double step, int count); // Построить таблицу значений функции
int check_monotonic(double start, double end, double step); // Проверить, возрастает/убывает ли функция на интервале
double find_x(double Y, double tolerance); // Найти x, при котором f(x) = Y (метод половинного деления)
double derivative_at_point(double x); // Вычислить производную в точке x 

int main() {
    setlocale(LC_ALL, "");
    int choice;
    double x, start, end, step, Y, tolerance;
    int count;

    printf("Конструирование программы анализа функции\n");
    printf("Выполнил: Сапрыкин Степан бИЦ-251\n\n\n");
    printf("Программа для работы с функцией\n");

    do {
        printf("\n1. Значение функции в точке\n");
        printf("2. Таблица значений\n");
        printf("3. Проверка на монотонность\n");
        printf("4. Поиск X по Y\n");
        printf("5. Производная в точке\n");
        printf("0. Выход\n");
        printf("Выберите пункт: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Введите x: ");
            scanf("%lf", &x);
            printf("f(%g) = %g\n", x, f(x));
            break;
        case 2:
            printf("Введите начало интервала: ");
            scanf("%lf", &start);
            printf("Введите шаг: ");
            scanf("%lf", &step);
            printf("Введите количество отсчетов: ");
            scanf("%d", &count);
            build_table(start, step, count);
            break;
        case 3:
            printf("Начало интервала: ");
            scanf("%lf", &start);
            printf("Конец интервала: ");
            scanf("%lf", &end);
            printf("Шаг: ");
            scanf("%lf", &step);
            int monotonic_type = check_monotonic(start, end, step);
            if (monotonic_type == 1) printf("Функция возрастает\n");
            else if (monotonic_type == -1) printf("Функция убывает\n");
            else printf("Функция не монотонна\n");
            break;
        case 4:
            printf("Введите Y: ");
            scanf("%lf", &Y);
            printf("Введите точность: ");
            scanf("%lf", &tolerance);
            find_x(Y, tolerance);
            break;
        case 5:
            printf("Введите x: ");
            scanf("%lf", &x);
            break;
        case 0: printf("Выход\n");
            break;
        default: printf("Неверный выбор\n");
            break;
        }
    } while (choice != 0);

    return 0;
}

double f(double x) {
    if (x < -PI / 4.0) {
        if (fabs(cos(x)) < 1e-10) return 0; //если cos равен 0
        return 1.0 / cos(x);
    }
    else if (x < 1.0) {
        return log(1.0 + exp(x));
    }
    else {
        double sum = 0.0;
        for (int n = 0; n <= 16; n++) {
            sum += pow(x, 2 * n + 1) / ((2 * n + 1) * sqrt(n + 1));
        }
        return sum;
    }
}

double build_table(double start, double step, int count) {
    if (step <= 0 || count <= 0) {
        printf("Ошибка! Шаг и количество отсчетов должны быть положительными.\n");
        return 0;
    }

    printf("\nТаблица значений:\n");
    printf("_______________________\n");
    printf("|   x    |    f(x)     |\n");
    printf("-----------------------\n");

    double x = start;
    for (int i = 0; i < count; i++) {
        printf("| %5.2lf  | %10.6g |\n", x, f(x));
        x += step; // Переход к следующей точке
    }
    printf("------------------------\n");
    return x;
}

int check_monotonic(double start, double end, double step) {
    double prev_value = f(start); // Сохраняем значение функции в начальной точке
    int increasing = 1; // Флаг возрастания (1 = ДА, 0 = НЕТ)
    int decreasing = 1; // Флаг убывания (1 = ДА, 0 = НЕТ)

    if (step <= 0 || end <= start) {
        printf("Ошибка: Шаг должнен быть положительным и начало интервала должно быть меньше конца интервала.\n");
        return 0;
    }

    for (double x = start + step; x <= end; x += step) {
        double curr_value = f(x); // Текущее значение функции

        if (curr_value > prev_value) {
            decreasing = 0; // Сбрасываем флаг убывания
        }
        if (curr_value < prev_value) {
            increasing = 0; // Сбрасываем флаг возрастания
        }
        prev_value = curr_value; // Обновляем предыдущее значение для следующей итерации
    }

    if (increasing == 1) {
        return 1;
    }
    if (decreasing == 1) {
        return -1;
    }
    return 0;
}

double find_x(double Y, double tolerance) {
    if (tolerance <= 0) {
        printf("Ошибка! Точность должна быть положительной.\n");
        return 0;
    }

    double a = -3.0, b = 3.0; // Область, где будет искаться решение уравнения f(x) = Y

    for (int i = 0; i < 100; i++) { // Метод деления пополам - 100 итераций максимум
        double mid = (a + b) / 2; // Находим середину интервала
        double f_mid = f(mid);

        if (fabs(f_mid - Y) < tolerance) { // Проверка достижения требуемой точности
            printf("Найдено: x = %g\n", mid);
            return mid; // Нашли решение, возвращаем x
        }
        if ((f(a) - Y) * (f_mid - Y) < 0) { // Выбор новой границы интервала
            b = mid; // Корень между a и mid
        }
        else {
            a = mid; // Корень между mid и b
        }
    }
}

double derivative_at_point(double x) {
    double h = 0.0001; // очень маленькое число
    double deriv = (f(x + h) - f(x)) / h;

    printf("f'(%g) = %g\n", x, deriv);

    return deriv; // Возвращаем значение производной
}
