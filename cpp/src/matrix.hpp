/*******************************************************************************
*
* Matrix class
*
*   SUMMARY
*       Implementation of a matrix in 2D.
*
*       The API's functionality is simple:
*       a) matrix creation
*       b) matrix destruction
*       c) reshape, transpose and identity matrix
*
*******************************************************************************/

#ifndef MATRIX_H_
#define MATRIX_H_

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <cstdio>
#include <initializer_list>
#include <sstream>
#include <vector>

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

struct Matrix
{
    uint32_t    rows = 0;
    uint32_t    cols = 0;
    std::vector<float>  val;

    // Constructors & Destructors
    // Memory allocation for a matrix
    Matrix(uint32_t rows, uint32_t cols);

    // Empty matrix
    Matrix();

    // Matrix allocation from a list
    Matrix(std::initializer_list<float> val);

    // Math operators
    Matrix& reshape(const uint32_t newRows, const uint32_t newCols);

    Matrix& transpose();

    Matrix& id(const size_t size);

    operator std::string() const
    {
        std::ostringstream out;

        out << "(MATRIX) in [" << this->rows <<"x" << this->cols << "]" << std::endl;
        if (this->rows * this->cols == 0)
        {
            out << "\t[] empty matrix" << std::endl;
        }
        else
        {
            for (uint32_t row = 0; row < this->rows; row++)
            {
                out << "\t[ ";
                for (uint32_t col = 0; col < this->cols; col++)
                {
                    uint32_t pos = this->cols * row + col;
                    out << " " << std::to_string(this->val[pos]) << ",";
                }
                out << "]" << std::endl;
            }
        }

        return out.str();
    }
};


/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

// Matrix allocation from a list
Matrix::Matrix(std::initializer_list<float> val) : val(val), rows(1), cols(val.size()) {}

Matrix::Matrix(uint32_t rows, uint32_t cols) : rows(rows), cols(cols)
{
    if (this->rows * this->cols != 0)
    {
        this->val.reserve(this->rows * this->cols);
    }
    else
    {
        std::fprintf(stderr, "Unable to reserve(%u) memory\n", 0U);
    }
}

// Empty matrix
Matrix::Matrix() : Matrix(0, 0) {}

Matrix& Matrix::reshape(const uint32_t newRows, const uint32_t newCols)
{
    const uint32_t total = this->rows * this->cols;
    const uint32_t newTotal = newRows * newCols;

    // Should I clip the matrix to force it into a smaller space?
    if (total > newTotal)
    {
        std::printf("It is not possible to reshape from [%ux%u] to [%ux%u]\n",
                this->rows, this->cols, newRows, newCols);
    }
    else
    {
        this->rows = newRows;
        this->cols = newCols;

        this->val.resize(newTotal);
    }

    return *this;
}

Matrix& Matrix::transpose()
{
    if (this->val.empty())
    {
        //LOG_WARNING("Nothing to transpose: Empty matrix!");
        return  *this;
    }
    else
    {
        // There is no need to transpose a row or a column vector. Their
        // memory layout is the same.
        if ((this->rows > 1) && (this->cols > 1))
        {
            for (uint32_t row = 0; row < this->rows; row++)
            {
                for (uint32_t col = 0; col < this->cols; col++)
                {
                    uint32_t pos = this->cols * row + col;
                    float val = this->val[pos];

                    uint32_t newPos = this->rows * col + row;
                    float newVal = this->val[newPos];

                    this->val[newPos] = val;
                    this->val[pos] = newVal;
                }
            }
        }

        // Swapping dimensions
        this->reshape(this->cols, this->rows);

        return *this;
    }
}

Matrix& Matrix::id(const size_t size)
{
    if (this->val.empty() != true)
    {
        std::printf("Overwriting matrix [%ux%u] with Id(%lu)\n", this->rows, this->cols, size);
        this->val.clear();
    }

    auto start = this->val.cbegin();
    this->val.insert(start, size * size, 0);
    this->rows = size;
    this->cols = size;

    for (uint32_t i = 0; i < size; i++)
    {
        uint32_t pos = this->cols * i + i;
        this->val[pos] = 1.0F;
    }

    return *this;
}
#endif /* MATRIX_H_ */
