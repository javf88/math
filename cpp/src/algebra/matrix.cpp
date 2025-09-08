/******************************************************************************/
/*    INCLUDED FILES                                                          */
/******************************************************************************/

#include <cfloat>
#include <cstdint>
#include <iomanip>

#include "matrix.hpp"
#include "memory.hpp"

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

Matrix::Matrix(Matrix& A): Matrix(A.rows, A.cols)
{
    // is there a cleaner way to get the logging from the other ctor?
    A.logMatrix << this->logMatrix.str();
    this->logMatrix.str("");

    LOG_INFO(A.logMatrix, "Copying matrix.");
    for (auto src = A.val.cbegin(); src != A.val.cend(); src++)
    {
        this->val.push_back(*src);
    }
}

// Empty matrix
Matrix::Matrix() : Matrix(0, 0)
{
    LOG_INFO(this->logMatrix, "Creating an empty matrix");
}

Matrix::~Matrix()
{
    std::cout << this->logMatrix.str();
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

Matrix* Matrix::getBlock()
{
    Matrix *block = nullptr;

    if ((this->rows < 2U) || (this->cols < 2U))
    {
        LOG_WARNING(this->logMatrix, "Unable to extract block Matrix from dimensions [", this->rows, "x", this->cols, "]");
        block = nullptr;
    }
    else
    {
        uint32_t rows = this->rows - 1U;
        uint32_t cols = this->cols - 1U;
        block = new Matrix(rows, cols);
        for (uint32_t i = 0U; i < block->rows; i++)
        {
            auto pRowSrc = this->val.cbegin() + this->cols * (i + 1U) + 1U;
            auto pRowDst = block->val.begin() + block->cols * i;
            block->val.insert(pRowDst, pRowSrc, pRowSrc + block->cols);
        }

        LOG_MATRIX(*block);
    }

    return block;
}

Matrix* Matrix::setBlock(Matrix *S)
{
    for (uint32_t i = 1U; i < this->rows; i++)
    {
        auto pRowDst = this->val.begin() + this->cols * i + 1U;
        auto pRowSrc = S->val.cbegin() + S->cols * (i - 1U);
        for (uint32_t j = 0U; j < S->cols; j++)
        {
            pRowDst[j] = pRowSrc[j];
        }
    }

    return this;
}

Matrix* Matrix::rowPermute()
{
    Matrix *PA = nullptr;

    uint32_t row = 0U;
    for (row = 0U; row < this->rows; row++)
    {
        auto entry = this->val.cbegin() + (this->cols * row);
        if (std::abs(*entry) > 2.0F * FLT_EPSILON)
        {
            LOG_DEBUG(this->logMatrix, "Pivot is in row ", row);
            break;
        }
    }

    if (0U < row)
    {
        if (row < this->rows)
        {
            Matrix P;
            P.id(this->rows);

            P.val[0U] = 0.0F;
            P.val[row] = 1.0F;
            P.val[this->cols * row] = 1.0F;
            P.val[this->cols * row + row] = 0.0F;
            LOG_MATRIX(P);

            PA = P * *this;
            // which matrix to return when this is made a function?
            LOG_MATRIX(*PA);
        }
        else
        {
            LOG_DEBUG(this->logMatrix, "The matrix is singular, nothing to permute.");
            PA = nullptr;
        }
    }
    else
    {
        LOG_DEBUG(this->logMatrix, "Nothing to permute (A = PA => P = I).");
        PA = this;
    }

    return PA;
}

Matrix* Matrix::rowReduction()
{
    Matrix L_inv;
    L_inv.id(this->rows);

    auto a = this->val.cbegin(); // A[0,0]
    for (uint32_t row = 1U; row < this->rows; row++)
    {
        uint32_t pos = this->cols * row;
        auto l = L_inv.val.begin() + pos; // L^{-1}[i,0]
        auto b = this->val.cbegin() + pos; // A[i,0]

        *l = *b / *a * -1.0F;
    }
    LOG_MATRIX(L_inv);

    // PA = LU => L^{-1} PA = U
    Matrix *U = L_inv * *this;
    LOG_MATRIX(*U);

    return U;
}

Matrix* Matrix::echelon()
{
    // overdertemined case
    if (this->rows > this->cols)
    {
        LOG_WARNING(this->logMatrix, "The matrix is an overdetermiend system.");
        LOG_WARNING(this->logMatrix, "There are more rows than unknows (rows = ", this->rows, ", cols = ", this->cols, ").");

        // Returning nullptr to signal wrong input
        return nullptr;
    }
    else
    {
        // scalar case
        if (this->rows == 1U)
        {
            float scalar = this->val[0U];
            if (std::abs(scalar) < 2 * FLT_EPSILON)
            {
                LOG_ERROR(this->logMatrix, "Scalar zero-matrix.");
            }
            else
            {
                LOG_WARNING(this->logMatrix, "Scalar matrix");
            }

            // Returning itself to signal end of recursion
            return this;
        }
        else
        {
            // general case
            // 1) get P for P x A product, if a(0,0) is zero
            Matrix *PA = this->rowPermute();
            if (PA == nullptr)
            {
                return nullptr;
            }
            else
            {
                LOG_MATRIX(*PA);
                // 2) get L(0) for L(0) x PA = L(0) x LU, to remove 0-th column
                Matrix *LiPA = PA->rowReduction();
                LOG_MATRIX(*LiPA);
                delete PA;
                // 3) get LPA(2,2) as subA in LPA = [LPA(1,1) | LPA(1,2)]
                //                                  [LPA(2,1) | LPA(2,2)]
                Matrix *subA = LiPA->getBlock();
                LOG_MATRIX(*subA);
                // 4) call recursively
                Matrix *U = subA->echelon();
                LOG_MATRIX(*U);

                // LiPA is U from previous iteration
                LiPA->setBlock(U);
                return LiPA;
            }
        }
    }
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
            LOG_DEBUG(tmp, "manager.push_back(", pMatrix, ").");
            pMatrix->manager.push_back(pMatrix);
        }
        else
        {
            // how to trigger this?
            LOG_ERROR(tmp, "Wrong (pMatrix*)", pMatrix);
            throw std::bad_alloc{};
        }
    }

    // how to log this into the right object
    std::cout << tmp.str();
    return pMatrix;
}

void Matrix::operator delete(void* ptr) noexcept
{
    Log tmp;
    if (ptr != nullptr)
    {
        LOG_INFO(tmp, "Looking for (Matrix*)", ptr);
        // do I need a getter?
        Memory<Matrix*> *pManager = &((Matrix*)ptr)->manager;
        for (auto pMatrix = pManager->cbegin(); pMatrix != pManager->cend(); pMatrix++)
        {
            if (ptr == *pMatrix)
            {
                LOG_INFO(tmp, "Freeing (Matrix*)", ptr);
                pManager->erase(pMatrix);
                std::free(ptr);

                break;
            }
        }
    }
    else
    {
        LOG_INFO(tmp, "Avoiding to delete (Matrix*)", ptr);
    }

    std::cout << tmp.str();
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
