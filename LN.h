#ifndef INC_3_LAB_2_0_LN_H
#define INC_3_LAB_2_0_LN_H

#ifndef INC_3_LAB_LN_H
#define INC_3_LAB_LN_H

#include <string_view>

#define NUMERAL_SYSTEM 10
#define CAPACITY 500

class LN
{
  public:
	LN(long long value = 0);

	LN(const char *str, size_t size);

	LN(std::string_view str);

	LN(const LN &other);

	LN(LN &other);

	LN(LN &&other);

	~LN();

	friend LN &operator-(LN &a);

	friend LN operator~(const LN &a);

	friend LN &operator+=(LN &a, const LN &b);

	friend LN operator+(const LN &a, const LN &b);

	friend LN &operator-=(LN &a, const LN &b);

	friend LN operator-(const LN &a, const LN &b);

	friend LN operator*(const LN &a, const LN &b);

	friend LN &operator*=(LN &a, const LN &b);

	friend LN operator/(const LN &a, const LN &b);

	friend LN &operator/=(LN &a, const LN &b);

	friend LN operator%(const LN &a, const LN &b);

	friend LN &operator%=(LN &a, const LN &b);

	friend bool operator==(const LN &, const LN &);

	friend bool operator!=(const LN &, const LN &);

	friend bool operator<(const LN &, const LN &);

	friend bool operator>(const LN &, const LN &);

	friend bool operator>=(const LN &, const LN &);

	friend bool operator<=(const LN &, const LN &);

	LN &operator=(const LN &other);

	LN &operator=(LN &&other) noexcept;

	bool isZero() const;

	bool isNan() const;

	bool isNegative() const;

	LN abs() const;

	const uint32_t *getRef();

	int getSign() { return sign_; };

	uint32_t getSize();

  private:
	uint32_t *digits_;

	uint32_t size_ = 0;
	int sign_ = 1;
	bool is_nan_ = false;
	void trim();

	void shift();

	void resize(int size);

	friend LN plus(const LN &a, const LN &b);

	friend LN minus(const LN &a, const LN &b);

	friend LN composition(const LN &a, const LN &b);

	friend LN division(const LN &a, const LN &b);

	friend LN mod(const LN &a, const LN &b);

	friend bool greater_than(const LN &a, const LN &b);

	friend bool smaller_than(const LN &a, const LN &b);

	friend bool is_equal(const LN &a, const LN &b);
};

LN operator""_ln(const char *str);

#endif	  // INC_3_LAB_LN_H

#endif	  // INC_3_LAB_2_0_LN_H
