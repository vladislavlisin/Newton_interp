#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

// Реализация интерполиционного полинома Ньютона, 3 варианта
// нужно не забывать исправить в нескольких местах на нужную вам функцию
// весь справочный материал взят из учебника Сорокина по ВМАиЛА (не для равноудаленных узлов)

using namespace std;

// cos(x) and sin(x)
double func_7(double value)
{
//    return abs(value);
return value;
}

// Функция целого числа: y = [x], берём целую часть, остальное отбрасываем
int func_6(double value)
{
    return (int)value;
}

// y = x^3
double func_5(double value)
{
    return value*value*value;
}

// y = 1/(1+25x^2) - функция Рунге
double func_4(double value)
{
    return 1/(1+25*value*value);
}

// y = e^x
double func_3(double value)
{
    return exp(value);
}

// y = x
double func_2(double value)
{
    return value;
}

// y = x^2
double func_1(double value)
{
    return value * value;
}

// узлы, первый узел, последний узел
// рекурсивно считаем разделенные разности, хз зачем, просто так решил забабахать =D
double differences(vector<double>& points, int start, int end)
{
    if((end - start + 1) == 2)
    {
        return (func_3(points[end]) - func_3(points[start]))/(points[end] - points[start]);
    }
    else
    {
        return (differences(points, start + 1, end) - differences(points, start, end - 1))/(points[end] - points[start]);
    }
}

// для не равноотстающих узлов
void Newton_interpolation(vector<double>& nodes, int nodes_number)
{
    double Newton = func_3(nodes[0]); // первый член это просто значение функции
    double point; // точка в которой будем считать значение интерполяционного полинома Ньютона
    cin >> point;

    for(int i = 1; i < nodes_number; i++)
    {
        // temp = (x-x_1)(x-x_2)...(x-x_k-1)
        double temp = 1;
        for(int j = 0; j < i; j++)
        {
            temp *= (point - nodes[j]);
        }
        // diff = (x-x_1)(x-x_2)...(x-x_k-1)*f(x_0,x_1, ... , x_k)
        temp *= differences(nodes, 0, i);
        cout << differences(nodes, 0, i) << endl;

        // складываем все члены
        Newton += temp;
    }

//    cout << "Function value: " << func_3(point) << endl;
//    cout << "Interpolation value: " << Newton << endl;
//    cout << "Error: " << abs(func_3(point) - Newton) << endl;
}

// https://studme.org/199286/informatika/interpolyatsionnye_mnogochleny_nyutona
// случай когда узлы находятся на одинаковом расстоянии друг от друга
// узлы, число узлов,
double Newton(vector<double>& nodes, int n)
{
    double step = abs(nodes[0] - nodes[1]); // расстояние между узлами фиксированное
    double x; // точка в которой будет вычисляться значение полинома
    cin >> x;
    vector<vector<double>> dy(n); // таблица конечных разностей
    for(auto & i : dy) i.resize(n);

    // подсчитываем dy
    for ( int i = 0; i < n; i++ )
    {
        dy[0][i] = func_3(nodes[i]);
    }

    for ( int i = 1; i < n; i++ )
    {
        for ( int j = 0; j < n-i; j++ )
        {
            dy[i][j] = dy[i-1][j+1] - dy[i-1][j];
        }
    }

    // вычисляем результирующий y
    double q = (x - nodes[0]) / step; // см. формулу
    double result = func_3(nodes[0]); // результат (y)

    double mult_q = 1; // произведение из q*(q-1)*(q-2)*(q-n)
    double fact = 1;  // факториал

    for ( int i = 1; i < n; i++ )
    {
        fact *= i;
        mult_q *= (q-i+1);

        result += mult_q/fact * dy[i][0];
    }

//    cout << "Function value: " << func_3(x) << endl;
//    cout << "Interpolation value: " << result << endl;
//    cout << "Error: " << abs(func_3(x) - result) << endl;
    return result;
}

// тоже для любых значений узлов, на любом расстоянии друг от друга, только через другую формулу для разделенной разности
double Newton_turbo_interp(vector<double>& nodes, int nodes_number, double x)
{
    double res = func_7(nodes[0]), F, den;
    for(int i = 1; i < nodes_number; i++)
    {
        F = 0;
        for(int j = 0; j <= i; j++)
        {//следующее слагаемое полинома
            den = 1;
//считаем знаменатель разделенной разности
            for(int k = 0; k <= i; k++)
            {
                if (k != j) den *= (nodes[j] - nodes[k]);
            }
//считаем разделенную разность
            F += (func_7(nodes[j]) / den);
        }
//домножаем разделенную разность на скобки (x-x[0])...(x-x[i-1])
        for(int k = 0; k < i; k++) F *= (x - nodes[k]);
        res +=F ;//полином
    }
//    cout << "Function value: " << func_5(x) << endl;
//    cout << "Interpolation value: " << res << endl;
//    cout << "Error: " << abs(func_5(x) - res) << endl;
    return res;
}

int main()
{
    // вводим узлы интерполяции, например отрезок [-1,1], 4 интервала, значит 5 узлов: -1 -0.5 0 0.5 1
    // [0,1]: 0 0.5 1
    // или [-2,2], 8 интервалов, значит 9 узлов: -2 -1.5 -1 -0.5 0 0.5 1 1.5 2
    // 4 nodes: 0 1 2 3
    int nodes_number;
    cin >> nodes_number;
    // если узлов меньше одного, то ошибочка
    if(nodes_number < 1) exit(1);
    vector<double> nodes(nodes_number);
    for(int i = 0; i < nodes_number; i++)
    {
        cin >> nodes[i];
    }

    double step = abs(nodes[0] - nodes[1])/2;

    vector<double> values(nodes.size()*2-1);
    values[0] = nodes[0];
    for(int i = 1; i < values.size(); i++) {
        values[i] = values[i-1] + step;
    }

    for(double value : values) {
        cout << value << " ";
    }
    cout << endl;

    cout << setprecision(8) << fixed;

    double error = 0;
    double good = 0;
    double max = 0;

    for(int i = 0; i < values.size()+1; i++) {
        error += Newton_turbo_interp(nodes, nodes_number, values[i]);
        good += func_7(values[i]);
        if(Newton_turbo_interp(nodes, nodes_number, values[i]) > max)
            max = Newton_turbo_interp(nodes, nodes_number, values[i]);
        cout << abs((Newton_turbo_interp(nodes, nodes_number, values[i]) - func_7(values[i]))/func_7(values[i])) * 100 << endl;
    }

    double rez = abs(error - good);
    cout << "Inter: " << error << endl;
    cout << "Func: " << good << endl;
    cout << "Error: " << rez << endl;
    cout << "Max: " << 0.32 << endl;
    return 0;
}