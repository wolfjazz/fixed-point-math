/** \file
 * Program entry point.
 */

#include <iostream>
using namespace std;

#include "fpm/q.hpp"


int main() {
    cout << "Hello fpm::Q!" << endl;
    cout << "s2s<-2,4>(4): " << fpm::s2s<-2,4>((int16_t)4) << endl;
    cout << "v2s<-5, int16_t, 5>(): " << fpm::v2s<int16_t, 5>(-5.) << endl;
    cout << "v2s<-5000, int16_t, 4>(): " << fpm::v2s<int16_t, -4>(-5000.) << endl;

    auto sqValue = fpm::sq<int16_t, 4, -56., 56.>(-2047.1);
    cout << "sqValue: " << (int)sqValue.Reveal() << "(" << (int)sqValue.Unwrap() << ")" << endl;
}


// EOF
