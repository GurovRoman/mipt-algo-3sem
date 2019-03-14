#include <iostream>
#include <vector>
#include <string>
#include <sstream>

class BigInteger {

private:

    std::vector<long long> digits_;
    bool negative_;
    static const char base_len_ = 3;
    static const long base_ = 1e+3;

    bool isNegative() const;
    bool isZero() const;

    void normalize();
    void add(const BigInteger &a, bool substract = 0, bool invert = 0);
    void divide(const BigInteger &divisor, bool modulo = 0);
    BigInteger &karatsubaMultiply(const BigInteger &other);
    template<typename InputIterator>
    BigInteger(InputIterator first, InputIterator last);

public:

    BigInteger();

    ~BigInteger() = default;
    BigInteger(const BigInteger &other) = default;
    BigInteger(BigInteger &&other) = default;
    explicit BigInteger(const std::string &string);

    BigInteger(long long value);
    BigInteger &operator=(const BigInteger &other) = default;

    BigInteger &operator=(BigInteger &&other) = default;
    std::string toString() const;

    size_t getSize() const;

    char getBaseLength() const;
    long long int getDigit(size_t pos) const;
    const BigInteger operator-() const;

    BigInteger &operator++();

    BigInteger &operator--();
    const BigInteger operator++(int);

    const BigInteger operator--(int);
    BigInteger &operator+=(const BigInteger &a);

    BigInteger &naiveMultiply(const BigInteger &other);
    BigInteger &operator-=(const BigInteger &a);
    BigInteger &operator*=(const BigInteger &a);
    BigInteger &operator/=(const BigInteger &a);
    BigInteger &operator%=(const BigInteger &a);

    int compare(const BigInteger &other) const;

    explicit operator bool() const;

    friend std::istream &operator>>(std::istream &is, BigInteger &a);

};

std::ostream &operator<<(std::ostream &os, const BigInteger &a);

const BigInteger operator+(const BigInteger &a, const BigInteger &b);
const BigInteger operator-(const BigInteger &a, const BigInteger &b);
const BigInteger operator*(const BigInteger &a, const BigInteger &b);
const BigInteger operator/(const BigInteger &a, const BigInteger &b);
const BigInteger operator%(const BigInteger &a, const BigInteger &b);

bool operator>(const BigInteger &a, const BigInteger &b);
bool operator<(const BigInteger &a, const BigInteger &b);
bool operator>=(const BigInteger &a, const BigInteger &b);
bool operator<=(const BigInteger &a, const BigInteger &b);
bool operator==(const BigInteger &a, const BigInteger &b);
bool operator!=(const BigInteger &a, const BigInteger &b);


BigInteger::BigInteger() : digits_({0}), negative_(0) {}

BigInteger::BigInteger(const long long int value) : digits_({value}), negative_(value < 0) {
    if (digits_[0] < 0) {
        digits_[0] *= -1;
    }
    for (size_t i = 0; digits_[i] >= base_; ++i) {
        digits_.push_back(digits_[i] / base_);
        digits_[i] %= base_;
    }
}

BigInteger::BigInteger(const std::string &string) {
    negative_ = (string[0] == '-');

    long long digit = 0, multiplier = 1;
    for (auto it = string.rbegin(); it != string.rend() - negative_; ++it) {
        digit += multiplier * (*it - '0');
        multiplier *= 10;
        if (multiplier >= base_) {
            digits_.push_back(digit);
            digit = 0;
            multiplier = 1;
        }
    }
    if (digit || !getSize())
        digits_.push_back(digit);

    normalize();
}

size_t BigInteger::getSize() const {
    return digits_.size();
}

char BigInteger::getBaseLength() const {
    return base_len_;
}

long long int BigInteger::getDigit(size_t pos) const {
    return pos < digits_.size() ? digits_[pos] : 0;
}

bool BigInteger::isNegative() const {
    return negative_;
}

bool BigInteger::isZero() const {
    return getSize() == 1 && digits_[0] == 0;
}


