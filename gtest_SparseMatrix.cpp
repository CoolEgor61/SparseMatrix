//
//  gtest_SparseMatrix.cpp
//  SparseMatrix
//
//  Created by Егор Петров on 10.10.2024.
//

#include "SparseMatrix.h"

#include "gtest.h"

//----------------------CMatrix---------------------------------------

// Пример: матрица 6 порядка
//
//          1 . . . 2 .             8
//          . . 3 4 . .             5
//          . . . . . .     *       0
//          . . . 8 . 5             4
//          . . 7 1 . 6             3
//          . . . . . .             1
//
//  std::size_t n = 6 порядок матрицы
//
//  std::size_t nz = 9 - количество ненулевых элементов
//
//  value - массив значений
//  int value[nz] = { 1, 2, 3, 4, 8, 5, 7, 1, 6 }
//
//  row - массив индексов строк, в которых находятся элементы из value
//  int row[nz] = { 0, 0, 1, 1, 3, 3, 4, 4, 4 }
//
//  col - массив индексов столбцов, в которых находятся элементы из value
//  int col[nz] = { 0, 4, 2, 3, 3, 5, 2, 3, 5 }
//

TEST(CMatrix, can_create_cmatrix)
{
    std::size_t n = 6;
    std::size_t nz = 9;
    int *value = new int[nz]{1, 2, 3, 4, 8, 5, 7, 1, 6};
    int *row = new int[nz]{0, 0, 1, 1, 3, 3, 4, 4, 4};
    int *col = new int[nz]{0, 4, 2, 3, 3, 5, 2, 3, 5};
    ASSERT_NO_THROW(CMatrix<int>A(n,nz,value,row,col));
}

TEST(CMatrix, cant_create_cmatrix_with_wrong_size)
{
    std::size_t n = 0;
    std::size_t nz = 9;
    int *value = new int[nz]{1, 2, 3, 4, 8, 5, 7, 1, 6};
    int *row = new int[nz]{0, 0, 1, 1, 3, 3, 4, 4, 4};
    int *col = new int[nz]{0, 4, 2, 3, 3, 5, 2, 3, 5};
    ASSERT_ANY_THROW(CMatrix<int>A(n,nz,value,row,col));
}

TEST(CMatrix, can_do_Ax_is_equal_to_b)
{
    std::size_t n = 6;
    std::size_t nz = 9;
    int *value = new int[nz]{1, 2, 3, 4, 8, 5, 7, 1, 6};
    int *row = new int[nz]{0, 0, 1, 1, 3, 3, 4, 4, 4};
    int *col = new int[nz]{0, 4, 2, 3, 3, 5, 2, 3, 5};
    CMatrix<int>A(n,nz,value,row,col);
    int *x = new int[n]{8,5,0,4,3,1};
    int *xres = new int[n]{14,16,0,37,10,0};
    int *_x = new int[n];
    _x=A*x;
    int flag=1;
    for(std::size_t i=0;i<n;i++) if (_x[i]!=xres[i]) flag=0;
    EXPECT_EQ(1, flag);
}

TEST(CMatrix, can_do_fast_Ax_is_equal_to_b)
{
    std::size_t n = 6;
    std::size_t nz = 9;
    int *value = new int[nz]{1, 2, 3, 4, 8, 5, 7, 1, 6};
    int *row = new int[nz]{0, 0, 1, 1, 3, 3, 4, 4, 4};
    int *col = new int[nz]{0, 4, 2, 3, 3, 5, 2, 3, 5};
    CMatrix<int>A(n,nz,value,row,col);
    int *x = new int[n]{8,5,0,4,3,1};
    int *xres = new int[n]{14,16,0,37,10,0};
    int *_x = new int[n];
    _x=A.fstMul(x);
    int flag=1;
    for(std::size_t i=0;i<n;i++) if (_x[i]!=xres[i]) flag=0;
    EXPECT_EQ(1, flag);
}

TEST (CMatrix, can_a_is_equal_b)
{
    std::size_t n = 6;
    std::size_t nz = 9;
    int *value = new int[nz]{1, 2, 3, 4, 8, 5, 7, 1, 6};
    int *value1 = new int[nz]{1, 56, 3, 4, 8, 5, 7, 1, 6};
    int *row = new int[nz]{0, 0, 1, 1, 3, 3, 4, 4, 4};
    int *col = new int[nz]{0, 4, 2, 3, 3, 5, 2, 3, 5};
    CMatrix<int>A(n,nz,value,row,col);
    CMatrix<int>B(n,nz,value1,row,col);
    A=B;
    EXPECT_EQ(B, A);
}

