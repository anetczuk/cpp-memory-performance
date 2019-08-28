//
//
//

#include <iostream>
#include "testutils.h"


using namespace std;


int main() {
    int failures = 0;

    {
        const float ret = 6.0;
        const float correct = 7.0;
        if (compare_float(correct, ret)==true) {
            ++failures;
        }
    }
    {
        const float ret = 6.0;
        const float correct = 7.0;
        if (compare_float(correct, ret, 1.00001)==false) {
            ++failures;
        }
    }

    return std::min( failures, 127 );
}
