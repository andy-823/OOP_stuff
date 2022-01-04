#include <iostream>
#include <functional>
#include <string>


using namespace std;

class album
{
private:
	string default_singer;
	string default_name;
	int default_count;
	int default_year;

	void helping_function(string value, string param_name = "name")
	{
		if (param_name == "singer") { default_singer = param_name; return;}
		if (param_name == "name") { default_name = param_name; return; }
		if (param_name == "count") { default_year = stoi(param_name); return; }
		if (param_name == "year") { default_year = stoi(param_name); return; }
		throw;
	}

	bool filter_singer(string param) { return param >= default_singer; }
	bool filter_name(string param) { return param >= default_name; }
	// bool filter_count(int param) {	return param >= default_count; } 
	// bool filter_year(int param)	{ return param >= default_year; }
public:
	string singer = "NA";
	string name = "NA";
	int count = -1;
	int year = -1;

	album(string param1 = "NA", string param2 = "NA", int param3 = -1, int param4 = -1)
	{
		singer = param1; name = param2;
		count = param3; year = param4;
	}

	// typedef bool (*filter)(string param);
	// // (string value, string name)
	// filter f(string value, string name)
	bool (*filter(string value, string name))(string)
	//void filter(string value, string name, bool(*ptr)(string))
	{
		try
		{
			helping_function(value, name);
			if (name == "singer") return &filter_singer;
			if (name == "name") return filter_name;
			// if (name == "singer") ptr = filter_singer;
			// if (name == "name") ptr = filter_name;
			// if (name == "count") return filter_count;
			// if (name == "year") return filter_year;
		}
		catch(...)
		{
			cout << "incorrect param name: " + name << "\n";
		}
	}

	friend ostream &operator << (ostream &s, album el);
};

ostream &operator << (ostream &s, album el)
{
	s << "singer: " << el.singer << "\n";
	s << "name: " << el.name << "\n";
	s << "songs count: " << el.count << "\n";
	s << "release year: " << el.year << "\n";
	return s;
}

template<typename T>
class Element
{
public:
	Element* next;
	Element* prev;
	T info;

	Element(T data)
	{
		next = prev = NULL;
		info = data;
	}
	Element(Element* Next, Element* Prev, T data)
	{
		next = Next;
		prev = Prev;
		info = data;
	}
	Element(const Element& el)
	{
		next = el.next;
		prev = el.prev;
		info = el.info;
	}

    //  вводим/выводим только info
	template<typename T1>
    friend istream& operator >> (istream& s, Element<T1>& el);
	template<typename T1>
	friend ostream& operator << (ostream& s, Element<T1>& el);
};

template<typename T>
ostream& operator >> (ostream& s, Element<T>& el)
{
	s >> el.info;
	return s;
}
template<typename T>
ostream& operator << (ostream& s, Element<T>& el)
{
	s << el.info;
	return s;
}

template<typename T>
class linked_list
{
protected:
	Element<T>* head;
	Element<T>* tail;
	int count;
public:
	linked_list()
	{
		head = tail = nullptr;
		count = 0;
	}
	virtual ~linked_list() 
	{
		cout << "\nBase class destructor";

		Element<T> *current = head;
		while (current != nullptr)
		{
			Element<T> *tmp = current;
			current = current -> next;
			delete tmp;
		}
		head = nullptr;
		tail = nullptr;
		count = 0;
	}

    virtual void pop();
	virtual void push(T value);
    
    bool empty() { return count == 0; }

    template<typename T1>
	friend ostream& operator<<(ostream& s, linked_list<T1>& el);
};

template<typename T>
ostream& operator<<(ostream& s, linked_list<T>& el)
{
	Element<T>* current;
	for (current = el.head; current != nullptr; current = current->next)
		s << *current << "; ";
	return s;
}

template<typename T>
class stack : public linked_list<T>
{
private:
	Element<T> *operator[](int index)
	{
		Element<T>* current = this -> head;
		for (int i = 0;
			current != nullptr && i < index;
			current = current->next, i++);
		return current;
	}
public:
	stack() : linked_list<T>() {};

	T get_last() { return this -> tail -> info; }

	virtual void pop()
	{
		if (this -> tail == nullptr) return;

		this -> count--;
		Element<T> *tmp = this -> tail;
		delete tmp;

		this -> tail = this -> tail -> prev;
		return;
	}
	virtual void push(T value)
	{
		this -> count++;
		Element<T> *element = new Element<T>(value);

		element -> prev = this -> tail;
		element -> next = nullptr;
		if (this->count == 1)
		{
			this -> head = this -> tail = element;
		}
		else
		{
			this -> tail -> next = element;
			this -> tail = element;
		}
	}

	void insert(T value, int index)
	{
		if (index < 0 || index > this -> count) throw;

		Element<T> *element = new Element<T>(value);
		if (index == this -> count)
		{
			element -> prev = this -> tail;
			element -> next = nullptr;
			this -> tail -> next = element;
			this -> tail = element;
			return;
		}
		Element<T> *current = this -> head;
		for (int i = 0; i < index; current = current -> next, i++);

		element -> prev = current -> prev;
		element -> next = current;
		current -> prev -> next = element;
		current -> prev = element;
		return;
	}
	void remove(int index)
	{
		if (index < 0 || index >= this -> count) throw;

		if (index == this -> count - 1)
		{
			this -> tail = this -> tail -> prev;
			delete this -> tail -> next;
			this -> tail -> next = nullptr;
			return;
		}
		if (index == 0)
		{
			this -> head = this -> head -> next;
			delete this -> head -> prev;
			this -> head -> prev = nullptr;
			return;
		}
		Element<T> *current = this -> head;
		for (int i = 0; i < index; current = current -> next, i++);
		current -> prev -> next = current -> next;
		current -> next -> prev = current -> prev;
		delete current;
		return;
	}

	Element<T> *Find(T value, bool (*ptr)(T))
	{
		Element<T> *current = this -> head;
		for (; current != nullptr; current = current -> hext)
			if (ptr(value, current -> info)) return *current;
		return nullptr;
	}
	Element<T> *Find_r(T value, bool (*ptr)(T), Element<T> *current = nullptr)
	{
		if (current == nullptr) current = this -> head;
		if (this -> head == nullptr) return nullptr;

		if (ptr(value, current -> info)) return current;

		if (current == this -> tail) return nullptr;
		return Find_r(value, current -> next);
	}

	void Filter(stack<T> *Res, 	bool (*ptr)(T))
	{
		Element<T> *current = this-> head;
		for (; current != nullptr; current = current -> hext)
			if (ptr(current -> info)) Res -> push(current -> info);
	}
	void Filter_r(stack<T> *Res, bool (*ptr)(T), Element<T> *current = nullptr)
	{
		if (current == nullptr) current = this -> head;
		if (this -> head == nullptr) return;

		if (ptr(current -> info)) Res -> push(current -> info);

		if (current == this -> tail) return;
		Filter_r(Res, ptr, current);
	}
};

int main()
{
	stack<album> *list = new stack<album>;
	for (int i = 0; i < 10; i++)
	{
		list -> push(album('a' + to_string(i), 'b' + to_string(i), i, i + 2000));
	}

}

//queue.filter(res, el.filter("aaa", "singer"))