#include <iostream>
#include <fstream>
#include <typeinfo>
#include <cstring>
#include <type_traits>

using namespace std;

class Exception : public exception
{
protected:
	string str;
public:
	Exception(const string s)
	{
		str = s;
	}
	~Exception() {}

	virtual void what()
	{
		cout << "\n" << "exception: " << str << "\n"; 
	}
};

class NotaCountException : public Exception
{
public:
	NotaCountException() : Exception("") {}
	~NotaCountException() {}

	virtual void what()
	{
		cout << "\n" << "operation unavailable: type of matrix is not a count" << "\n"; 
	}
};

class IndexOutOfBoundsException : public Exception
{
protected:
	int height, width, height_ind, width_ind;
public:
	IndexOutOfBoundsException(int h, int w, int h_i, int w_i) : Exception("Index out of bounds")
	{
		height = h; width = w; height_ind = h_i; width_ind = w_i;
	}
	~IndexOutOfBoundsException() {}

	virtual void what()
	{
		cout << "Index out of bounds, height = " << height << ", width = " << width << "\n";
		cout << "height index = " << height_ind << ", width index = " << width_ind << "\n";
	}
};

class WrongDimensionsException : public Exception
{
protected:
	int height, width;
public:
	WrongDimensionsException(int h, int w) : Exception("Wrong dimensions")
	{
		height = h; width = w;
	}
	~WrongDimensionsException() {}

	virtual void what()
	{
		cout << "Wrong dimensions, height = " << height << ", width = " << width << "\n";
	}
};

class WrongSizeException : public WrongDimensionsException
{
public:
	WrongSizeException(int h, int w) : WrongDimensionsException(h, w) {}

	virtual void what()
	{
		cout << "Incorrect parameters: height = " << height << ", width = " << width << "\n";
	}
};

template <typename T>
class base_matrix;

template <typename T>
ostream& operator << (ostream &s, base_matrix<T> &m)
{
	if (typeid(s) == typeid(cout))
	{
		m.print();
	}
	else
	{
		int height = m.height, width = m.width;
		s << "\n" << height << " " << width << "\n";

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				s << m.ptr[i][j] << " ";
		}
	}
	return s;
}

template <typename T>
istream& operator >> (istream &s, base_matrix<T> &m)
{
	if (typeid(s) == typeid(cin))
	{
		for(int i = 0; i < m.height; i++)
			for(int j = 0; j < m.width; j++)
				s >> m.ptr[i][j];
	}
	else
	{
		int height, width;
		s >> height >> width;

		base_matrix<T> *tmp = &m;
		delete tmp;
		m = base_matrix<T>(height, width);

		for(int i = 0; i < height; i++)
			for(int j = 0; j < width; j++)
			{
				s >> m.ptr[i][j];
			}
	}
	return s;
}

template <typename T>
class base_matrix
{
protected:
	int height, width;
	T **ptr;
	bool created = false;

	void clear_memory()
	{
		for (int i = 0; i < height; i++)
			delete[] ptr[i];
		delete[] ptr;
	}
public:
	base_matrix(int Height = 2, int Width = 2)
	{
		if (created) clear_memory();

		height = Height;
		width = Width;

		if (height < 1 || width < 1) throw WrongSizeException(height, width);

		ptr = new T *[height];
		for (int i = 0; i < height; i++)
			ptr[i] = new T[width];

		created = true;
	}

	base_matrix(const base_matrix &m)
	{
		if (created) clear_memory();

		height = m.height;
		width = m.width;
		ptr = new T *[height];
		for (int i = 0; i < height; i++)
			ptr[i] = new T [width];

		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				ptr[i][j] = m.ptr[i][j];

		created = true;
	}

	~base_matrix()
	{
		cout << "\ndestructor is working\n";
		created = false;
		clear_memory();
	}

	base_matrix &operator = (const base_matrix &m)
	{
		if (created) clear_memory();

		height = m.height;
		width = m.width;

		ptr = new T *[height];
		for (int i = 0; i < height; i++)
			ptr[i] = new T [width];

		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				ptr[i][j] = m.ptr[i][j];

		created = true;
		return *this;
	}

	T& operator() (int row, int column)
	{
		if (row < 0 || column < 0 || row >= height || column >= width)
			throw IndexOutOfBoundsException(height, width, row, column);
		return ptr[row][column];
	}

