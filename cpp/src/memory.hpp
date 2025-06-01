#include <iostream>
#include <cstdint>
#include <string>
#include <sstream>

struct Matrix
{
    Matrix(uint32_t rows, uint32_t cols) : rows(rows), cols(cols)
    {
        if (this->rows * this->cols != 0)
        {
            this->val = new float[this->rows * this->cols];
        }
        else
        {
            this->val = NULL;
        }
    }

    Matrix() : Matrix(0, 0) {};


    ~Matrix()
    {
        if (this->val != NULL)
        {
            delete this->val;
        }
    }


    operator std::string() const
    {
        std::ostringstream out;

        out << "(MATRIX) in [" << this->rows <<"x" << this->cols << "]" << std::endl;

        return out.str();
    }

    private:
        uint32_t rows;
        uint32_t cols;
        float   *val;
};
