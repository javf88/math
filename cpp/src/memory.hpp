/*******************************************************************************
*
* MEMORY MANAGEMENT SYSTEM
*
*   SUMMARY
*       This single-header module implements a simple but effective memory
*       management system, the M2S. It extends/generalizes the vector container
*       to a matrix container.
*
*       The API's functionality is simple:
*       a) matrix creation
*       b) matrix destruction
*
*******************************************************************************/

#ifndef MEMORY_H_
#define MEMORY_H_

/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <cstdint>
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

    // Operators
    Matrix& reshape(uint32_t newRows, uint32_t newCols)
    {
        uint32_t total = this->rows * this->cols;
        uint32_t newTotal = newRows * newCols;

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

    Matrix& transpose()
    {
        Matrix *AT = nullptr;
        if (this->val.empty())
        {
            //LOG_WARNING("Nothing to transpose: Empty matrix!");
            return  *this;
        }
        else
        {
            // Swapping dimensions
            //uint32_t tmp = this->rows;
            //this->rows = this->cols;
            //this->cols = tmp;

            // There is no need to transpose a row or a column vector. Their
            // memory layout is the same.
            if ((this->rows == 0) || (this->cols == 0))
            {
                return *this;
            }
            else
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
                uint32_t tmp = this->cols;
                this->cols = this->rows;
                this->rows = tmp;

                return *this;
            }
        }
    }


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
        // Empty else
    }
}

// Empty matrix
Matrix::Matrix() : Matrix(0, 0) {}

#endif /* MEMORY_H_ */
