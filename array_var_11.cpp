#include <iostream>
#include <cmath>

using namespace std;

class MyArrayParent
{
protected:
	//сколько памяти выделено?
	//но используется не как количество памяти а максимальное количество элементов
	int capacity = 100;
	//количество элементов - сколько памяти используем
	int count = 0;
	//массив
	double* ptr = nullptr;
public: 
	MyArrayParent(int Dimension = 100)
	{
		delete[] ptr;

		cout << "MyArrayParent constructor\n";

		if (Dimension < 1)
		{
			cout << "\nyou tried to create an array with invalid capacity\n";
			cout << "array capacity must be at least 1\n";
			cout << "making capacity 1\n";
			Dimension = 1;
		}

		ptr = new double[Dimension];
		capacity = Dimension;
		count = 0;
	}
	MyArrayParent(double* arr, int len)
	{
		delete[] ptr;

		cout << "MyArrayParent constructor\n";

		capacity = len * 1.5 + 1;
		count = len;
		ptr = new double[capacity];

		for (int i = 0; i < len; i++)
			ptr[i] = arr[i];
	}
	~MyArrayParent()
	{
		cout << "MyArrayParent destructor\n";

		if (ptr != nullptr)
		{
			delete[] ptr;
			ptr = nullptr;
		}
	}

	int Capacity() { return capacity; }
	int Size() { return count; }
	double* Arr() { return ptr; }

	double GetComponent(int index)
	{
		if (index >= 0 && index < count)
			return ptr[index];
		return -1;
	}
	void SetComponent(int index, double value)
	{
		if (index >= 0 && index < count)
			ptr[index] = value;
		else 
			cout << "\nfailed to set component: index " << index << " out of range\n";
	}

	//добавление в конец нового значения
	void push(double value)
	{
		if (++count <= capacity)
		{
			ptr[count - 1] = value;
		}
		else
		{
			capacity *= 1.5 + 1;
			double* tmp = new double[capacity];

			for (int i = 0; i < count - 1; i++)
				tmp[i] = ptr[i];
			tmp[count - 1] = value;

			delete[] ptr;
		    ptr = tmp;
		}
	}

	//удаление элемента с конца
	void RemoveLastValue()
	{
		if (count > 0)
			count--;
		else
			cout << "\nerror: attempt to remove element from empty array\n";
	}

	//этот оператор сделан на примере такого же оператора из заготовки с пары
	double& operator[](int index)
	{
		if (count == 0)
			throw("\nattempt to access empty array\n");

		if (index < 0 || index >= count)
			throw("index ", index, " out of range");

		return ptr[index];
	}

	MyArrayParent& operator=(const MyArrayParent& V)
	{
		delete[] ptr;

		capacity = V.capacity;
		count = V.count;

		ptr = new double[V.capacity];
		for (int i = 0; i < V.count; i++)
			ptr[i] = V.ptr[i];

		return *this;
	}

	MyArrayParent(const MyArrayParent& V)
	{
		delete[] ptr;

		capacity = V.capacity;
		count = V.count;

		ptr = new double[V.capacity];
		for (int i = 0; i < V.count; i++)
			ptr[i] = V.ptr[i];	
	}

	void print()
	{
		cout << "\nMyArrParent, size: " << count <<", values: {";
		for (int i = 0; i < count; i++)
		{
			cout << ptr[i];
			if (i != count - 1)
				cout << ", ";
		}
		cout << "}\n";
	}
};

class MyArrayChild : public MyArrayParent
{
public:
	MyArrayChild(int Dimension = 100) : MyArrayParent(Dimension) { cout << "MyArrayChild constructor\n"; }
	MyArrayChild(double* arr, int len) : MyArrayParent(arr, len) { cout << "MyArrayChild constructor\n"; }

	~MyArrayChild()	{ cout << "\nMyArrayChild destructor\n"; }

	//удаление элемента
	void RemoveAt(int index = -1)
	{
		if (index == -1) index = count - 1;//0 at the beginning, -1 or count - 1 at the end

		if (index > -1 && index < count)
		{
			count--;
			for (int i = index; i < count; i++)
				ptr[i] = ptr[i + 1];
		}
		else
			cout << "\nattempt to remove element with invalid index\n";
	}