void BigInteger::normalize() {
    while (getSize() > 1 && digits_[getSize() - 1] == 0)
        digits_.pop_back();
    if (getSize() == 1 && digits_[0] == 0) {
        negative_ = 0;
    }
}

void BigInteger::add(const BigInteger &a, const bool substract, const bool invert) {
    for (size_t i = 0; i < a.getSize(); ++i) {
        if (i == getSize())
            digits_.push_back(0);
        digits_[i] += a.digits_[i] * (2 * !substract - 1);
        digits_[i] *= (2 * !invert - 1);
    }

    for (size_t i = 0; i < getSize(); ++i) {
        if (digits_[i] < 0) {
            --digits_[i + 1];
            digits_[i] += base_;
        }
        else if (digits_[i] >= base_) {
            if (i + 1 == getSize())
                digits_.push_back(0);
            ++digits_[i + 1];
            digits_[i] -= base_;
        }
    }

    negative_ = negative_ ^ invert;

    normalize();
}

const BigInteger BigInteger::operator-() const {
    BigInteger a = *this;
    a.negative_ = !isZero() && !negative_;
    return a;
}

BigInteger &BigInteger::operator++() {
    *this += 1;
    return *this;
}

BigInteger &BigInteger::operator--() {
    *this -= 1;
    return *this;
}

const BigInteger BigInteger::operator++(int) {
    BigInteger a(*this);
    *this += 1;
    return a;
}

const BigInteger BigInteger::operator--(int) {
    BigInteger a(*this);
    *this -= 1;
    return a;
}

BigInteger &BigInteger::operator+=(const BigInteger &a) {
    if (negative_ == a.negative_)
        add(a, 0, 0);
    else if ((*this >= -a) != negative_)
        add(a, 1, 0);
    else
        add(a, 1, 1);
    return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &a) {
    return *this += (-a);
}

BigInteger &BigInteger::operator*=(const BigInteger &a) {
    return karatsubaMultiply(a);
}

BigInteger &BigInteger::operator/=(const BigInteger &a) {
    if (a.isNegative())
        divide(-a);
    else
        divide(a);
    negative_ = !isZero() && (negative_ != a.negative_);
    return *this;
}

BigInteger &BigInteger::operator%=(const BigInteger &a) {
    if (a.isNegative())
        divide(-a, 1);
    else
        divide(a, 1);
    return *this;
}

void BigInteger::divide(const BigInteger &divisor, bool modulo) {
    BigInteger quotient, remainder;

    for (size_t i = digits_.size() - 1; i <= digits_.size() - 1; --i) {
        remainder.digits_.insert(remainder.digits_.begin(), digits_[i]);
        remainder.normalize();

        quotient.digits_.insert(quotient.digits_.begin(), 0);
        quotient.normalize();

        while (divisor <= remainder) {
            if (divisor.getSize() == 1) {
                long long count = (remainder.getSize() != 1 ? base_ : remainder.getDigit(0)) / divisor.getDigit(0);
                remainder -= count * divisor.getDigit(0);
                quotient += count;
            }
            else {
                ++quotient;
                remainder -= divisor;
            }
        }
    }

    if (!modulo)
        digits_ = quotient.digits_;
    else
        digits_ = remainder.digits_;

    normalize();
}

std::ostream &operator<<(std::ostream &os, const BigInteger &a) {
    if (a < 0)
        os << '-';
    os.fill('0');
    for (size_t i = a.getSize(); i > 0; --i) {
        if (i != a.getSize())
            os.width(a.getBaseLength());
        os << a.getDigit(i - 1);
    }
    return os;
}

std::istream &operator>>(std::istream &is, BigInteger &a) {
    std::string input;
    is >> input;
    a = BigInteger(input);
    return is;
}

const BigInteger operator+(const BigInteger &a, const BigInteger &b) {
    BigInteger c = a;
    c += b;
    return c;
}

const BigInteger operator-(const BigInteger &a, const BigInteger &b) {
    BigInteger c = a;
    c -= b;
    return c;
}

