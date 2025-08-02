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

#include <cstdint>
#include <initializer_list>
#include <new>
#include <string>
#include <vector>

#include "memory.hpp"

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

struct Matrix
{
    Log LogMatrix;

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

    void* operator new(std::size_t count);

    operator std::string() const;
};

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

// Matrix allocation from a list
Matrix::Matrix(std::initializer_list<float> val) : val(val), rows(1), cols(val.size())
{
    LOG_INFO(LogMatrix, "Constructing row vector [", this->rows, "x", this->cols, "].");
}

Matrix::Matrix(uint32_t rows, uint32_t cols) : rows(rows), cols(cols)
{
    if (this->rows * this->cols != 0)
    {
        LOG_INFO(LogMatrix, "Reserving memory with ", this->rows, "x", this->cols, " elements.");
        this->val.reserve(this->rows * this->cols);
    }
    else
    {
        LOG_WARNING(LogMatrix, "Unable to reserve memory.");
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
        LOG_ERROR(LogMatrix, "It is not possible to reshape from [",
                this->rows, "x", this->cols, "] to [", newRows, "x", newCols, "].");
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
        LOG_WARNING(LogMatrix, "Nothing to transpose: Empty matrix!");
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
        LOG_WARNING(LogMatrix, "Overwriting matrix [", this->rows, "x", this->cols, "] with Id(", size, ").");
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

void* Matrix::operator new(std::size_t count)
{
    Log LogOperators;
    LOG_INFO(LogOperators, "new(", count, ") Matrix.");
    Matrix *ptr = nullptr;

    if (count == 0U)
    {
        // how to trigger this?
        LOG_ERROR(LogOperators, "Wrong malloc(", count, ").");
        throw std::bad_alloc{};
    }
    else
    {
        ptr = (Matrix*)std::malloc(count);
        if (ptr != nullptr)
        {
            LOG_DEBUG(LogOperators, "stack.push(", ptr, ").");
            std::stack<void*> *pStack = Static::getStack();
            pStack->push(ptr);
        }
        else
        {
            // how to trigger this?
            LOG_ERROR(LogOperators, "Wrong ptr(", ptr, ").");
            throw std::bad_alloc{};
        }
    }

    return ptr;
}

Matrix::operator std::string() const
{
    Log LogMatrix;

    LOG_DEBUG(LogMatrix, "A in [", this->rows, "x", this->cols, "].");
    if (this->rows * this->cols == 0)
    {
        LogMatrix << Log::Level::WARNING << __FILE__ << ":" << __LINE__ \
                  << Log::MSG::ENDC << Log::MSG::GRAY << "\t(empty matrix)" \
                  << Log::MSG::ENDC << Log::MSG::ENDL;
    }
    else
    {
        for (uint32_t row = 0; row < this->rows; row++)
        {
            uint32_t pos = this->cols * row;
            LogMatrix << "\t[" << Log::MSG::WHITE;
            for (uint32_t col = 0; col < this->cols - 1U; col++)
            {
                pos += 1U;
                LogMatrix << " " << std::to_string(this->val[pos]) << ",";
            }
            pos += 1U;
            LogMatrix << " " << std::to_string(this->val[pos]) << "]" << Log::MSG::ENDL;
        }
    }

    return LogMatrix.str();
}
#endif /* MATRIX_H_ */