	//поиск элемента
	int IndexOf(double value, bool bFindFromStart = true)
	{
		if (bFindFromStart)
		{
			for (int i = 0; i < count; i ++ )
				if (ptr[i] == value) { return i; }
			return -1;
		}
		else
		{
			cout << "enter the index of element to search\n";
			int tmp;
			cin >> tmp;

			for (int i = tmp; i < count; i ++)
				if (ptr[i] == value) { return i; }
			return -1;
		}
	}

	//вставка элемента
	void InsertAt(double value, int index = -1)
	{
		if (index == -1) index = count;//0 at the beginning, -1 or count at the end

		if (index > -1 && index <= count)
		{
			if (count == capacity) push(0);//push made count 1 more
			else count++;

			for (int i = count - 1; i > index; i--)
				ptr[i] = ptr[i - 1];
			ptr[index] = value;
		}
		else
			cout << "\nattempt to insert an element with invalid index\n";
	}

	MyArrayChild operator+(MyArrayChild &v)
	{
		cout << "\nmyarraychild operator +\n";

		int new_count = count + v.count;

		MyArrayChild res(new_count);
		res.count = count + v.count;

		for (int i = 0; i < count; i++)
			res.ptr[i] = ptr[i];
		for (int i = 0; i < v.count; i++)
			res[i + count] = v[i];

		return res;
	}

	MyArrayChild SubSequence(int StartIndex = 0, int Length = -1)
	{
		if (StartIndex < 0 || StartIndex >= count)
			throw("index ", StartIndex, " out of range");

		if (Length == -1) Length = count - StartIndex;
		if (Length < 0)
			throw("subsequence can't have ", Length, " length");

		if (StartIndex + Length > count)
			cout << "The end of the subsequence can't be behind array end\n";

		MyArrayChild res;
		for (int i = StartIndex; i < count && Length > 0; i++)
		{
			res.push(ptr[i]);
			Length--;
		}
		return res;

	}

	/*  here i have to mark (show?) all elements,
		differing from average no more than diff */
	MyArrayChild Difference(int diff = 0)
	{
		diff = abs(diff);
		MyArrayChild res(100);

		if (count == 0)
		{
			cout << "\nOperation impossible: array is empty\n";

			return res;
		}

		double average = ptr[0];

		/*  average = (average * n + x)/(n + 1) =
			= average/(n + 1) * n + x/(n+1)*/
		//for (int i = 1; i < count; i++)
		//	average = average / (i + 1) * i + ptr[i] / (i + 1);

		for (int i = 1; i < count; i++) average += ptr[i];
		average /= count; 

		for (int i = 0; i < count; i++)
			if (abs(average - ptr[i]) < diff)
			{
				res.push(ptr[i]);				
			}

		return res;
	}
};

class MySortedArray : public MyArrayChild
{
private:
	/*  this function returns index of value
		if this value would be in the array
		i don't know if it works fully correctly
		or no but in my tests it did */
	int binary_search(double value)
	{
		if (count == 0) return 0;

		if (value < ptr[0]) return 0;
		if (value > ptr[count - 1]) return count; 

		int mid, left = 0, right = count - 1;

		while (true)
		{
			mid = (left + right) / 2;

			if (right == left + 1)
			{
				if (value < ptr[left]) return left;
				if (value > ptr[right]) return right + 1;

				return right;	
			}

			if (ptr[mid] < value)
				left = mid;
			else
				right = mid;
		}
	}
public:
	MySortedArray(int Dimension = 100) : MyArrayChild(Dimension)
	{
		cout << "MySortedArray constructor\n";
	}
	MySortedArray(double* arr, int len) : MyArrayChild(arr, len)
	{
		cout << "MySortedArray constructor\n";

		for (int i = 1; i < len; i++)
			if (ptr[i] < ptr[i - 1])
			{
				cout << "inputed array is not sorted";

				count = 0;
				capacity = 128;

				delete[] ptr;
				ptr = nullptr;
			}
	}
	~MySortedArray() { cout << "\nMySortedArray destructor\n"; }

