#include <iostream>
using namespace std;


template< int lower_, int upper_ >
class MyRange final {
public:
    constexpr static int lower = lower_;
    constexpr static int upper = upper_;

    template< int, int >
    friend class MyRange;

    constexpr MyRange(MyRange const &) noexcept = default;
    constexpr MyRange(MyRange&&) noexcept = default;
    constexpr ~MyRange() {}

    template< int value >
    requires ( value >= lower && value <= upper )
    static constexpr
    MyRange wrap = MyRange( value );

    template< class Rhs, int l = lower + Rhs::lower, int u = upper + Rhs::upper, typename R = MyRange<l,u> >
    friend
    R operator +(MyRange const lhs, Rhs const &rhs) noexcept {
        int result = lhs.value + rhs.unwrap();
        return construct<R>( result );
    }

    int unwrap() const noexcept { return value; }

private:
    MyRange() = delete;
    MyRange& operator =(MyRange const &) = delete;
    MyRange& operator =(MyRange&&) = delete;

    // this must not be exposed because value has to be checked against limits at compile-time;
    // wrap<value> is the public "constructor"
    explicit constexpr MyRange(int value) noexcept : value(value) {}

    // helper: construct another specialization of MyRange
    template< class TO >
    static constexpr TO construct(int value) noexcept { return TO(value); }

    int const value;
};


template< std::integral T >
consteval T charDigitTo(char c, std::size_t digit) {
    T result = c - '0';
    for (size_t i = 1; i < digit; ++i) {
        result *= 10;
    }
    return result;
}

template< std::integral T, std::size_t size, std::size_t digit = size >
requires ( size > 0 && digit <= size )
consteval T charArrayTo(const char chars[size]) {
    if constexpr (digit == 0) {
        return 0;
    }
    else {
        return charDigitTo<T>(chars[size - digit], digit) + charArrayTo<T, size, digit - 1>(chars);
    }
}

template< char ...charArray >
consteval auto operator ""_q() {
    constexpr std::size_t length = sizeof...(charArray);
    constexpr char chars[length]{ charArray... };
    static_assert(std::all_of(chars, chars + length, [](char c) { return isdigit(c); }), "The argument to _q must be a positive integer");
    constexpr auto value = charArrayTo<int, length>(chars);
    return MyRange<value, value>::template wrap<value>;
}


int main() {
    auto value = MyRange<5,20>::wrap<8>;

    auto result = value + MyRange<6,6>::wrap<6>;  // add 6, explicit casting
    auto result2 = value + 6_q;  // add 6, use literal operator ""_q to convert 6 to MyRange<6,6>::wrap<6>

    // 14; limits: 11, 26
    cout << result.unwrap() << "; limits: " << decltype(result)::lower << ", " << decltype(result)::upper << endl;
    cout << result2.unwrap() << "; limits: " << decltype(result2)::lower << ", " << decltype(result2)::upper << endl;
    return 0;
}
