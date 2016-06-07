#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <fstream>
#include <stdexcept>

#define TYPE int

template <typename T>
class Matrix
{
private:
	int size;
	T** data;

public:
	static const int RANDOM_MAX = 23170;

	Matrix(int size)
		: size(size)
	{
		data = new T*[size];
		for (int i = 0; i < size; i++)
			data[i] = new T[size];
	}

	/// Copy constructor (must have because of ApplicationData setMatrix)
	Matrix(const Matrix& m)
		: size(m.size)
	{
		data = new T*[size];
		for (int i = 0; i < size; i++)
			data[i] = new T[size];

		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				data[i][j] = T(m.getData()[i][j]);
	}

	~Matrix()
	{
		for (int i = 0; i < size; i++)
			delete[] data[i];
		delete[] data;
	}

	int getSize() const { return size; }
	T** getData() const { return data; }

	friend std::istream& operator>>(std::istream& _in, Matrix<TYPE>& _m)
	{
		for (int i = 0; i < _m.size; i++)
			for (int j = 0; j < _m.size; j++)
			{
				_in >> _m.data[i][j];
				if (_in.fail() || _in.bad())
					throw std::runtime_error("Invalid file format.");
			}

		return _in;
	}

	friend std::ostream& operator<<(std::ostream& _out, Matrix<TYPE>& _m)
	{
		for (int i = 0; i < _m.size; i++)
		{
			for (int j = 0; j < _m.size; j++)
			{
				_out << _m.data[i][j] << " ";
				if (_out.fail() || _out.bad())
					throw std::runtime_error("Could not write to file.");
			}
			_out << "\n";
		}

		return _out;
	}
};

#endif