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
/*    PUBLIC MACROS                                                           */
/******************************************************************************/

/**
 * @brief   Macro to get a row-vector from a matrix.
 */
#define GET_ROW_VECTOR(A, row) \
    (A).getBlock(row, row + 1U, 0U, (A).cols)

/**
 * @brief   Macro to get a column-vector from a matrix.
 */
#define GET_COLUMN_VECTOR(A, col) \
    (A).getBlock(0U, (A).rows, col, col + 1U)

/**
 * @example    LOG_MATRIX(A);
 */
#if LOG_LEVEL_DEBUG <= LOG_CONFIG
    #define LOG_MATRIX(A) \
        LOG_DEBUG((A).logMatrix, #A, " in [", (A).rows, "x", (A).cols, "]."); \
        (A).log(#A);
#else
    #define LOG_DEBUG(A.logMatrix, ...)
#endif

/******************************************************************************/
/*    API                                                                     */
/******************************************************************************/

struct Matrix
{
    // Logging capabilities
    Log logMatrix;
    std::string name = "A";

    uint32_t    rows = 0;
    uint32_t    cols = 0;
    std::vector<float>  val;

    // Constructors & Destructors
    // Memory allocation for a matrix
    Matrix(uint32_t rows, uint32_t cols);

    // would this be private?
    Matrix(Matrix& A): Matrix(A.rows, A.cols)
    {
        // is there a cleaner way to get the logging from the other ctor?
        A.logMatrix << this->logMatrix.str();
        this->logMatrix.str("");

        LOG_INFO(A.logMatrix, "Copying matrix.");
        for (auto src = A.val.cbegin(); src != A.val.cend(); src++)
        {
            this->val.push_back(*src);
        }
    };

    // Empty matrix
    Matrix();

    // to move to implementation section
    ~Matrix()
    {
        std::cout << this->logMatrix.str();
    }

    // Matrix allocation from a list
    Matrix(std::initializer_list<float> val);

    // Math operators
    Matrix& reshape(const uint32_t newRows, const uint32_t newCols);

    Matrix& transpose();

    Matrix& id(const size_t size);

    Matrix* getBlock(const uint32_t row, const uint32_t rowEnd,
                     const uint32_t col, const uint32_t colEnd)
    {
        Matrix *block = nullptr;

        // row < rowEnd <= S->rows, the same holds for cols, to be a valid call
        if ((rowEnd <= row) || (colEnd <= col) ||
            (this->rows < rowEnd) || (this->cols < colEnd))
        {
            return block;
        }

        block = new Matrix(rowEnd - row, colEnd - col);
        for (uint32_t i = row; i < rowEnd; i++)
        {
            auto pRowSrc = this->val.cbegin() + this->cols * i + col;
            auto pRowDst = block->val.cbegin() + block->cols * (i - row);
            block->val.insert(pRowDst, pRowSrc, pRowSrc + block->cols);
        }

        LOG_MATRIX(*block);
        return block;
    }

    void* operator new(std::size_t count);

    // log is the public API
    // log(string newName) calls log()
    void log(const std::string &newName);

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
    LOG_INFO(this->logMatrix, "Constructing row vector [", this->rows, "x", this->cols, "].");
}

Matrix::Matrix(uint32_t rows, uint32_t cols) : rows(rows), cols(cols)
{
    if (this->rows * this->cols != 0)
    {
        LOG_INFO(this->logMatrix, "Reserving memory with ", this->rows, "x", this->cols, " elements.");
        this->val.reserve(this->rows * this->cols);
    }
    else
    {
        LOG_WARNING(this->logMatrix, "Unable to reserve memory.");
    }
}

// Empty matrix
Matrix::Matrix() : Matrix(0, 0)
{
    LOG_INFO(this->logMatrix, "Creating an empty matrix");
}

Matrix& Matrix::reshape(const uint32_t newRows, const uint32_t newCols)
{
    const uint32_t total = this->rows * this->cols;
    const uint32_t newTotal = newRows * newCols;

    // Should I clip the matrix to force it into a smaller space?
    if (total > newTotal)
    {
        LOG_ERROR(this->logMatrix, "It is not possible to reshape from [",
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
        LOG_WARNING(this->logMatrix, "Nothing to transpose: Empty matrix!");
        return  *this;
    }
    else
    {
        // There is no need to transpose a row or a column vector
        if ((this->rows > 1) && (this->cols > 1))
        {
            Matrix A(*this);
            for (uint32_t row = 0; row < A.rows; row++)
            {
                for (uint32_t col = 0; col < A.cols; col++)
                {
                    uint32_t pos = A.cols * row + col;
                    uint32_t newPos = this->cols * col + row;

                    this->val[newPos] = A.val[pos];
                }
            }
        }

        this->reshape(this->cols, this->rows);
    }

    return *this;
}

Matrix& Matrix::id(const size_t size)
{
    if (this->val.empty() != true)
    {
        LOG_WARNING(this->logMatrix, "Overwriting matrix [", this->rows, "x", this->cols, "] with Id(", size, ").");
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
    Log tmp;
    LOG_INFO(tmp, "new(", count, ") Matrix.");
    Matrix *pMatrix = nullptr;

    if (count == 0U)
    {
        // how to trigger this?
        LOG_ERROR(tmp, "Wrong malloc(", count, ").");
        throw std::bad_alloc{};
    }
    else
    {
        pMatrix = (Matrix*)std::malloc(count);
        if (pMatrix != nullptr)
        {
            LOG_DEBUG(tmp, "stack.push(", pMatrix, ").");
            std::stack<void*> *pStack = Static::getStack();
            pStack->push(pMatrix);
        }
        else
        {
            // how to trigger this?
            LOG_ERROR(tmp, "Wrong pMatrix(", pMatrix, ").");
            throw std::bad_alloc{};
        }
    }

    std::cout << tmp.str();
    return pMatrix;
}

void Matrix::log(const std::string &newName)
{
    if (this->name != newName)
    {
        this->name.clear();
        this->name = newName;
    }

    if (this->rows * this->cols == 0)
    {
        this->logMatrix << Log::MSG::GRAY << std::string(3U, ' ') << "[] (empty matrix)" << Log::MSG::ENDC << Log::MSG::ENDL;
    }
    else
    {
        Log *tmp = this->log();
        this->logMatrix << tmp->str();
        delete tmp;
    }
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
