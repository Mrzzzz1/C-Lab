#include"Head.h"
template<typename T>
MAT<T>::MAT(int r, int c) :r(r), c(c), e(new T[r * c]) {}

template<typename T>
MAT<T>::MAT(const MAT& a) : e(new T[a.r * a.c]), r(a.r), c(a.c)
{
	for (int i = 0; i < a.r * a.c; i++)
	{
		e[i] = a.e[i];
	}
}

template<typename T>
MAT<T>::MAT(MAT&& a) noexcept :e(a.e), r(a.r), c(a.c)
{
	*(T*)&(a.c) = 0;
	*(T*)&(a.r) = 0;
	*(T**)&(a.e) = NULL;
}

template<typename T>
MAT<T>::~MAT() noexcept
{
	if (e != nullptr)
	{
		delete []e;
		*(T**)&e = nullptr;
		*(int*)&r = 0;
		*(int*)&c = 0;
	}
}

template<typename T>
T* const MAT<T>::operator[](int r)
{
	if (r < this->r && r >= 0)
	{
		return &e[r * c];
	}
	else
	{
		throw("Operator[] error!");
	}
}

template<typename T>
MAT<T> MAT<T>::operator+(const MAT& a) const
{
	if (r == a.r && c == a.c)
	{
		MAT m(r, c);
		int tmp = 0, i = 0;
		for (i = 0; i < r * c; i++)
		{
			m.e[i] = e[i] + a.e[i];
		}
		return m;

	}
	throw("Operator+ error!");
}

template<typename T>
MAT<T> MAT<T>::operator-(const MAT& a) const
{
	if (r == a.r && c == a.c)
	{
		MAT m(r, c);
		int i = 0;
		for (i = 0; i < r * c; i++)
		{
			m.e[i] = e[i] - a.e[i];
		}
		return m;
	}
	throw("Operator- error!");
}

template<typename T>
MAT<T> MAT<T>::operator*(const MAT& a) const
{
	if (c == a.r)
	{
		MAT m(r, a.c);
		int i = 0, j = 0, k = 0;
		for (i = 0; i < r; i++)
		{
			for (j = 0; j < a.c; j++)
			{
				m[i][j] = 0;
				for (k = 0; k < c; k++)
				{
					m[i][j] += e[i * c + k] * a.e[k * c + j];
				}
			}
		}
		return m;
	}
	throw("Operator* error!");
}

template<typename T>
MAT<T> MAT<T>::operator~() const
{
	MAT m(c, r);
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			m[j][i] = e[i * c + j];
		}
	}
	return m;
}

template<typename T>
MAT<T>& MAT<T>::operator=(const MAT& a)
{
	if (this != &a)
	{
		if (e != nullptr)
		{
			delete []e;
		}
		*(T**)&e = new T [r*c];
		for (int i = 0; i < r * c; i++)
		{
			e[i] = a.e[i];
		}
		*(int*)&r = a.r;
		*(int*)&c = a.c;
	}
	return *this;
}

template<typename T>
MAT<T>& MAT<T>::operator=(MAT&& a) noexcept
{
	if (this != &a)
	{
		if (e != nullptr)
		{
			delete []e;
		}
		*(T**)&e = a.e;
		*(int*)&r = a.r;
		*(int*)&c = a.c;

		*(T**)&a.e = nullptr;
		*(int*)&a.r = 0;
		*(int*)&a.c = 0;
		return *this;
	}
}

template<typename T>
MAT<T>& MAT<T>::operator+=(const MAT& a)
{
	*this = *this + a;
	return *this;
}

template<typename T>
MAT<T>& MAT<T>::operator-=(const MAT& a)
{
	*this = *this - a;
	return *this;
}

template<typename T>
MAT<T>& MAT<T>::operator*=(const MAT& a)
{
	*this = *this * a;
		return *this;
}

template<typename T>
char* MAT<T>::print(char* s) const noexcept
{
	int n = 0;
	if (typeid(e[0]) == typeid(int))
	{
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				if (j != 0)
				{
					n += sprintf(s + n, "%c", ' ');
				}
				n += sprintf(s + n, "%6ld", e[i * c + j]);
				if (j == c - 1)
				{
					n += sprintf(s + n, "%c", '\n');
				}
			}
		}
	}
	else if (typeid(e[0]) == typeid(long))
	{
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				if (j != 0)
				{
					n += sprintf(s + n, "%c", ' ');
				}
				n += sprintf(s + n, "%6lld", e[i * c + j]);
				if (j == c - 1)
				{
					n += sprintf(s + n, "%c", '\n');
				}
			}
		}
	}
	else if (typeid(e[0]) == typeid(float))
	{
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				if (j != 0)
				{
					n += sprintf(s + n, "%c", ' ');
				}
				n += sprintf(s + n, "%8f", e[i * c + j]);
				if (j == c - 1)
				{
					n += sprintf(s + n, "%c", '\n');
				}
			}
		}
	}
	else if (typeid(e[0]) == typeid(double))
	{
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				if (j != 0)
				{
					n += sprintf(s + n, "%c", ' ');
				}
				n += sprintf(s + n, "%8lf", e[i * c + j]);
				if (j == c - 1)
				{
					n += sprintf(s + n, "%c", '\n');
				}
			}
		}
	}
	s[n] = '\0';
	printf("%s", s);
	return s;
}