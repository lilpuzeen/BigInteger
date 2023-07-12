#include "LN.h"

template< typename T >
T max(const T &a, const T &b)
{
	return a > b ? a : b;
}

template< typename T >
T min(const T &a, const T &b)
{
	return a < b ? a : b;
}

LN::LN(long long value)
{
	sign_ = 1;
	digits_ = new uint32_t[CAPACITY];
	for (int i = 0; i < CAPACITY; ++i)
	{
		digits_[i] = 0;
	}
	if (value < 0)
	{
		sign_ = -1;
		value = -value;
	}
	while (value != 0)
	{
		digits_[size_++] = value % NUMERAL_SYSTEM;
		value /= NUMERAL_SYSTEM;
	}
}

LN::LN(const char *str, size_t size)
{
	digits_ = new uint32_t[CAPACITY];
	for (int i = 0; i < CAPACITY; ++i)
	{
		digits_[i] = 0;
	}
	if (str[0] == '-')
	{
		sign_ = -1;
		this->size_ = size - 1;
		int pos = 0;
		for (int i = size - 1; i >= 1; i--)
		{
			digits_[pos++] = str[i] - '0';
		}
	}
	else
	{
		this->size_ = size;
		sign_ = 1;
		int pos = 0;
		for (int i = size - 1; i >= 0; i--)
		{
			digits_[pos++] = str[i] - '0';
		}
	}
}

LN::LN(std::string_view str)
{
	digits_ = new uint32_t[CAPACITY];
	for (int i = 0; i < CAPACITY; ++i)
	{
		digits_[i] = 0;
	}

	if (str[0] == '-')
	{
		sign_ = -1;
		size_ = str.size() - 1;
		int pos = 0;
		for (int i = str.size() - 1; i >= 1; i--)
		{
			digits_[pos++] = str[i] - '0';
		}
	}
	else
	{
		size_ = str.size();
		sign_ = 1;
		int pos = 0;
		for (int i = str.size() - 1; i >= 0; i--)
		{
			digits_[pos++] = str[i] - '0';
		}
	}
}

LN::LN(LN &other)
{
	digits_ = new uint32_t[CAPACITY];
	for (int i = 0; i < CAPACITY; ++i)
	{
		digits_[i] = 0;
	}
	size_ = other.size_;
	sign_ = other.sign_;
	is_nan_ = other.is_nan_;
	for (int i = 0; i < size_; ++i)
	{
		digits_[i] = other.digits_[i];
	}
}

LN::LN(const LN &other)
{
	digits_ = new uint32_t[CAPACITY];
	for (int i = 0; i < CAPACITY; ++i)
	{
		digits_[i] = 0;
	}
	size_ = other.size_;
	sign_ = other.sign_;
	is_nan_ = other.is_nan_;
	for (int i = 0; i < size_; ++i)
	{
		digits_[i] = other.digits_[i];
	}
}

LN::LN(LN &&other)
{
	size_ = other.size_;
	sign_ = other.sign_;
	is_nan_ = other.is_nan_;
	digits_ = other.digits_;
	other.digits_ = nullptr;
}

LN::~LN()
{
	delete[] digits_;
}

LN &LN::operator=(const LN &other)
{
	if (this != &other)
	{
		size_ = other.size_;
		sign_ = other.sign_;
		is_nan_ = other.is_nan_;
		for (int i = 0; i < size_; ++i)
		{
			digits_[i] = other.digits_[i];
		}
	}
	return *this;
}

LN &LN::operator=(LN &&other) noexcept
{
	if (this != &other)
	{
		size_ = other.size_;
		sign_ = other.sign_;
		is_nan_ = other.is_nan_;
		delete[] digits_;
		digits_ = other.digits_;
		other.digits_ = nullptr;
	}
	return *this;
}

LN &operator-(LN &a)
{
	a.sign_ = a.sign_ == 1 ? -1 : 1;
	return a;
}

LN operator""_ln(const char *str)
{
	return LN{ str };
}

LN operator~(const LN &a)
{
	if (a.sign_ == -1)
	{
		LN temp;
		temp.is_nan_ = true;
		return temp;
	}
	LN b = a;
	while (b * b > a)
	{
		b = (b + a / b) / 2;
	}
	return b;
}

LN operator+(const LN &a, const LN &b)
{
	if (a.isNan() || b.isNan())
	{
		LN temp;
		temp.is_nan_ = true;
		return temp;
	}
	if (a.sign_ != b.sign_)
	{
		if (greater_than(a, b))
		{
			LN result = minus(a, b);
			result.sign_ = a.sign_;
			return result;
		}
		else
		{
			LN result = minus(b, a);
			result.sign_ = b.sign_;
			return result;
		}
	}
	else
	{
		LN result = plus(a, b);
		result.sign_ = a.sign_;
		return result;
	}
}

