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

    // Memory allocation for a matrix
    Matrix(uint32_t rows, uint32_t cols);

    // Empty matrix
    Matrix();

    // Matrix allocation from a list
    Matrix(std::initializer_list<float> val);

    /*Matrix& transpose()
    {
        Matrix *AT = nullptr;
        if (this->val == nullptr)
        {
            //LOG_WARNING("Nothing to transpose: Empty matrix!");
            return  *this;
        }
    }*/


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
                    out << " " << std::to_string(this->val[col]) << ",";
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