	void push(double value)
	{
		int index = binary_search(value);

		InsertAt(value, index);
	}

	int IndexOf(double value)
	{
		int index = binary_search(value);
		const double inacc = 0.0001;

		if (count > 0 && abs(ptr[index] - value) < inacc)
			return index;
		//if (count > 0 && abs(ptr[probable_index - 1] - value) < inacc)
		//	return index;

		return -1;
	}

	/*  here i have to mark (show?) all elements,
		differing from average no more than diff */
	MySortedArray Difference(int diff = 0)
	{
		diff = abs(diff);
		MySortedArray res(100);

		if (count == 0)
		{
			cout << "\nOperation impossible: array is empty\n";

			return res;
		}

		double average = ptr[0];
		
		/*  average = (average * n + x)/(n + 1) =
			= average/(n + 1) * n + x/(n+1)*/
		//for (int i = 1; i < count; i++)
		//	average = average / (i + 1) * i + ptr[i] / (i + 1);

		for (int i = 1; i < count; i++) average += ptr[i];
		average /= count; 

		for (int i = 0; i < count; i++)
			if (abs(average - ptr[i]) < diff)
			{
				res.push(ptr[i]);				
			}

		return res;
	}
};

int main()
{
	if (true)
	{
		cout << "\ntesting basic operations\n";

		MyArrayChild arr;
		int i = 0;
		for (i = 0; i < 30; i++)
			arr.push(i + 1);
		arr.print();

		MyArrayChild arr2(10);
		for (int i = 0; i < 40; i++)
			arr2.push(i + 1);
		arr2.print();
		cout << "\n";

		arr = arr2;
		arr.print();

		MyArrayChild arr3 = arr;
		arr3.print();

		arr.RemoveLastValue();
		arr.print();

		cout << "\ntesting getcomponent and setcomponent\n";

		cout << "test1 " << arr.GetComponent(-1) << "\n";
		cout << "test2 " << arr.GetComponent(3) << "\n";

		arr.SetComponent(0, 30);
		arr.print();

		arr.SetComponent(-2, 10);
		arr.print();

		cout << "\ntesting removelastvalue\n";		

		MyArrayChild arr4(10);
		arr4.RemoveLastValue();

		MyArrayChild arr5(-1);
		arr5.RemoveLastValue();
		//cout << "\n" << sp << "\n";

		cout << "\ntesting removeat\n";

		arr.RemoveAt(-2);
		arr.print();

		arr.RemoveAt(-1);
		arr.print();

		arr.RemoveAt(0);
		arr.print();

		cout << "\ntesting indexof\n";

		cout << "test1 " << arr.IndexOf(-1) << "\n";
		cout << "test2 " << arr.IndexOf(2) << "\n";

		cout << "test3 " << arr.IndexOf(-1, false) << "\n";
		cout << "test4 " << arr.IndexOf(2, false) << "\n";

		cout << "\ntesting insertat\n";

		arr.InsertAt(10);
		arr.print();

		arr.InsertAt(10, -2);
		arr.print();

		arr.InsertAt(5, 3);
		arr.print();

		cout << "\ntesting +\n";

		arr = arr + arr2;
		arr.print();

		cout << "\ntesting difference\n";

		MyArrayChild tmp = arr2.Difference(10);
		tmp.print();

		tmp = arr2.Difference(5);
		tmp.print();

		cout << "\ntesting subsequence\n";

		//tmp = arr2.SubSequence(-1, 3); //programm stops
		//tmp.print();

		tmp = arr2.SubSequence(1, 3);
		tmp.print();

		tmp = arr2.SubSequence(3, 50);
		tmp.print();

		tmp = arr2.SubSequence(2, -1);

		cout << "\ntesting sorted arr\n";

		MySortedArray arr6(arr2.Arr(), arr2.Size());
		arr6.print();

		cout << "\n " << arr6.IndexOf(6) << "\n";

		arr6.push(33);
		arr6.push(33);
		arr6.push(33.5);
		arr6.print();
	}
	char c; cin >> c;
	return 0;
}