TEST (CMatrix, can_a_is_not_equal_b)
{
    std::size_t n = 6;
    std::size_t nz = 9;
    int *value = new int[nz]{1, 2, 3, 4, 8, 5, 7, 1, 6};
    int *value1 = new int[nz]{1, 56, 3, 4, 8, 5, 7, 1, 6};
    int *row = new int[nz]{0, 0, 1, 1, 3, 3, 4, 4, 4};
    int *col = new int[nz]{0, 4, 2, 3, 3, 5, 2, 3, 5};
    CMatrix<int>A(n,nz,value,row,col);
    CMatrix<int>B(n,nz,value1,row,col);
    EXPECT_NE(B, A);
}

//----------------------------------------------------------------

//------------------------CRSMatrix-------------------------------

// Пример: матрица 6 порядка
//
//          1 . . . 2 .
//          . . 3 4 . .
//          . . . . . .
//          . . . 8 . 5
//          . . 7 1 . 6
//          . . . . . .
//
//  std::size_t n = 6 порядок матрицы
//
//  std::size_t nz = 9 - количество ненулевых элементов
//
//  value - массив значений
//  int value[nz] = { 1, 2, 3, 4, 8, 5, 7, 1, 6 }
//
//  col - массив индексов столбцов, в которых находятся элементы из value
//  int col[nz] = { 0, 4, 2, 3, 3, 5, 2, 3, 5 }
//
//  row_index - массив смещения индексов строк, в которых находятся элементы из value
//  int row_index[n+1] = { 0, 2, 4, 4, 6, 9, 9 }
//

TEST(CRSMatrix, can_create_crsmatrix)
{
    std::size_t n = 6;
    std::size_t nz = 9;
    int *value = new int[nz]{1, 2, 3, 4, 8, 5, 7, 1, 6};
    int *row_index = new int[n+1]{ 0, 2, 4, 4, 6, 9, 9 };
    int *col = new int[nz]{0, 4, 2, 3, 3, 5, 2, 3, 5};
    ASSERT_NO_THROW(CRSMatrix<int>A(n,nz,value,col,row_index));
    delete [] value; delete [] row_index; delete [] col;
}

TEST(CRSMatrix, cant_create_crsmatrix_with_wrong_size)
{
    std::size_t n = 0;
    std::size_t nz = 9;
    int *value = new int[nz]{1, 2, 3, 4, 8, 5, 7, 1, 6};
    int *row_index = new int[n+1]{ 0 };
    int *col = new int[nz]{0, 4, 2, 3, 3, 5, 2, 3, 5};
    ASSERT_ANY_THROW(CRSMatrix<int>A(n,nz,value,col,row_index));
}

TEST(CRSMatrix, can_do_Ax_is_equal_to_b)
{
    std::size_t n = 6;
    std::size_t nz = 9;
    int *value = new int[nz]{1, 2, 3, 4, 8, 5, 7, 1, 6};
    int *row_index = new int[n+1]{ 0, 2, 4, 4, 6, 9, 9 };
    int *col = new int[nz]{0, 4, 2, 3, 3, 5, 2, 3, 5};
    CRSMatrix<int>A(n,nz,value,col,row_index);
    int *x = new int[n]{8,5,0,4,3,1};
    int *xres = new int[n]{14,16,0,37,10,0};
    int *_x = new int[n];
    _x=A*x;
    int flag=1;
    for(std::size_t i=0;i<n;i++) if (_x[i]!=xres[i]) flag=0;
    EXPECT_EQ(1, flag);
}

TEST (CRSMatrix, can_a_is_equal_b)
{
    std::size_t n = 6;
    std::size_t nz = 9;
    int *value = new int[nz]{1, 2, 3, 4, 8, 5, 7, 1, 6};
    int *value1 = new int[nz]{1, 56, 3, 4, 8, 5, 7, 1, 6};
    int *row_index = new int[n+1]{ 0, 2, 4, 4, 6, 9, 9 };
    int *col = new int[nz]{0, 4, 2, 3, 3, 5, 2, 3, 5};
    CRSMatrix<int>A(n,nz,value,col,row_index);
    CRSMatrix<int>B(n,nz,value1,col,row_index);
    A=B;
    EXPECT_EQ(B, A);
}

TEST (CRSMatrix, can_a_is_not_equal_b)
{
    std::size_t n = 6;
    std::size_t nz = 9;
    int *value = new int[nz]{1, 2, 3, 4, 8, 5, 7, 1, 6};
    int *value1 = new int[nz]{1, 56, 3, 4, 8, 5, 7, 1, 6};
    int *row_index = new int[n+1]{ 0, 2, 4, 4, 6, 9, 9 };
    int *col = new int[nz]{0, 4, 2, 3, 3, 5, 2, 3, 5};
    CRSMatrix<int>A(n,nz,value,col,row_index);
    CRSMatrix<int>B(n,nz,value1,col,row_index);
    EXPECT_NE(B, A);
}

//----------------------------------------------------------------
