
#include <iostream>
#include <string>
#include <Windows.h>
#include <cmath>
using namespace std;
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    float x;
    float y;
    
    cin >> x;
    cin >> y;

    float R = x * log10(y);
    float sec = 1 / cos(x);
    float S = sec * pow(y, 1.0 / 3.0);

    cout <<"R = "<< R << "\n";
    cout <<"S = "<< S << "\n";

    float C = max(R, S);

    cout <<"C = "<< C << endl;

    system("pause");
    return 0;
}