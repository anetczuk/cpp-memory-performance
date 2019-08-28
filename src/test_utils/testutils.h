//
//
//

#ifndef SRC_TEST_UTILS_TESTUTILS_H_
#define SRC_TEST_UTILS_TESTUTILS_H_


inline bool compare_float(const float a, const float b, const float epsilon = 0.00001) {
    const float diff = std::abs(a - b);
    return (diff < epsilon);
}


#endif /* SRC_TEST_UTILS_TESTUTILS_H_ */
