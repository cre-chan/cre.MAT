//
// Created by cre-chan on 19-8-28.
//

#ifndef MATRIX_STATIC_MATRIX_HPP
#define MATRIX_STATIC_MATRIX_HPP

#include <iostream>
#include <type_traits>
#include <stdexcept>

namespace matrix {
    using namespace std;

    template<typename T, uint x_dim = 0, uint y_dim = 0>
    class Matrix {
        static_assert(x_dim > 0 && y_dim > 0, "A matrix with dimension less than 1 is not allowed");


        using T_arr=const T (&)[x_dim*y_dim];
        using row= T (&)[y_dim];
        using Self=Matrix<T,x_dim,y_dim>;
    public:
        T content[x_dim][y_dim];

        Matrix() = default;

        //拷贝构造器
        Matrix(const Self &other) {
            for (uint i = 0; i < x_dim; i++)
                for (uint j = 0; j < y_dim; j++)
                    (*this)[i][j] = other.content[i][j];
        }

        //从一个一维数组创建矩阵，可以使用{}初始化列表
        Matrix(T_arr array) {
            for (uint i = 0; i < x_dim; i++)
                for (uint j = 0; j < y_dim; j++)
                    content[i][j] = array[i * y_dim + j];
        }

        //从一个指针创建矩阵
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

        //加法，仅适用于x_dim与y_dim均对应情况
        template<typename E, typename R=decltype(declval<T>() + declval<E>())>
        constexpr Matrix<R, x_dim, y_dim> operator-(const Matrix<E, x_dim, y_dim> &right) const {
            Matrix<R, x_dim, y_dim> ret;


            for (uint i = 0; i < x_dim; i++)
                for (uint j = 0; j < y_dim; j++)
                    ret[i][j] = content[i][j] - right.content[i][j];


            return ret;
        }

        //乘法，仅针对
        template<typename E, typename R=decltype(declval<T>() * declval<E>()), uint other_y_dim>
        constexpr Matrix<R, x_dim, other_y_dim> operator*(const Matrix<E, y_dim, other_y_dim> &other) const {
            //判断定义在T与E的乘积类型R上的加法是否对于R封闭
            static_assert(is_same<decltype(declval<R>() + declval<R>()), R>::value, "R+R is not closed");

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
        Matrix<T, x, y> &reshape() {
            static_assert(x * y == x_dim * y_dim, "INVALID_PARAMETERS:x*y does not equal to the original size");

            auto &self = *this;
            return (Matrix<T, y, x> &) self;
        }


    };

    //todo
    template<typename T>
    class Matrix<T, 0, 0> {

        using Self=Matrix<T>;

        //拷贝构造函数
        Matrix(const Self& other):x(other.x),y(other.y),content(new T[other.x * other.y]){
            for (uint i = 0; i < x; i++)
                for (uint j = 0; j < y; j++)
                    content[i * y + j] = other.content[i * y + j];
        };


    public:
        T *content;
        uint x;
        uint y;


        template<uint x_dim, uint y_dim>
        static Self dyn(const T (&arr)[x_dim * y_dim]) {
            return Matrix<T>(arr, x_dim, y_dim);
        }

        //todo 解决bad_alloc问题
        Self clone(){
            //拷贝构造函数
            return Matrix(*this);
        }

        //移动构造函数
        Matrix(Self&& other) noexcept:content(other.content),x(other.x),y(other.y) {
            other.content= nullptr;
            other.x=0;
            other.y=0;
        }

        //用于赋值
        Self& operator=(Self other){
            swap(content,other.content);
            x=other.x;
            y=other.y;

            return (*this);
        }

        //todo:检查以保证x_dim与y_dim大于0
        Matrix(const T arr[], uint x_dim, uint y_dim) : content(new T[x_dim * y_dim]), x(x_dim), y(y_dim) {
            if(x_dim==0||y_dim==0){
                clog<<"x_dim or y_dim is equal to 0"<<endl;
                abort();
            }

            for (uint i = 0; i < x_dim; i++)
                for (uint j = 0; j < y_dim; j++)
                    content[i * y_dim + j] = arr[i * y_dim + j];
        }

        ~Matrix(){
            if(content)
                delete[] content;
        }
    };

    template<typename T, uint x, uint y>
    ostream &operator<<(ostream &out, const Matrix<T, x, y> &mat) {
        for (uint i = 0; i < x; i++) {
            for (uint j = 0; j < y; j++)
                out << mat.content[i][j] << '\t';

            out << endl;
        }

        return out;
    }

    template<class T>
    ostream &operator<<(ostream &out, const Matrix<T> &mat) {
        for (uint i = 0; i < mat.x; i++) {
            for (uint j = 0; j < mat.y; j++)
                out << mat.content[i * mat.y + j] << '\t';

            out << endl;
        }

        return out;
    }
}


#endif //MATRIX_STATIC_MATRIX_HPP
