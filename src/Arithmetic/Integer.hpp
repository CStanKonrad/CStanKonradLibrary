/*
The MIT License (MIT)

Copyright (c) 2016 CStanKonrad

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/


/* Based on  main.edu.pl course */
#ifndef CSTANKONRADLIBRARY_ARITHMETIC_INTEGER
#define CSTANKONRADLIBRARY_ARITHMETIC_INTEGER


#include <vector>
#include <algorithm>
#include <string>
#include <assert.h>



namespace CSKL
{

	typedef long long digit_type;

	template <typename T>
	int numberLength(T _number)	//number of digits in number
	{
		if (_number == 0)
			return 1;
		else
		{
			int result = 0;
			while (_number != 0)
			{
				++result;
				_number /= 10;
			}
			return result;
		}
	}

	class CInteger
	{
	private:
		std::vector<digit_type> digits;
		bool isNegative = false;
		digit_type DIGIT_BASE = 1000000000;
		digit_type DIGIT_SIZE = 9;
		
		void multiplyByDigit(digit_type _digit);
		void divideByDigit(digit_type _digit);
		
		std::string convertDigitToString(const digit_type _digit) const;
		
		void fixMinus0();
		void cutLeadingZeros();
		void subtractAbs(const CInteger &_num); // treats numbers like positive integers
		
	public:
		/* Constructors */
		
		CInteger();
		CInteger(long long _val);
		CInteger(const CInteger &_num);
		CInteger(const std::string &_num);
		
		/* end of Constructors */
	
	
		/* Utility */
		
		/* O(log(10^9, _val) */
		void assign(long long _val);
		
		/* O(_num.size()) */
		void assign(const std::string &_num);
		
		/* O(log(10^9, _num) */
		void assign(const CInteger &_num);
		
		
		
		/* O(log(10^9, _num) */
		void add(const CInteger &_num);
		
		/* O(log(10^9, _num)*O(log(10^9, *this) */
		void multiply(const CInteger &_num);
		
		/* O(log(2, 10^9)*O(log(10^9, _num)*(O(log(10^9, *this) - O(log(10^9, _num) */
		void divide(const CInteger &_num);
		
		
		/* end of Utility */
		
		
		
		/* Operators */
		/* O(log(10^9, _num) */
		bool operator == (const CInteger &_num) const;
		
		/* O(log(10^9, _num) */
		bool operator != (const CInteger &_num) const;
		
		/* O(log(10^9, _num) */
		bool operator < (const CInteger &_num) const;
		
		/* O(log(10^9, _num) */
		bool operator <= (const CInteger &_num) const;
		
		/* O(log(10^9, _num) */
		bool operator > (const CInteger &_num) const;
		
		/* O(log(10^9, _num) */
		bool operator >= (const CInteger &_num) const;
		
		
		
		/* O(log(10^9, _num) */
		CInteger operator+(const CInteger &_num) const;
		
		/* O(log(10^9, _num) */
		CInteger operator-(const CInteger &_num) const;
		
		friend CInteger operator-(const CInteger &_num);
		
		/* O(log(10^9, _num)*O(log(10^9, *this) */
		CInteger operator*(const CInteger &_num) const;
		
		/* O(log(2, 10^9)*O(log(10^9, _num)*(O(log(10^9, *this) - O(log(10^9, _num) */
		CInteger operator/(const CInteger &_num) const;
		
		/* O(log(2, 10^9)*O(log(10^9, _num)*(O(log(10^9, *this) - O(log(10^9, _num) */
		CInteger operator%(const CInteger &_num) const;
		
		/* end of Operators */
		
		friend CInteger abs(CInteger _num);
		
		/* O(returned_value.size() */
		std::string toString() const;
		friend std::ostream& operator<<(std::ostream &_os, const CInteger &_num);
		
		
		
		
	};

}
#endif
