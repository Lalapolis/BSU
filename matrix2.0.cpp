#include <iostream>
#include <vector>
#include <typeinfo>

using namespace std;

double RowSum(vector<double>& row) {
	double sum = 0;
	for (int i = 0; i < row.size(); i++)
	{
		sum = sum + row[i];
	}
	return sum;
}

double Square(double x) {
	return x * x;
}

double MakeNonnegative(double x) {
	if (x < 0) {
		return 0;
	}
	else {
		return x;
	}
}

template<class T>
class Matrix {

	int rows_;
	int columns_;
	T** matr;

public:
	Matrix() = delete;

	Matrix(int rows, int columns) {
		rows_ = rows;
		columns_ = columns;
		matr = new T* [rows_];
		for (int i = 0; i < rows_; i++) {
			matr[i] = new T[columns_];
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
		matr = new T* [rows_];
		for (int i = 0; i < rows_; i++) {
			matr[i] = new T[columns_];
		}

		for (int i = 0; i < rows_; i++) {
			for (int j = 0; j < columns_; j++) {
				matr[i][j] = other.matr[i][j];
			}
		}
	};

	~Matrix() {
		for (int i = 0; i < rows_; i++) {
			delete[] matr[i];
		}
		delete[] matr;
	};

	Matrix& operator=(const Matrix& other) {
		if (this != &other) {
			for (int i = 0; i < rows_; i++) {
				delete[]matr[i];
			}
			delete[]matr;

			rows_ = other.rows_;
			columns_ = other.columns_;

			matr = new T* [rows_];
			for (int i = 0; i < rows_; i++)
			{
				matr[i] = new T[columns_];
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

	T& operator()(int index1, int index2) {
		if ((index1 < 0) || (index2 < 0) || (index1 >= rows_) || (index2 >= columns_)) {
			throw "Bad Index";
		}
		return matr[index1][index2];
	};

	const T& operator()(int index1, int index2) const {
		if ((index1 < 0) || (index2 < 0) || (index1 >= rows_) || (index2 >= columns_)) {
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
		if (columns_ != other.rows_) {
			throw "warrning operator*=";
		}
		Matrix temp(rows_, other.columns_);
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

	Matrix& operator*=(const T& number) {
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
		return other * (-1);
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
		else {
			return false;
		}
		return true;
	};
	friend bool operator!=(const Matrix& left, const Matrix& right) {
		return !(left == right);
	};

	friend Matrix operator*(const T& number, const Matrix& matrix) {
		Matrix temp(matrix);
		temp *= number;
		return temp;
	};
	friend Matrix operator*(const Matrix& matrix, const T& number) {
		return number * matrix;
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

	int Columns() const {
		return columns_;
	}

	Matrix Transpose() const {
		Matrix b(columns_, rows_);
		for (int i = 0; i < columns_; i++) {
			for (int j = 0; j < rows_; j++) {
				b.matr[i][j] = matr[j][i];
			}
		}
		return b;
	};

	Matrix Identity(int size) {
		for (int i = 0; i < rows_; i++)
		{
			delete[] matr[i];
		}
		delete[] matr;

		rows_ = size;
		columns_ = size;
		matr = new T* [size];
		for (int i = 0; i < size; i++) {
			matr[i] = new T[size];
		}

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (i == j) {
					matr[i][j] = 1;
				}
				else {
					matr[i][j] = 0;
				}
			}
		}
		return *this;
	}

	void swaplines(int i1, int i2) {
		for (int j = 0; j < columns_; j++)
		{
			swap((*this)(i1,j), (*this)(i2,j));
		}
	}

	T Determinant() {
		if (rows_ != columns_) cerr << "Not qudro";
		double mul = 1;
		Matrix x(*this);
		for (int i = 0; i < x.rows_ - 1; i++)
		{
			if (!x(i,i))
			{
				for (int s = i;s < rows_;s++)
					if (x(s,i))
					{
						x.swaplines(i, s);
						mul *= -1;
						break;
					}
			}
			mul *= x(i,i);
			if (mul == 0) return 0;
			double xii = x(i,i);
			for (int j = i; j < columns_; j++)
			{
				if (xii)
					x(i,j) /= xii;
			}
			for (int s = i + 1; s < rows_; s++)
			{
				double xsi = x(s,i);
				for (int j = i; j < columns_; j++)
				{
					x(s,j) -= x(i,j) * xsi;
				}
			}
		}
		mul *= x(rows_ - 1,columns_ - 1);
		return mul;

	}

	friend T FindMin(const Matrix matrix) {
		T min = matrix(0,0);
		for (int i = 0; i < matrix.rows_; i++)
		{
			for (int j = 0; j < matrix.columns_; j++)
			{
				if (matrix(i,j) < min)
				{
					min = matrix(i,j);
				}
			}
		}
		return min;
	}
	
	template<class Pred>
	friend Matrix Apply(Matrix& matrix,Pred func) {
		Matrix temp(matrix);
		for (int i = 0; i < temp.rows_; i++)
		{
			for (int j = 0; j < temp.columns_; j++)
			{
				temp(i,j) = func(matrix(i,j));
			}
		}
		return temp;
	}

	template<class Pred>
	friend vector<T> RowwiseApply(Matrix& matrix, Pred func) {
		vector<T> result;
		vector<vector<T>> temp(matrix.rows_, vector<T>(matrix.columns_));
		for (int i = 0; i < matrix.rows_; i++)
		{
			for (int j = 0; j < matrix.columns_; j++)
			{
				temp[i][j] = matrix(i, j);
			}
		}
		for (int i = 0; i < matrix.rows_; i++)
		{
			result.push_back(func(temp[i]));
		}
		return result;
	}

};




int main() {
	Matrix<double> test(1, 2);
	Matrix<double> test2(2, 2);
	cout << test;	
	test(0, 0) = 4;
	test(0, 1) = 5;
	cout << test << endl;
	cout << Apply(test, Square) << endl;
	cout << FindMin(test) << endl;
	for (auto it: RowwiseApply(test,RowSum))
	{
		cout << it << endl;
	}

	test2(0, 0) = 2;
	test2(0, 1) = 1;
	test2(1, 0) = 1;
	test2(1, 1) = 1;
	cout << test << endl;
	cout << test2.Determinant();
}