LN &operator+=(LN &a, const LN &b)
{
	a = a + b;
	return a;
}

LN operator-(const LN &a, const LN &b)
{
	if (a.isNan() || b.isNan())
	{
		LN temp;
		temp.is_nan_ = true;
		return temp;
	}
	if (a.sign_ != b.sign_)
	{
		LN result = plus(a, b);
		result.sign_ = a.sign_;
		return result;
	}
	else
	{
		if (a.sign_ == 1)
		{
			if (a.abs() > b.abs())
			{
				LN result = minus(a, b);
				result.sign_ = 1;
				return result;
			}
			else
			{
				LN result = minus(b, a);
				result.sign_ = -1;
				return result;
			}
		}
		else
		{
			if (a.abs() > b.abs())
			{
				LN result = minus(a, b);
				result.sign_ = -1;
				return result;
			}
			else
			{
				LN result = minus(b, a);
				result.sign_ = 1;
				return result;
			}
		}
	}
}

LN &operator-=(LN &a, const LN &b)
{
	a = a - b;
	return a;
}

LN operator*(const LN &a, const LN &b)
{
	if (a.isNan() || b.isNan())
	{
		LN temp;
		temp.is_nan_ = true;
		return temp;
	}
	if (a.sign_ != b.sign_)
	{
		LN result;
		result = composition(a, b);
		result.sign_ = -1;
		return result;
	}
	else
	{
		LN result;
		result = composition(a, b);
		result.sign_ = 1;
		return result;
	}
}

LN &operator*=(LN &a, const LN &b)
{
	a = a * b;
	return a;
}

LN operator/(const LN &a, const LN &b)
{
	if (a.isNan() || b.isNan())
	{
		LN temp;
		temp.is_nan_ = true;
		return temp;
	}
	if (b.isZero())
	{
		LN temp;
		temp.is_nan_ = true;
		return temp;
	}
	if (b == 1 || b == -1)
	{
		if (a.sign_ == b.sign_)
		{
			return a.abs();
		}
		else
		{
			LN result = a;
			result.sign_ = -1;
			return result;
		}
	}
	if (a.sign_ != b.sign_)
	{
		LN result;
		result = division(a.abs(), b.abs());
		result.sign_ = -1;
		return result;
	}
	else
	{
		LN result;
		result = division(a.abs(), b.abs());
		result.sign_ = 1;
		return result;
	}
}

LN &operator/=(LN &a, const LN &b)
{
	a = a / b;
	return a;
}

LN operator%(const LN &a, const LN &b)
{
	if (a.isNan() || b.isNan())
	{
		LN temp;
		temp.is_nan_ = true;
		return temp;
	}
	if (a.sign_ != b.sign_)
	{
		LN result = b - mod(a, b);
		result.sign_ = 1;
		return result;
	}
	else
	{
		LN result;
		result = mod(a, b);
		result.sign_ = 1;
		return result;
	}
}

LN &operator%=(LN &a, const LN &b)
{
	a = a % b;
	return a;
}

bool operator==(const LN &a, const LN &b)
{
	if (a.isNan() || b.isNan())
	{
		return false;
	}
	if (a.sign_ != b.sign_)
	{
		return false;
	}
	return is_equal(a, b);
}

bool operator!=(const LN &a, const LN &b)
{
	if (a.isNan() || b.isNan())
	{
		return true;
	}
	return !(a == b);
}

bool operator>(const LN &a, const LN &b)
{
	if (a.isNan() || b.isNan())
	{
		return false;
	}
	if (a.sign_ != b.sign_)
	{
		return a.sign_ > b.sign_;
	}
	if (a.sign_ == -1)
	{
		return smaller_than(a, b);
	}
	return greater_than(a, b);
}

bool operator>=(const LN &a, const LN &b)
{
	return a > b || a == b;
}

bool operator<(const LN &a, const LN &b)
{
	if (a.isNan() || b.isNan())
	{
		return false;
	}
	if (a.sign_ != b.sign_)
	{
		return a.sign_ < b.sign_;
	}
	if (a.sign_ == -1)
	{
		return greater_than(a, b);
	}
	return smaller_than(a, b);
}

bool operator<=(const LN &a, const LN &b)
{
	return a < b || a == b;
}

