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

#include "Integer.hpp"


namespace CSKL
{
	
	void CInteger::multiplyByDigit(digit_type _digit)
	{
		assert(_digit < DIGIT_BASE && _digit >= 0);
		digit_type move = 0;
		for (unsigned i = 0; i < digits.size(); ++i)
		{
			digits[i] *= _digit;
			digits[i] += move;
			move = digits[i]/DIGIT_BASE;
			digits[i] %= DIGIT_BASE;
		}
		
		while (move > 0)
		{
			digits.push_back(move % DIGIT_BASE);
			move /= DIGIT_BASE;
		}
		cutLeadingZeros();
		fixMinus0();
	}
	
	void CInteger::divideByDigit(digit_type _digit)
	{
		assert(_digit < DIGIT_BASE && _digit > 0);
		
		digit_type move = 0, tmp;
		for (int i = digits.size() - 1; i >= 0; --i)
		{
			tmp = digits[i];
			digits[i] = (digits[i] + move*DIGIT_BASE)/_digit;
			move = (tmp + move*DIGIT_BASE) % _digit;
		}
		cutLeadingZeros();
		fixMinus0();
	}
	
	std::string CInteger::convertDigitToString(const digit_type _digit) const
	{
		std::string result;
		int len = numberLength(_digit);
		
		for (int i = 1; i <= DIGIT_SIZE - len; ++i)
		{
			result += "0";
		}
		result += std::to_string(_digit);
		return result;
	}
	
	void CInteger::fixMinus0()
	{
		assert(digits.size() != 0);
		if (digits.size() == 1 && digits[0] == 0)
		{
			isNegative = false;
		}
	}
	
	void CInteger::cutLeadingZeros()
	{
		for (unsigned i = digits.size() - 1; i > 0; --i)
		{
			if (digits[i] != 0)
				break;
			else
			{
				digits.pop_back();
			}
		}
	}
	
	void CInteger::subtractAbs(const CInteger &_num)
	{
		digit_type move = 0;
		for (unsigned i = 0; i < this->digits.size(); ++i)
		{
			if (i < _num.digits.size())
				digits[i] += -_num.digits[i] + move;
			else
				digits[i] += move;
			if (digits[i] < 0)
			{
				digits[i] += DIGIT_BASE;
				move = -1;
			}
			else
			{
				move = 0;
			}
		}
		cutLeadingZeros();
		fixMinus0();
	}
	
	/* Constructors */
		
	CInteger::CInteger() 
	{
		assign(0);
	}
	CInteger::CInteger(long long _val)
	{
		assign(_val);
	}
	CInteger::CInteger(const CInteger &_num)
	{
		assign(_num);
	}
	CInteger::CInteger(const std::string &_num)
	{
		assign(_num);
	}
		
	/* end of Constructors */
	
	/* Utility */
		
	void CInteger::assign(long long _val)
	{
		if (_val < 0)
		{
			isNegative = true;
			_val = -_val;
		}
		else
			isNegative = false;
		
		digits.clear();
		if (_val == 0)
		{
			digits.push_back(0);
		}
		while (_val > 0)
		{
			digits.push_back(_val%DIGIT_BASE);
			_val /= DIGIT_BASE;
		}
		return;
	}
	
	void CInteger::assign(const std::string &_num)
	{
		digits.clear();
		int e = 0;
		if (_num[0] == '-')
		{
			isNegative = true;
			e = 1;
		}
		else
			isNegative = false;
		
		digit_type buffer = 0;
		digit_type factor = 1;
		int len = 0;
		for (int i = _num.size() - 1; i >= e; --i)
		{
			buffer += factor*(_num[i] - '0');
			factor *= 10;
			++len;
			
			if (len == DIGIT_SIZE)
			{
				len = 0;
				digits.push_back(buffer);
				buffer = 0;
				factor = 1;
			}
		
		}
		if (buffer != 0)
		{
			digits.push_back(buffer);
		}
		if (digits.size() == 0)
			digits.push_back(0);
		fixMinus0();
		return;
	}
	
	void CInteger::assign(const CInteger &_num)
	{
		isNegative = _num.isNegative;
		digits.clear();
		for (unsigned i = 0; i < _num.digits.size(); ++i)
		{
			digits.push_back(_num.digits[i]);
		}
	}
	

	void CInteger::add(const CInteger &_num)
	{
		if (this->isNegative == _num.isNegative)
		{
			for (unsigned i = std::max(this->digits.size(), _num.digits.size()) + 1; digits.size() < i; digits.push_back(0));
			
			for (unsigned i = 0; i < _num.digits.size(); ++i)
			{
				this->digits[i] += _num.digits[i];
				
				this->digits[i + 1] += this->digits[i]/DIGIT_BASE;
				this->digits[i] %= DIGIT_BASE;
			}
			
			for (int i = this->digits.size() - 1; ;++i)
			{
				if (this->digits[i] < DIGIT_BASE)
					break;
				else
				{
					this->digits.push_back(this->digits[i]/DIGIT_BASE);
					this->digits[i] %= DIGIT_BASE;
				}
			}
			
			if (this->digits[this->digits.size() - 1] == 0 && this->digits.size() != 1)
				this->digits.pop_back();
		}
		else
		{
			CInteger tmp;
			if (this->isNegative == false)
			{
				tmp = -_num;
				if (*this < tmp)
				{
					tmp = *this;
					*this = _num;
					
				}
				subtractAbs(tmp);
			}
			else
			{
				tmp = _num;
				if (-(*this) < _num)
				{
					tmp = *this;
					*this = _num;
					
				}
				subtractAbs(tmp);
			}
		}
	}
	
