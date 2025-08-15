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
#include <iomanip>
#include <new>
#include <string>
#include <vector>

#include "levels.hpp"
#include "memory.hpp"

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

struct Matrix
{
    // Logging capabilities
    Log LogMatrix;
    std::string name = "A";

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

    // log(string newName) calls log()
    std::string log(const std::string &newName);

    // log() calls repeately log(vector<float>::c_itr row)
    // to format the whole of the matrix with N+1 Cols
    // A = [col0 | col1 | col2 | ... | colN]
    Log* log() const;

    Log* log(const std::vector<float>::const_iterator row) const;
};

/******************************************************************************/
/*    IMPLEMENTATION                                                          */
/******************************************************************************/

// Matrix allocation from a list
Matrix::Matrix(std::initializer_list<float> val) :  val(val), rows(1), cols(val.size())
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

std::string Matrix::log(const std::string &newName)
{
    Log logMatrix;

    this->name.clear();
    this->name = newName;

    if (this->rows * this->cols == 0)
    {
        logMatrix << Log::MSG::GRAY << "[] (empty matrix)" << Log::MSG::ENDC << Log::MSG::ENDL;
    }
    else
    {
        Log *tmp = this->log();
        logMatrix << tmp->str();
        delete tmp;
    }

    // here it goes the stream selection?
    //std::cout << logMatrix.str() << std::endl;

    return logMatrix.str();
}

Log* Matrix::log() const
{
    Log *pRow = nullptr;
    // the first/edge case could be handle at initialization(new) level?
    Log *pMatrix = new Log;
    uint32_t margin = 3U;

    // edge case, first iteration
    //          |123 1234|
    // building "   A = ["
    *pMatrix << Log::MSG::GRAY << std::string(margin, ' ') << this->name << " = [" << Log::MSG::ENDC;
    // 3U = size(" = ")
    margin += this->name.size() + margin;

    pRow = this->log(this->val.cbegin());
    *pMatrix << pRow->str();
    delete pRow;

    *pMatrix << Log::MSG::GRAY << "]" << Log::MSG::ENDC << Log::MSG::ENDL;

    // to loop over only when rows > 0
    for (uint32_t i = 1U; i < this->rows; i++)
    {
        uint32_t pos = this->cols * i;
        *pMatrix << Log::MSG::GRAY << std::string(margin, ' ') << "[" << Log::MSG::ENDC;

        Log *pRow = this->log(this->val.cbegin() + pos);
        *pMatrix << pRow->str();
        delete pRow;

        *pMatrix << Log::MSG::GRAY << "]" << Log::MSG::ENDC << Log::MSG::ENDL;
    }

    return pMatrix;
}

// It returns only the content of [ a, b, ..., i, ..., n], without the "[]"
Log* Matrix::log(const std::vector<float>::const_iterator row) const
{
    const uint32_t width = 10U;
    Log *pRow = new Log;

    // building initial element
    *pRow << Log::MSG::WHITE;

    *pRow << std::right << std::setw(width) << std::fixed << row[0U];
    for (uint32_t j = 1U; j < this->cols; j++)
    {
        *pRow << "," << std::right << std::setw(width) << std::fixed << row[j];
    }

    *pRow << Log::MSG::ENDC;

    // Delete after returning
    return pRow;
}
#endif /* MATRIX_H_ */
