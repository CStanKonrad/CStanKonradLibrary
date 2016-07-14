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

	void CInteger::multiplyByDigit(digit_type digit_)
	{
		assert(digit_ < DIGIT_BASE && digit_ >= 0);
		digit_type move = 0;
		for (unsigned i = 0; i < digits.size(); ++i)
		{
			digits[i] *= digit_;
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

	void CInteger::divideByDigit(digit_type digit_)
	{
		assert(digit_ < DIGIT_BASE && digit_ > 0);

		digit_type move = 0, tmp;
		for (int i = digits.size() - 1; i >= 0; --i)
		{
			tmp = digits[i];
			digits[i] = (digits[i] + move*DIGIT_BASE)/digit_;
			move = (tmp + move*DIGIT_BASE) % digit_;
		}
		cutLeadingZeros();
		fixMinus0();
	}

	std::string CInteger::convertDigitToString(const digit_type digit_) const
	{
		std::string result;
		int len = numberLength(digit_);

		for (int i = 1; i <= DIGIT_SIZE - len; ++i)
		{
			result += "0";
		}
		result += std::to_string(digit_);
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

	void CInteger::subtractAbs(const CInteger &num_)
	{
		digit_type move = 0;
		for (unsigned i = 0; i < this->digits.size(); ++i)
		{
			if (i < num_.digits.size())
				digits[i] += -num_.digits[i] + move;
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
	CInteger::CInteger(long long val_)
	{
		assign(val_);
	}
	CInteger::CInteger(const CInteger &num_)
	{
		assign(num_);
	}
	CInteger::CInteger(const std::string &num_)
	{
		assign(num_);
	}

	/* end of Constructors */

	/* Utility */

	void CInteger::assign(long long val_)
	{
		if (val_ < 0)
		{
			isNegative = true;
			val_ = -val_;
		}
		else
			isNegative = false;

		digits.clear();
		if (val_ == 0)
		{
			digits.push_back(0);
		}
		while (val_ > 0)
		{
			digits.push_back(val_%DIGIT_BASE);
			val_ /= DIGIT_BASE;
		}
		return;
	}

	void CInteger::assign(const std::string &num_)
	{
		digits.clear();
		int e = 0;
		if (num_[0] == '-')
		{
			isNegative = true;
			e = 1;
		}
		else
			isNegative = false;

		digit_type buffer = 0;
		digit_type factor = 1;
		int len = 0;
		for (int i = num_.size() - 1; i >= e; --i)
		{
			buffer += factor*(num_[i] - '0');
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

	void CInteger::assign(const CInteger &num_)
	{
		isNegative = num_.isNegative;
		digits.clear();
		for (unsigned i = 0; i < num_.digits.size(); ++i)
		{
			digits.push_back(num_.digits[i]);
		}
	}


	void CInteger::add(const CInteger &num_)
	{
		if (this->isNegative == num_.isNegative)
		{
			for (unsigned i = std::max(this->digits.size(), num_.digits.size()) + 1; digits.size() < i; digits.push_back(0));

			for (unsigned i = 0; i < num_.digits.size(); ++i)
			{
				this->digits[i] += num_.digits[i];

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
				tmp = -num_;
				if (*this < tmp)
				{
					tmp = *this;
					*this = num_;

				}
				subtractAbs(tmp);
			}
			else
			{
				tmp = num_;
				if (-(*this) < num_)
				{
					tmp = *this;
					*this = num_;

				}
				subtractAbs(tmp);
			}
		}
	}

	void CInteger::multiply(const CInteger &num_)
	{
		this->isNegative = this->isNegative xor num_.isNegative;
		CInteger tmp, buf = *this;
		*this = 0;
		for (unsigned i = 0, oldSize; i < num_.digits.size(); ++i)
		{
			tmp = buf;
			tmp.multiplyByDigit(num_.digits[i]);
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

	void CInteger::divide(const CInteger &num_)
	{
		assert(num_ != 0);
		CInteger  absNum = abs(num_), absThis = abs(*this), tmp;
		tmp = absNum;
		if (absNum > absThis)
		{
			*this = 0;
			return;
		}
		this->isNegative = this->isNegative xor num_.isNegative;



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

	bool CInteger::operator == (const CInteger &num_) const
	{
		if ((this->digits.size() != num_.digits.size()) || (this->isNegative != num_.isNegative))
			return false;

		for (int i = this->digits.size() - 1; i >= 0; --i)
		{
			if (this->digits[i] != num_.digits[i])
				return false;
		}
		return true;
	}

	bool CInteger::operator != (const CInteger &num_) const
	{
		return !((*this) == num_);
	}

	bool CInteger::operator < (const CInteger &num_) const
	{
		if (this->isNegative == true && num_.isNegative == false)
			return true;
		else if (this->isNegative == false && num_.isNegative == true)
			return false;

		if ((this->digits.size() < num_.digits.size()))
			return !this->isNegative;
		else if ((this->digits.size() > num_.digits.size()))
			return this->isNegative;
		else
		{
			for (int i = this->digits.size() - 1; i >= 0; --i)
			{
				if (this->digits[i] < num_.digits[i])
					return !this->isNegative;
				else if (this->digits[i] > num_.digits[i])
					return this->isNegative;
			}
			return false;
		}
	}

	bool CInteger::operator <= (const CInteger &num_) const
	{
		return (*this < num_ || *this == num_);
	}

	bool CInteger::operator > (const CInteger &num_) const
	{
		return !(*this <= num_);
	}

	bool CInteger::operator >= (const CInteger &num_) const
	{
		return !(*this < num_);
	}



	CInteger CInteger::operator+(const CInteger &num_) const
	{
		CInteger result(*this);
		result.add(num_);
		return result;
	}

	CInteger CInteger::operator-(const CInteger &num_) const
	{
		CInteger result(*this);
		result.add(-num_);
		return result;
	}
	CInteger operator-(const CInteger &num_)
	{
		CInteger result = num_;
		result.isNegative = !result.isNegative;
		result.fixMinus0();
		return result;
	}

	CInteger CInteger::operator*(const CInteger &num_) const
	{
		CInteger result(*this);
		result.multiply(num_);
		return result;
	}

	CInteger CInteger::operator/(const CInteger &num_) const
	{
		CInteger result(*this);
		result.divide(num_);
		return result;
	}
	CInteger CInteger::operator%(const CInteger &num_) const
	{
		assert(num_.isNegative == false);
		CInteger tmp = *this;
		if (tmp.isNegative == true)
		{
			tmp.isNegative = false;
			tmp = num_ - (tmp%num_);
		}
		return ((tmp) - (((tmp)/num_)*(num_)));
	}

	/* end of Operators */

	CInteger abs(CInteger num_)
	{
		num_.isNegative = false;
		return num_;
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

	std::ostream& operator<<(std::ostream &os_, const CInteger &num_)
	{
		os_ << num_.toString();
		return os_;
	}



}
