  

# cre.MAT

欢迎来到cre.MAT。本项目是兴趣使然的cpp项目，目标是通过模板实现一个线性代数库。

# 为什么使用cre.MAT

## 1.灵活性

cre.MAT允许你以符合数学习惯的方式创建、操作矩阵。

创建$3\times3$单位矩阵$\left[ \begin{matrix} 1 & 0 &0\\0&1&0\\0&0&1\\ \end{matrix} \right]$可以使用以下代码,示例：

```c++
int mat3x3[9]={1,0,0,0,1,0,0,0,1};
int *pmat3x3=mat3x3;

Matrix<int,3,3> identity1({1,0,0,0,1,0,0,0,1});//使用初始化列表创建矩阵
Matrix<int,3,3> identity2(mat3x3);//从数组创建矩阵
Matrix<int,3,3> identity3(pmat3x3);//从指针创建矩阵，有越界访问风险

```

矩阵的加减法与乘法示例：

```c++
Matrix<int,3,3> a({1,1,1,1,1,1,1,1,1});
Matrix<float,3,3> b({0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f,0.5f});

cout<<a+b<<endl;//整形与浮点型矩阵相加
cout<<a*b<<endl;//整形与浮点型矩阵相乘
```

## 2.安全性

cre.MAT默认使用栈分配，避免错综复杂的内存管理与aliasing的问题。同时，利用cpp的编译期检查，我们可以提前将错误排查出来。

示例：

以下代码通不过编译

```c++
Matrix<float,2,2> a({0.5f,0.5f,0.5f,0.5f});
Matrix<float,2,3> b({0.5f,0.5f,0.5f,0.5f,0.5f,0.5f});

cout<<a+b<<endl;//ERROR! 2x2的矩阵无法和2x3的矩阵相乘
cout<<a*b.transposed()<<endl;//ERROR! 2x2的矩阵无法和3x2的矩阵相乘
```

