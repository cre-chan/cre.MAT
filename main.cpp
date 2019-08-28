#include <iostream>
#include <type_traits>
#include <stdexcept>

using namespace std;


template<typename T, uint x_dim, uint y_dim>
class Matrix {
    static_assert(x_dim > 0 && y_dim > 0, "A matrix with dimension less than 1 is not allowed");


    typedef const T (&T_arr)[x_dim * y_dim];
    typedef T (&row)[y_dim];
public:
    T content[x_dim][y_dim];

    Matrix() = default;

    //拷贝构造器
    Matrix(const Matrix<T, x_dim, y_dim> &other) {
        for (uint i = 0; i < x_dim; i++)
            for (uint j = 0; j < y_dim; j++)
                content[i][j] = other.content[i][j];
    }

    //从一个一维数组创建矩阵，可以使用{}初始化列表
    Matrix(T_arr array) {
        for (uint i = 0; i < x_dim; i++)
            for (uint j = 0; j < y_dim; j++)
                content[i][j] = array[i * y_dim + j];
    }

    //从一个一维数组创建矩阵，可以使用{}初始化列表
    Matrix(T *array) {
        for (uint i = 0; i < x_dim; i++)
            for (uint j = 0; j < y_dim; j++)
                content[i][j] = array[i * y_dim + j];
    }

    //加法，仅适用于x_dim与y_dim均对应情况
    template<typename E, typename R=decltype(declval<T>() + declval<E>())>
    constexpr Matrix<R, x_dim, y_dim> operator+(const Matrix<E, x_dim, y_dim> &right) const {
        Matrix<R, x_dim, y_dim> ret;


        for (uint i = 0; i < x_dim; i++)
            for (uint j = 0; j < y_dim; j++)
                ret[i][j] = content[i][j] + right.content[i][j];


        return ret;
    }

    //乘法，仅针对
    template<typename E,typename R=decltype(declval<T>()*declval<E>()),uint other_y_dim>
    constexpr Matrix<R, x_dim, other_y_dim> operator*(const Matrix<E, y_dim, other_y_dim> &other) const {
        //判断定义在T与E的乘积类型R上的加法是否对于R封闭
        static_assert(is_same<decltype(declval<R>()+declval<R>()),R>::value,"R+R is not closed");

        Matrix<R, x_dim, other_y_dim> ret;

        for (uint i = 0; i < x_dim; i++) {
            R sum[other_y_dim];

            for (uint k = 0; k < other_y_dim; k++) sum[k] = content[i][0] * other.content[0][k];

            for (uint j = 1; j < y_dim; j++)
                for (uint k = 0; k < other_y_dim; k++) sum[k] = sum[k] + content[i][j] * other.content[j][k];

            for (uint k = 0; k < other_y_dim; k++) ret[i][k] = sum[k];
        }

        return ret;
    }

    row operator[](uint i) {
        if (i >= x_dim) throw out_of_range("i in Matrix[i][j] out of range");
        return content[i];
    }

    Matrix<T, y_dim, x_dim> transposed() {
        Matrix<T, y_dim, x_dim> ret;

        for (uint i = 0; i < x_dim; i++)
            for (uint j = 0; j < y_dim; j++)
                ret[j][i] = (*this)[i][j];

        return ret;
    }

    template<uint x, uint y>
    Matrix<T, y, x> &reshape() {
        auto &self = *this;
        return (Matrix<T, y, x> &) self;
    }

};


template<typename T, uint x, uint y>
ostream &operator<<(ostream &out, Matrix<T, x, y> mat) {
    for (uint i = 0; i < x; i++) {
        for (uint j = 0; j < y; j++)
            out << mat.content[i][j] << '\t';

        out << endl;
    }

    return out;
}


int main() {
    Matrix<int, 2, 2> a({1, 1, 1, 1});
    Matrix<float, 2, 2> b({1.0f, 2.0f, 3.5f, 4.0f});

    cout << a + b<<endl;
    cout<<a*b;
    return 0;
}