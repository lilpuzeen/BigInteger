//
// Created by aslan on 5/9/2023.
//

#ifndef INC_3_LAB_2_0_TEST_H
#define INC_3_LAB_2_0_TEST_H

#include <cstdlib>
#include <iostream>
#include <cassert>
#include "LN.h"

using std::cout;

std::ostream& operator<<(std::ostream& out,LN value);

std::string toString(LN value);

bool intTest()
{
    srand(time(nullptr));
    for(int i = 0; i < 1000; ++i){
        long long x = rand()%50000 - 100000;
        long long y = rand()%50000 - 100000;
        LN A(x),B(y);
        std::cout << A << " " << B << "\n";
        cout << x << " + " << y << " = " << A + B << "\n";
        cout << x << " - " << y << " = " << A - B << "\n";
        cout << x << " * " << y << " = " << A * B << "\n";
        cout << x << " / " << y << " = " << A / B << "\n";
        //cout << x << " % " << y << " = " << A % B << "\n";
        cout << x << " == " << y << " = " << (A == B) << "\n";
        cout << x << " < " << y << " = " << (A < B) << "\n";
        cout << x << " > " << y << " = " << (A > B) << "\n";
        cout << x << " <= " << y << " = " << (A <= B) << "\n";
        cout << x << " >= " << y << " = " << (A >= B) << "\n";
        //cout << x << " " << ~A << "\n";

        assert(toString(A + B) == std::to_string(x + y));

        assert(toString(A - B) == std::to_string(x - y));

        assert(toString(A * B) == std::to_string(x * y));

        assert(toString(A / B) == std::to_string(x / y));

        //assert((toString(A % B)) == to_string(x % y));

        assert((A == B) == (x == y));

        assert((A < B) == (x < y));

        assert((A > B) == (x > y));

        assert((A <= B) == (x <= y));

        assert((A >= B) == (x >= y));

        //assert((~A) == ((int)sqrt(x)));
    }
    return true;
}

#endif //INC_3_LAB_2_0_TEST_H