	void CInteger::multiply(const CInteger &_num)
	{
		this->isNegative = this->isNegative xor _num.isNegative;
		CInteger tmp, buf = *this;
		*this = 0;
		for (unsigned i = 0, oldSize; i < _num.digits.size(); ++i)
		{
			tmp = buf;
			tmp.multiplyByDigit(_num.digits[i]);
			oldSize = tmp.digits.size();
			for (unsigned j = 0; j < i; ++j)
				tmp.digits.push_back(0);
			for (unsigned j = 0; j < oldSize; ++j)
				 tmp.digits[tmp.digits.size() - 1 - j] = tmp.digits[oldSize - 1 - j];
			for (unsigned j = 0; j < i; ++j)
				 tmp.digits[j] = 0;
			
			this->add(tmp);
		}
		fixMinus0();
		cutLeadingZeros();
	}
	
	void CInteger::divide(const CInteger &_num)
	{
		assert(_num != 0);
		CInteger  absNum = abs(_num), absThis = abs(*this), tmp;
		tmp = absNum;
		if (absNum > absThis)
		{
			*this = 0;
			return;
		}
		this->isNegative = this->isNegative xor _num.isNegative;
		
		
		
		for (int i = 0; i < (int)tmp.digits.size(); ++i)
		{
			tmp.digits[tmp.digits.size() - 1 - i] = absThis.digits[absThis.digits.size() - 1 - i];
		}
		
		
		int i = absThis.digits.size() - tmp.digits.size();
		while ((int)this->digits.size() > i + 1)
			this->digits.pop_back();
		
		while (i >= 0)
		{
			digit_type begin = 0, end = DIGIT_BASE, target = (begin + end)/2;	//[begin;end)
			while (end - begin > 1)
			{
				if (absNum*target > tmp)
					end = target;
				else
					begin = target;
				target = (begin + end)/2;
			}
			
			this->digits[i] = begin;

			tmp = tmp - (absNum*begin);
			
			tmp.digits.push_back(0);
			for (int j = tmp.digits.size() - 2; j >= 0; --j)
			{
				tmp.digits[j + 1] = tmp.digits[j];
			}
			tmp.cutLeadingZeros();
			
			--i;
			
			if (i >= 0)
				tmp.digits[0] = absThis.digits[i];
			
			
		}
		
		fixMinus0();
		cutLeadingZeros();
	}
		
	/* end of Utility */
	
	
	/* Operators */
		
	bool CInteger::operator == (const CInteger &_num) const
	{
		if ((this->digits.size() != _num.digits.size()) || (this->isNegative != _num.isNegative))
			return false;
		
		for (int i = this->digits.size() - 1; i >= 0; --i)
		{
			if (this->digits[i] != _num.digits[i])
				return false;
		}
		return true;
	}
	
	bool CInteger::operator != (const CInteger &_num) const
	{
		return !((*this) == _num);
	}
	
	bool CInteger::operator < (const CInteger &_num) const
	{
		if (this->isNegative == true && _num.isNegative == false)
			return true;
		else if (this->isNegative == false && _num.isNegative == true)
			return false;
			
		if ((this->digits.size() < _num.digits.size()))
			return !this->isNegative;
		else if ((this->digits.size() > _num.digits.size()))
			return this->isNegative;
		else
		{
			for (int i = this->digits.size() - 1; i >= 0; --i)
			{
				if (this->digits[i] < _num.digits[i])
					return !this->isNegative;
				else if (this->digits[i] > _num.digits[i])
					return this->isNegative;
			}
			return false;
		}
	}
	
	bool CInteger::operator <= (const CInteger &_num) const
	{
		return (*this < _num || *this == _num);
	}
	
	bool CInteger::operator > (const CInteger &_num) const
	{
		return !(*this <= _num);
	}
	
	bool CInteger::operator >= (const CInteger &_num) const
	{
		return !(*this < _num);
	}
	
	
	
	CInteger CInteger::operator+(const CInteger &_num) const
	{
		CInteger result(*this);
		result.add(_num);
		return result;
	}
	
	CInteger CInteger::operator-(const CInteger &_num) const
	{
		CInteger result(*this);
		result.add(-_num);
		return result;
	}
	CInteger operator-(const CInteger &_num)
	{
		CInteger result = _num;
		result.isNegative = !result.isNegative;
		result.fixMinus0();
		return result;
	}
	
	CInteger CInteger::operator*(const CInteger &_num) const
	{
		CInteger result(*this);
		result.multiply(_num);
		return result;
	}
	
	CInteger CInteger::operator/(const CInteger &_num) const
	{
		CInteger result(*this);
		result.divide(_num);
		return result;
	}
	CInteger CInteger::operator%(const CInteger &_num) const
	{
		assert(_num.isNegative == false);
		CInteger tmp = *this;
		if (tmp.isNegative == true)
		{
			tmp.isNegative = false;
			tmp = _num - (tmp%_num);
		}
		return ((tmp) - (((tmp)/_num)*(_num)));
	}
	
	/* end of Operators */
	
	CInteger abs(CInteger _num)
	{
		_num.isNegative = false;
		return _num;
	}
	
	std::string CInteger::toString() const
	{
		std::string result = (isNegative == true ? "-" : "");
		result += std::to_string(digits[digits.size() - 1]);
		for (int i = digits.size() - 2; i >= 0; --i)
		{
			result += convertDigitToString(digits[i]);
		}
		return result;
	}
	
	std::ostream& operator<<(std::ostream &_os, const CInteger &_num)
	{
		_os << _num.toString();
		return _os;
	}
	
	
	
}
