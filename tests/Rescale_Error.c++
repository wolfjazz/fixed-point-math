#include <iostream>
using namespace std;



template< typename BASE_T, int TQ >
class MyType {
public:
    using base_t = BASE_T;
    static constexpr int Q = TQ;

    MyType(BASE_T value) : value(value*Q) {}
    ~MyType() {}
    BASE_T GetScaled() const { return value; }
    BASE_T GetValue() const { return value / Q; }

    template< class FROM, typename BASE_TF = typename FROM::base_t, int QF = FROM::Q >
    MyType(FROM const &from) {
        this->value = from.GetScaled() * Q / QF + (FROM::DoSomething<3>(FROM::Q));
    }

    template< int toQ >
    static constexpr BASE_T DoSomething(BASE_T v) {
        return v * Q * toQ;
    }

private:
    BASE_T value;
};

template< class MT1, typename BASE_T1 = typename MT1::base_t, int Q1 = MT1::Q,
          class MT2, typename BASE_T2 = typename MT2::base_t, int Q2 = MT2::Q,
          typename BASE_TR = BASE_T1, int QR = (Q1 >= Q2 ? Q1 : Q2),
          typename MTR = MyType<BASE_TR, QR> >
MTR operator+(MT1 const &lhs, MT2 const &rhs) {
    return MTR( lhs.GetValue() + rhs.GetValue() );
}

using Mti5 = MyType<int, 5>;
using Mti4 = MyType<int, 4>;
using Mti999 = MyType<int, 999>;
using Mti100 = MyType<int, 100>;



int main() {
    Mti5 a = Mti5(3);
    Mti4 b = a;
    Mti999 c = b;
    Mti100 d = a + c + b;

    cout << "a: " << a.GetValue() << " (" << a.GetScaled() << ")" << endl;
    cout << "b: " << b.GetValue() << " (" << b.GetScaled() << ")" << endl;
    cout << "c: " << c.GetValue() << " (" << c.GetScaled() << ")" << endl;
    cout << "d: " << d.GetValue() << " (" << d.GetScaled() << ")" << endl;
    cout << "sth: " << Mti5::DoSomething<6>(4) << endl;

    return 0;
}