const BigInteger operator*(const BigInteger &a, const BigInteger &b) {
    BigInteger c = a;
    c *= b;
    return c;
}

const BigInteger operator/(const BigInteger &a, const BigInteger &b) {
    BigInteger c = a;
    c /= b;
    return c;
}

const BigInteger operator%(const BigInteger &a, const BigInteger &b) {
    BigInteger c = a;
    c %= b;
    return c;
}

int BigInteger::compare(const BigInteger &other) const {
    if (negative_ != other.negative_)
        return other.negative_ ? 1 : -1;

    bool invert = negative_;

    if (getSize() != other.getSize())
        return (getSize() > other.getSize()) != invert ? 1 : -1;

    for (size_t i = getSize() - 1; i < getSize(); --i)
        if (digits_[i] != other.digits_[i])
            return (digits_[i] > other.digits_[i]) != invert ? 1 : -1;

    return 0;
}

bool operator>(const BigInteger &a, const BigInteger &b) {
    return a.compare(b) == 1;
}

bool operator<(const BigInteger &a, const BigInteger &b) {
    return a.compare(b) == -1;
}

bool operator>=(const BigInteger &a, const BigInteger &b) {
    return a.compare(b) >= 0;
}

bool operator<=(const BigInteger &a, const BigInteger &b) {
    return a.compare(b) <= 0;
}

bool operator==(const BigInteger &a, const BigInteger &b) {
    return a.compare(b) == 0;
}

bool operator!=(const BigInteger &a, const BigInteger &b) {
    return a.compare(b) != 0;
}

BigInteger::operator bool() const {
    return *this != 0;
}

std::string BigInteger::toString() const {
    std::ostringstream output;
    output << *this;
    return output.str();
}

template<typename InputIterator>
BigInteger::BigInteger(InputIterator first, InputIterator last) : digits_(first, last), negative_(0) {
    normalize();
}

BigInteger &BigInteger::karatsubaMultiply(const BigInteger &other) {
    size_t n = std::max(getSize(), other.getSize());
    if (n <= 32) {
        return naiveMultiply(other);
    }

    size_t k = n / 2;

    BigInteger this_low = BigInteger(digits_.begin(),
                                digits_.begin() + std::min(k, digits_.size()));
    BigInteger this_high =
            k < digits_.size()
            ? BigInteger(digits_.begin() + k, digits_.end())
            : BigInteger(0);
    BigInteger other_low = BigInteger(other.digits_.begin(),
                                other.digits_.begin() + std::min(k, other.digits_.size()));
    BigInteger other_high =
            k < other.digits_.size()
            ? BigInteger(other.digits_.begin() + k, other.digits_.end())
            : BigInteger(0);

    BigInteger prod1(this_high);
    prod1.karatsubaMultiply(other_high);

    BigInteger prod2(this_low);
    prod2.karatsubaMultiply(other_low);

    BigInteger &prod3 = (this_high += this_low).karatsubaMultiply(other_high += other_low);

    prod3 -= prod1;
    prod3 -= prod2;
    prod1.digits_.insert(prod1.digits_.begin(), (n / 2) * 2, 0);
    prod3.digits_.insert(prod3.digits_.begin(), n / 2, 0);
    prod1 += prod2;
    prod1 += prod3;

    prod1.negative_ = negative_ ^ other.negative_;

    *this = prod1;
    return *this;
}

BigInteger &BigInteger::naiveMultiply(const BigInteger &other) {
    BigInteger c;
    for (size_t i = 0; i < getSize(); ++i) {
        long long carry = 0;
        for (size_t j = 0; j < other.getSize() || carry; ++j) {
            if (c.getSize() <= i + j)
                c.digits_.push_back(0);

            long long temp = c.digits_[i + j] + digits_[i] * other.getDigit(j) + carry;

            c.digits_[i + j] = temp % base_;
            carry = temp / base_;
        }
    }

    c.normalize();

    digits_ = c.digits_;
    negative_ = !isZero() && (negative_ != other.negative_);

    return *this;
}
