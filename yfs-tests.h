/*
 * yfs-tests.h
 *
 *  Created on: Apr 19, 2016
 *      Author: pjg2
 */

#ifndef YFS_TESTS_H_
#define YFS_TESTS_H_

#include <stdbool.h>

int num_tests = 0;
int tests_failed = 0;
int tests_run = 0;

struct tr {
	int index;
	int duration; // Test duration in milliseconds
	char* message; // 0 if test passed
};
typedef struct tr* test_result;

typedef char* (*func_ptr)();

char* assert(char* message, bool test);
test_result run_test(func_ptr test);
int print_test_result(test_result result);
test_result* all_tests();

#endif /* YFS_TESTS_H_ */
