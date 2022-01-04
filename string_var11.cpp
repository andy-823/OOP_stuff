#include <iostream>

using namespace std;

class BaseString
{
protected:
	char* p;
	int len;
	int capacity;
public:
	BaseString(char* ptr)
	{
		cout << "\nBase Constructor 1\n";
		cout << "Maximum length is 255\n";

		capacity = 256;
		len = 0;

		p = new char[capacity];

		int i = 0;
		if (ptr != NULL)
		{		
			while(ptr[i] != '\0' && i < capacity - 1)
			{
				p[i] = ptr[i];
				i++;
				len = i;
			}
		}
		p[i] = '\0';
	}

	BaseString(int Capacity = 256)
	{
		cout<<"\nBase Constructor 0\n";

		capacity = Capacity;
		p = new char[capacity];
		p[0] = '\0';
		len = 0;
	}

	~BaseString()
	{
		cout<<"\nBase Destructor\n";

		if(p != NULL)
			delete[] p;
		len = 0;
	}

	BaseString& operator=(const BaseString &s)
	{
		cout << "\nbase operator = \n";

		len = s.len;
		if (s.len > capacity)
		{
			capacity = s.len + 1;
			delete[] p;
			p = new char[capacity];
		}

		for (int i = 0; i <= s.len; i++)
			p[i] = s.p[i];

		return *this;
	}

	BaseString(const BaseString& s) 
	{
		cout << "\nbase copy constructor\n";

		capacity = s.capacity;
		len = s.len;
		p = new char[capacity];

		for (int i = 0; i < s.len; i++)
			p[i] = s.p[i];
	}

	int Length() {return len;}
	int Capacity() { return capacity; }
	//char* get() {return p;}
	char& operator[](int i) {return p[i];}

	virtual void print()
	{
		for(int i = 0; p[i] != '\0'; i++)
			cout << p[i];
	}
};

class String : public BaseString
{
public:
	String(char *ptr) : BaseString(ptr) {}
	String(int capacity = 256) : BaseString(capacity) {}
	~String() {}

	String& operator=(const String& s)
	{
		cout << "\nstring operator = \n";

		len = s.len;
		if (len > capacity)
		{
			capacity = len + 1;
			delete[] p;
			p = new char[capacity];
		}

		for (int i = 0; i <= len; i++)
			p[i] = s.p[i];

		return *this;
	}

	String(const String& s)
	{
		cout << "\nstring copy constructor\n";

		capacity = s.capacity;
		p = new char[capacity];
		len = s.len;

		for (int i = 0; i <= len; i++)
			p[i] = s.p[i];
	}

	String operator+(String &s)
	{
		cout << "\nstring operator +\n";

		int new_capacity = len + s.len + 1;

		String res(new_capacity);

		for (int i = 0; i < len; i++)
			res[i] = p[i];
		for (int i = 0; i <= s.len; i++)
			res[i + len] = s[i];

		res.len = len + s.len;

		return res;
	}

	bool endwith(char* arr = {'\0'})
	{
		int arr_len = 0;

		if (arr != NULL)
		{		
			while(arr[arr_len] != '\0' && arr_len < capacity)
			{
				arr_len++;
			}
		}

		if (arr_len > len) return false;

		for (int i = 0; i <= arr_len; i++)
			if (arr[arr_len - i] != p[len - i]) return false;
		return true;
	}
};

int main()
{
	if (true)
	{
		String s("tst");
		s.print();

		String s1 = s;
		s1.print();

		String s2;
		s2 = s + s1;
		s2.print();

		s1 = s2 + s;
		s1.print();

		cout << "\nend is tst: " << s1.endwith("tst") << "\n";
		cout << "\nend is ast: " << s1.endwith("ast") << "\n";
		cout << "\nend is tat: " << s1.endwith("tat") << "\n";
		cout << "\nend is tsa: " << s1.endwith("tsa") << "\n";
	}
	char c; cin>>c;
	return 0;
}