	void print()
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				cout << ptr[i][j] << " ";
			cout << "\n";
		}
	}

	friend ostream& operator << <T>(ostream& s, base_matrix &m);
	friend istream& operator >> <T>(istream& s, base_matrix &m);
};

template <typename T>
class matrix : public base_matrix<T>
{
protected:
	bool correct_type()
	{
		return (typeid(T) == typeid(short)
			|| typeid(T) == typeid(unsigned short)
			|| typeid(T) == typeid(int)
			|| typeid(T) == typeid(unsigned int)
			|| typeid(T) == typeid(long)
			|| typeid(T) == typeid(unsigned long)
			|| typeid(T) == typeid(long long)
			|| typeid(T) == typeid(unsigned long long)
			|| typeid(T) == typeid(float)
			|| typeid(T) == typeid(double)
			|| typeid(T) == typeid(long double)
			|| typeid(T) == typeid(char));
	}
public:
	matrix(int Height, int Width) : base_matrix<T>(Height, Width) {}

	void fill()
	{
		if (!correct_type()) throw Exception("operation 'fill' impossible for this type of matrix\n");

		for (int i = 0; i < this -> height; i++)
			for (int j = 0; j < this -> width; j++)
			{
				int tmp = rand() % 100 + 54 ;
				this -> ptr[i][j] = static_cast<T>(tmp);
			}
	}

	double *vector() 
	{	
		if (!correct_type() || typeid(T) == typeid(char)) throw NotaCountException();

		double *res = new double [this -> height];
		for (int i = 0; i < this -> height; i++)
		{
			for (int j = 0; j < this -> width; j++)
				res[i] += static_cast<double> (this -> ptr[i][j]);
			res[i] /= this -> width;
		}
		return res;
	}
	int vector_len() { return this -> height; }
};

int main()
{

	cout << "\n\ncreating and writing matrices\n\n";

	base_matrix<int> *arr = new base_matrix<int>[5];
	int i = 0;
	for (; i < 5; i++)
	{
		arr[i] = base_matrix<int>(i + 1, i + 1);
		for (int j = 0; j < (i + 1) * (i + 1); j++)
			arr[i](j / (i + 1), j % (i + 1)) = 4 * i + j + 1;
	}

	try
	{
		ofstream fout("text.txt");
		if (fout)
		{
			cout << "these matrices are:\n";
			fout << i << "\n";
			for (i = 0; i < 5; i++)
			{
				fout << arr[i];
				cout << arr[i] << "\n";
			}
		}
		fout.close();
	}
	catch(...)
	{
		cout << "\nwriting unsuccessful";
	}

	base_matrix<int>* arr2 = new base_matrix<int>[5];
	ifstream fin("text.txt");
	i = 0;
	try
	{	
		if (fin.is_open())
		{
			int count; fin >> count;
			cout << "\nreading matrices, they are:\n";
			for (; i < count; i++)
			{
				fin >> arr2[i];
				cout << arr2[i] << "\n";
			}
		}
		fin.close();
	}
	catch(...)
	{
		cout << "\nreading unsuccessful, i = " << i;
	}

	cout << "\n\ncreating, filling and showing vectors of matrices\n\n";

	double *vec;
	matrix<int> arr4 = matrix<int>(4,4);
	arr4.fill();
	arr4.print();
	vec = arr4.vector();
	cout <<"\n";
	for (int i = 0; i < arr4.vector_len(); i++)
		cout << vec[i] << " ";

	cout << "\n\n";

	try
	{
		matrix<char> arr5 = matrix<char>(4, 4);
		arr5.fill();
		arr5.print();
		vec = arr5.vector();
		cout <<"\n";
		for (int i = 0; i < arr5.vector_len(); i++)
			cout << vec[i] << " ";
	}
	catch(NotaCountException e)
	{
		e.what();
	}
	
	cout << "\n\nchecking last exeptions\n\n";

	cout << "wrong size exception\n";
	try
	{
		matrix<int> *arr6 = new matrix<int>(-1, -1);
	}
	catch(WrongSizeException e)
	{
		e.what();
		cout << "\n";
	}

	cout << "index out of bounds exception\n";
	try
	{
		int tmp = arr4(-1, -1);		
	}
	catch(IndexOutOfBoundsException e)
	{
		e.what();
		cout << "\n";
	}

	char c; cin >> c;
	return 0;
}