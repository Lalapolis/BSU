#include <iostream>
using namespace std;

class Matrix {

    int rows_;
    int columns_;
    double** matr;

public:
    Matrix() = delete; 

    Matrix(int rows, int columns) {
        rows_ = rows;
        columns_ = columns;
        matr = new double* [rows_];
        for (int i = 0; i < rows_; i++) {
            matr[i] = new double[columns_];
        }

        for (int i = 0; i < rows_; i++) {
            for (int j = 0; j < columns_; j++) {
                matr[i][j] = 0;
            }
        }
    };  

    Matrix(const Matrix& other) {
        rows_ = other.rows_;
        columns_ = other.columns_;
        matr = new double* [rows_];
        for (int i = 0; i < rows_; i++) {
            matr[i] = new double[columns_];
        }

        for (int i = 0; i < rows_; i++)
        {
            for (int j = 0; j < columns_; j++)
            {
                matr[i][j] = other.matr[i][j];
            }
        }
    };

    ~Matrix() {
        for (int i = 0; i < rows_; i++)
        {
            delete[] matr[i];
        }
        delete[] matr;
    };

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            for (int i = 0; i < rows_; i++)
            {
                delete[]matr[i];
            }
            delete[]matr;

            rows_ = other.rows_;
            columns_ = other.columns_;

            matr = new double* [rows_];
            for (int i = 0; i < rows_; i++)
            {
                matr[i] = new double[columns_];
            }

            for (int i = 0; i < rows_; i++)
            {
                for (int j = 0; j < columns_; j++)
                {
                    matr[i][j] = other.matr[i][j];
                }
            }
        }
            return *this;
       
    }

    double& operator()(int index1, int index2) {
        if ((index1 < 0 )|| (index2 < 0 ) || (index1 >= rows_ )|| (index2 >= columns_))
        {
            throw "Bad Index";
        }
        return matr[index1][index2];
    };

   const double& operator()(int index1, int index2) const {
        if ((index1 < 0) || (index2 < 0) || (index1 >= rows_) || (index2 >= columns_))
        {
            throw "Bad Index";
        }
        return matr[index1][index2];
    };

    Matrix& operator+=(const Matrix& other) {
            if (other.rows_ != rows_ || other.columns_ != columns_)
            {
                throw "warrning operator+=";
            }

            for (int i = 0; i < rows_; i++)
            {
                for (int j = 0; j < columns_; j++)
                {
                    matr[i][j] += other.matr[i][j];
                }
            }      
      return *this;
    }; 
   
    Matrix& operator-=(const Matrix& other) {
        if (other.rows_ != rows_ || other.columns_ != columns_)
        {
            throw "warrning operator-=";
        }

        for (int i = 0; i < rows_; i++)
        {
            for (int j = 0; j < columns_; j++)
            {
                matr[i][j] -= other.matr[i][j];
            }
        }
        return *this;
       
    };  

    Matrix& operator*=(const Matrix& other) {
        if (rows_ != other.columns_ ) {
            throw "warrning operator*=";;
        }
        Matrix temp(rows_,other.columns_);
        for (int i = 0; i < rows_; i++)
        {
            for (int j = 0; j < other.columns_; j++)
            {
                for (int k = 0; k < columns_; k++)
                    temp.matr[i][j] += matr[i][k] * other.matr[k][j];
            }
        }
        *this = temp;
        return *this;
    };  
    
    Matrix& operator*=(const double& number) {
        for (int i = 0; i < rows_; i++)
        {
            for (int j = 0; j < columns_; j++)
            {
                matr[i][j] *= number;
            }
        }
        return *this;
    };


    friend Matrix operator-(const Matrix& other) {
        return other *(-1);
    };  

    friend bool operator==(const Matrix& left, const Matrix& right) {
        if (left.rows_ == right.rows_ && left.columns_ == right.columns_) {
            for (int i = 0; i < left.rows_; i++)
            {
                for (int j = 0; j < left.columns_; j++)
                {
                    if (left.matr[i][j] != right.matr[i][j]) {
                        return false;
                    };
                }
            }
        }
        else return false;
        return true;
    };
    friend bool operator!=(const Matrix& left, const Matrix& right) {
        return !(left == right);
    };

    friend Matrix operator*(const double& number, const Matrix& matrix) {
        Matrix temp(matrix);
        temp*=number;
        return temp;
    };
    friend Matrix operator*(const Matrix& matrix, const double& number) {
        return number*matrix;
    };

    friend Matrix operator+(const Matrix& left, const Matrix& right) {
        if (left.rows_ != right.rows_ && left.columns_ != right.columns_)
        {
            throw "warrning operator+";
        }
        Matrix tmp(left);
        tmp += right;
        return tmp;
    }; 

    friend Matrix operator-(const Matrix& left, const Matrix& right) {
        if (left.rows_ != right.rows_ && left.columns_ != right.columns_)
        {
            throw "warrning operator-";
        }
        Matrix tmp(left);
        tmp -= right;
        return tmp;
    };  
    friend Matrix operator*(const Matrix& left, const Matrix& right) {
        if (left.rows_ != right.columns_ && left.columns_ != right.rows_)
        {
            throw "warrning operator*";
        }
        Matrix templ(left);
        templ *= right;
        return templ;
    } 

    friend ostream& operator<<(ostream& out, const Matrix& matrix) {
        for (int i = 0; i < matrix.rows_; i++)
        {
            for (int j = 0; j < matrix.columns_; j++)
            {
                out << matrix.matr[i][j] << ' ';
            }
            out << std::endl;
        }
        return out;
    }

    int Rows() const {
        return rows_;
    }
    int Columns() const{
        return columns_;
    }
    Matrix Transpose() {
        Matrix b(columns_, rows_);
        for (int i = 0; i < columns_; i++) {
            for (int j = 0; j < rows_; j++) {
                b.matr[i][j] = matr[j][i];
            }
        }
        *this = b;
        return *this;
    }; 

    Matrix Identity(int size) {
        for (int i = 0; i < rows_; i++)
        {
            delete[] matr[i];
        }
        delete[] matr;

        rows_ = size;
        columns_ = size;
        matr = new double* [size];
        for (int i = 0; i < size; i++) {
            matr[i] = new double[size];
        }

        for (int i = 0; i < size; i++) {                            
            for (int j = 0; j < size; j++) {
                if (i == j) matr[i][j] = 1;
                else matr[i][j] = 0;
            }
        }
        return *this;
    };
};

int main() {
    Matrix test(3,4);
    test.Identity(3);
    Matrix test2(3, 3);
    cout << test<< endl;
    
    

}