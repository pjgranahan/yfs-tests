#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "yfs-tests.h"

char mkyfs[50] = "/clear/courses/comp421/pub/bin/mkyfs";
char yalnix[50] = "/clear/courses/comp421/pub/bin/yalnix -n";
char yfs[50] = "../comp421/lab3/yfs";

/*
 * BEGIN TESTS
 */
char* test_chdir() {
	printf("Testing chdir\n");
	fflush(stdout);
	FILE *fp;

//	system(mkyfs);
	fp = popen(mkyfs, "r");
	if (fp == NULL) {
		printf("Failed to run mkyfs\n");
		exit(1);
	}

//	create_default_disk();
	printf("made disk\n");
	fflush(stdout);

	/* Open the command for reading. */

	char test_command[200];
	strcpy(test_command, yalnix);
	strcat(test_command, " ");
	strcat(test_command, yfs);
	strcat(test_command, " ");
	strcat(test_command, "../comp421/lab3/tests/unit_tests/chdir");
	printf("test command: %s\n", test_command);
	fflush(stdout);
	fp = popen(test_command, "r");
	if (fp == NULL) {
		printf("Failed to run test command\n");
		exit(1);
	}

	char result[1024];
	fgets(result, sizeof(result), fp);
	printf("finished result\n");
	fflush(stdout);

	remove("DISK");
	printf("removed disk\n");
	fflush(stdout);

	printf("Done testing chdir\n");
	return result;
}
/*
 * END TESTS
 */


/*
 * Asserts that the test is true, and returns an appropriate message based on the result.
 */
char* assert(char* message, bool test) {
	if (!(test)) {
		return message;
	} else {
		return 0;
	}
}

/*
 * Runs the provided function as a test.
 * Records the message returned by the test, and how long the test took to run, in a test_result object.
 */
test_result run_test(func_ptr test) {
	test_result result = malloc(sizeof(test_result));
	if (result == NULL) {
		printf("Failed to malloc a test result on test run %d", tests_run);
		exit(-1);
	}

	// Time the test - timing code taken from http://stackoverflow.com/a/459704/3394807
	clock_t start = clock();
	result->message = test(); // record the test message
	clock_t stop = clock();
	clock_t duration = stop - start;
	result->duration = duration * 1000 / CLOCKS_PER_SEC; //record the test duration

	// Increment the number of tests we have run
	result->index = tests_run;
	tests_run++;

	// Return the test_result object
	return result;
}

/*
 * Prints a test_result object.
 */
int print_test_result(test_result result) {
	printf("test %d (%d ms) -\t\tresult:", result->index, result->duration);
	if (result->message != 0) {
		printf("\t%s\n", result->message);
		return -1;
	} else {
		printf("\tpass\n");
		return 0;
	}
}

/*
 * Runs all tests and returns an array of test_result objects.
 */
test_result* all_tests() {
	// Create an array of test functions
	printf("\tCreating array of tests...\n");
	func_ptr tests[1] = {
			test_chdir
	};

	// Set the number of tests that will be run
	num_tests = sizeof(tests)/sizeof(tests[0]);

	// Create an array of test results
	printf("\tCreating array of test results...\n");
	test_result* results = malloc(sizeof(test_result) * num_tests);
	if (results == NULL) {
		printf("Failed to malloc results array");
		exit(-1);
	}

	// Run every test and store the results
	int i;
	for (i = 0; i < num_tests; i++) {
		printf("\t\tRunning test %d\n", i);
		fflush(stdout);
		results[i] = run_test(tests[i]);
	}

	printf("\tDone running tests...\n");

	// Return the results
	return results;
}

int main(int argc, char **argv) {

	printf("Running tests...\n");
	test_result* results = all_tests();
	printf("\nTEST RESULTS\n");
	printf("---------------------------\n");

	int i;
	for (i = 0; i < num_tests; i++) {
		tests_failed += print_test_result(results[i]);
		free(results[i]);
	}
	free(results);

	if (tests_failed == 0) {
		printf("ALL TESTS PASSED\n");
	}

	return tests_failed;
}
