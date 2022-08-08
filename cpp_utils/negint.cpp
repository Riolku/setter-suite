// Since many problems use -1 as "no answer" or infinity, and because -1 is such
// a common default value, this file provides helpers for -1 related functions

template <typename T> struct NegInt {
  T x;

  constexpr NegInt(T x) : x(x) {}

  // >0 if A > B
  // 0 if A = B
  // <0 if A < B
  T cmp(const NegInt<T> &other) const {
    if (x == -1) {
      return other.x == -1 ? 0 : 1;
    }
    if (other.x == -1) {
      return -1;
    }
    return x - other.x;
  }

  bool operator<(const NegInt<T> &other) const { return cmp(other) < 0; }
  bool operator>(const NegInt<T> &other) const { return cmp(other) > 0; }
  bool operator<=(const NegInt<T> &other) const { return cmp(other) <= 0; }
  bool operator>=(const NegInt<T> &other) const { return cmp(other) >= 0; }
  bool operator==(const NegInt<T> &other) const { return x == other.x; }
  bool operator!=(const NegInt<T> &other) const { return x != other.x; }

  void offer_min(const NegInt<T> &other) {
    if (other < *this) {
      x = other.x;
    }
  }

  NegInt<T> operator+(const NegInt<T> &other) const {
    if (x == -1 || other.x == -1)
      return -1;

    return NegInt<T>(x + other.x);
  }

  NegInt<T> &operator+=(const NegInt<T> &other) {
    if (x != -1) {
      if (other.x == -1)
        x = -1;
      else
        x += other.x;
    }
    return *this;
  }

  T get_printable() const { return x; }
};
