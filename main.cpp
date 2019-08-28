#include <iostream>
#include <static_matrix.hpp>

using namespace matrix;

int main() {
    Matrix<int, 2, 2> a({1, 1, 1, 1});
    Matrix<float, 2, 2> b({1.0f, 2.0f, 3.5f, 4.0f});

    cout << a + b<<endl;
    cout<<a*b;
    return 0;
}