#include <iostream>
#include <static_matrix.hpp>

using namespace matrix;

int main() {
    int c_arr[] = {1, 2, 2, 1};

    Matrix<int, 2, 2> a({1, 1, 1, 1});
    Matrix<float, 2, 2> b({1.0f, 2.0f, 3.5f, 4.0f});

    Matrix<int> c = Matrix<int>::dyn<2, 2>({1, 2, 2, 1});


    cout << a + b << endl;
    cout << a * b << endl;

    cout << c << endl;
    cout<<c.clone()<<endl;

    return 0;
}