LN plus(const LN &a, const LN &b)
{
	LN result;
	auto size = max(a.size_, b.size_);
	result.size_ = size + 1;
	int carry = 0;
	for (int i = 0; i < size || carry; ++i)
	{
		result.digits_[i] = (a.size_ > i ? a.digits_[i] : 0) + (b.size_ > i ? b.digits_[i] : 0) + carry;
		if (result.digits_[i] >= NUMERAL_SYSTEM)
		{
			result.digits_[i] -= NUMERAL_SYSTEM;
			carry = 1;
		}
		else
		{
			carry = 0;
		}
	}
	result.trim();
	return result;
}

LN minus(const LN &a, const LN &b)
{
	LN result = a;
	int carry = 0;
	for (int i = 0; i < result.size_; ++i)
	{
		int temp = result.digits_[i] - b.digits_[i] - carry;
		if (temp < 0)
		{
			temp += NUMERAL_SYSTEM;
			result.digits_[i + 1]--;
		}
		result.digits_[i] = temp;
	}
	result.trim();
	return result;
}

LN composition(const LN &a, const LN &b)
{
	LN result;
	result.size_ = a.size_ + b.size_ + 1;
	for (int i = 0; i < a.size_; ++i)
	{
		int carry = 0;
		for (int j = 0; j < b.size_ || carry; ++j)
		{
			result.digits_[i + j] += a.digits_[i] * (j < b.size_ ? b.digits_[j] : 0) + carry;
			carry = result.digits_[i + j] / NUMERAL_SYSTEM;
			result.digits_[i + j] -= carry * NUMERAL_SYSTEM;
		}
	}
	result.trim();
	return result;
}

LN division(const LN &a, const LN &b)
{
	LN result;
	result.size_ = max(a.size_, b.size_);
	LN currentValue;
	currentValue.size_ = max(a.size_, b.size_) + 1;
	for (int i = a.size_ - 1; i >= 0; --i)
	{
		currentValue.shift();
		currentValue.digits_[0] = a.digits_[i];
		int x = 0;
		int l = 0;
		int r = NUMERAL_SYSTEM;
		while (l <= r)
		{
			int m = (l + r) >> 1;
			auto current = composition(b, m);
			currentValue.size_ = max(a.size_, b.size_) + 1;
			if (current.abs() <= currentValue.abs())
			{
				x = m;
				l = m + 1;
			}
			else
			{
				r = m - 1;
			}
		}
		result.digits_[i] = x;
		currentValue = minus(currentValue, b * x);
	}
	result.trim();
	return result;
}

LN mod(const LN &a, const LN &b)
{
	LN div = a / b;
	LN temp = a - div * b;
	return temp;
}

bool greater_than(const LN &a, const LN &b)
{
	if (a.size_ != b.size_)
	{
		return a.size_ > b.size_;
	}
	for (int i = a.size_ - 1; i >= 0; --i)
	{
		if (a.digits_[i] != b.digits_[i])
		{
			return a.digits_[i] > b.digits_[i];
		}
	}
	return false;
}

bool smaller_than(const LN &a, const LN &b)
{
	if (a.size_ != b.size_)
	{
		return a.size_ < b.size_;
	}
	for (int i = a.size_ - 1; i >= 0; --i)
	{
		if (a.digits_[i] != b.digits_[i])
		{
			return a.digits_[i] < b.digits_[i];
		}
	}
	return false;
}

bool is_equal(const LN &a, const LN &b)
{
	if (a.size_ != b.size_)
	{
		return false;
	}
	for (int i = 0; i < a.size_; ++i)
	{
		if (a.digits_[i] != b.digits_[i])
		{
			return false;
		}
	}
	return true;
}

LN LN::abs() const
{
	LN temp = *this;
	temp.trim();
	temp.sign_ = 1;
	return temp;
}

// todo resize
void LN::resize(int size)
{
	if (size < CAPACITY)
	{
	}
}

void LN::shift()
{
	for (int i = size_; i >= 1; --i)
	{
		digits_[i] = digits_[i - 1];
	}
	if (digits_[size_])
	{
		++size_;
	}
}

void LN::trim()
{
	int pos = size_;
	while (pos >= 0 && !digits_[pos])
	{
		pos--;
	}
	size_ = pos + 1;
}

const uint32_t *LN::getRef()
{
	return digits_;
}

uint32_t LN::getSize()
{
	return size_;
}

bool LN::isNegative() const
{
	return sign_ == -1;
}

bool LN::isNan() const
{
	return is_nan_;
}

bool LN::isZero() const
{
	return size_ == 0 || (size_ == 1 && digits_[0] == 0);
}
