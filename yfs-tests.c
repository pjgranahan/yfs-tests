#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

char yalnix[100] = "/clear/courses/comp421/pub/bin/yalnix -n -lu 5 -ly 5 -lk 0";
char yfs[50] = "../comp421/lab3/yfs";
char test_dir[50] = "../comp421/lab3/tests/";
char mkyfs_command[100] = "/clear/courses/comp421/pub/bin/mkyfs";

int num_tests = 0;
int tests_run = 0;

int RESULT_MESSAGE_SIZE = 1024;

struct tr {
	int index;
	int duration; // Test duration in milliseconds
	char message[1024];
};
typedef struct tr* test_result;

typedef void (*func_ptr)(char* result);

int mkdisk(int num_inodes);
int rmdisk();
//char* assert(char* message, int test);
test_result run_test(func_ptr test);
void print_test_result(test_result result);
test_result* all_tests();
void mkcommand(char* test_name, char* target);
void build_and_run_command(char* test_name, char* result);

/*
 * BEGIN TESTS
 */
void test_chdir(char* result) {
	mkdisk(0);
	build_and_run_command("chdir", result);
	rmdisk();
}

void test_create(char* result) {
	mkdisk(0);
	build_and_run_command("create", result);
	rmdisk();
}
/*
 * END TESTS
 */

void run_command(char* command, char* result) {
	printf("Running command: %s\n", command);
	fflush(stdout);

	FILE *fp;
	fp = popen(command, "r");
	if (fp == NULL) {
		printf("Failed to run command: %s\n", command);
		exit(1);
	}

	// Record and return the result if it's wanted
	if (result != NULL)
		fgets(result, RESULT_MESSAGE_SIZE, fp);
}

void build_and_run_command(char* test_name, char* result) {
	// Build the command
	char command[300];
	mkcommand(test_name, command);

	// Run the command
	run_command(command, result);
}

/*
 * Builds a command using the given test name and a target char array
 * example command: ./yalnix yfs test1
 */
void mkcommand(char* test_name, char* target) {
	strcpy(target, yalnix);
	strcat(target, " ");
	strcat(target, yfs);
	strcat(target, " ");
	strcat(target, test_dir);
	strcat(target, test_name);
}

/*
 * Makes a fresh disk
 */
int mkdisk(int num_inodes) {
	// Construct command
	if (num_inodes != 0) {
		strcat(mkyfs_command, " ");
		strcat(mkyfs_command, (char*)&num_inodes);
	}

	run_command(mkyfs_command, NULL);

	return 0;
}

/*
 * Removes a disk
 */
int rmdisk() {
	remove("DISK");

	return 0;
}

///*
// * Asserts that the test is true, and returns an appropriate message based on the result.
// */
//char* assert(char* message, int test) {
//	if (!(test)) {
//		return message;
//	} else {
//		return 0;
//	}
//}

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
	test(result->message);
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
void print_test_result(test_result result) {
	printf("test %d (%d ms) -\t\tresult:", result->index, result->duration);
	printf("\t%s\n", result->message);
}

/*
 * Runs all tests and returns an array of test_result objects.
 */
test_result* all_tests() {
	// Create an array of test functions
	printf("\tCreating array of tests...\n");
	func_ptr tests[1] = {
			test_create
//			test_chdir
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

int main() {

	printf("Running tests...\n");
	test_result* results = all_tests();
	printf("\nTEST RESULTS (%d tests)\n", num_tests);
	printf("---------------------------\n");

	int i;
	for (i = 0; i < num_tests; i++) {
		print_test_result(results[i]);
	}
	fflush(stdout);

	return 0;
}
