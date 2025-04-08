#include <iostream>
using namespace std;

int main() {
    cout << fixed;
    cout.precision(6);

    int a = 13; float b = 0.165;
    cout << a << " * " << b << " = " << (float)a * b;
    return 0;
}