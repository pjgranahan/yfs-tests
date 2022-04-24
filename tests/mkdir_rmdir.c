/*****************************************************
* Copyright 2016 Bumjin Im, Patrick Granahan         *
*                                                    *
* COMP 421/521 Lab 3 (Yalnix yfs) test code          *
*    mkdir_rmdir.c                                   *
*                                                    *
* Author                                             *
*  Bumjin Im (imbumjin@rice.edu)                     *
*  Patrick  (pjgranahan@rice.edu)                    *
*                                                    *
* Created Apr. 16. 2016.                             *
*****************************************************/

#include <comp421/iolib.h>
#include <comp421/yalnix.h>
#include <stdio.h>

#define assert(message, test) do { if (!(test)) { Shutdown(); printf(message); return 1; } } while (0)

int main()
{
	int ret;

	//Simplest mkdir tests
	fprintf(stderr, "Simplest mkdir tests\n");
	ret = MkDir("a");
	assert("Simple mkdir failed a", ret == 0);

	ret = MkDir("a");
	assert("Simple mkdir failed a2", ret == ERROR);

	ret = MkDir("b");
	assert("Simple mkdir failed b", ret == 0);

	ret = MkDir("01234567890123456789");
	assert("Simple mkdir failed 01234567890123456789", ret == 0);
	
	// Mkdir tests with directories involved
	fprintf(stderr, "Mkdir tests with directories involved\n");
	ret = MkDir("a/b");
	assert("Mkdir tests with directories involved failed ab", ret == 0);

	ret = MkDir("a/b");
	assert("Mkdir tests with directories involved failed: didn't error on directory remake", ret == ERROR);

	ret = MkDir("a/b/c");
	assert("Mkdir tests with directories involved failed abc", ret == 0);

	ret = MkDir("a/b/c/d");
	assert("Mkdir tests with directories involved failed abcd", ret == 0);

	ret = MkDir("/a/b/c/d/e");
	assert("Mkdir tests with directories involved failed abcde", ret == 0);

	ret = MkDir("/a/b/c/d/e/f");
	assert("Mkdir tests with directories involved failed abcdef", ret == 0);

	ret = MkDir("b/a/c");
	assert("Mkdir tests with directories involved failed: shouldn't create intermediate directories. see piazza @311", ret == ERROR);

	// Tricky edge cases
	fprintf(stderr, "Mkdir tricky edge case tests\n");
	ret = MkDir("0123456789012345678901234567890");
	assert("Expected pathname to be too long", ret == ERROR);

	ret = MkDir("");
	assert("Expected fail on empty directory", ret == ERROR);

	ret = MkDir("/");
	assert("Expected fail on making root dir", ret == ERROR);

	ret = MkDir(".");
	assert("Expected fail on mkdir(.)", ret == ERROR);

	ret = MkDir("..");
	assert("expected fail on mkdir(..)", ret == ERROR);

	// Moving on to RmDir tests
	fprintf(stderr, "Moving on to RmDir tests\n");
	ret = RmDir("b");
	assert("Failed simple RmDir", ret == 0);

	ret = RmDir("b");
	assert("Failed simple RmDir: tried to remove previously removed dir", ret == ERROR);

	ret = RmDir("a");
	assert("Tried to remove nonempty directory", ret == ERROR);

	// RmDir tests with directories
	fprintf(stderr, "RmDir tests with directories\n");
	ret = RmDir("a/b/c/d/e/f");
	assert("Failed simple RmDir with dirs: f", ret == 0);

	ret = RmDir("a/b/c/d/e");
	assert("Failed simple RmDir with dirs: e", ret == 0);

	ret = RmDir("a/b/c/d");
	assert("Failed simple RmDir with dirs: d", ret == 0);

	ret = RmDir("a/b/c");
	assert("Failed simple RmDir with dirs: c", ret == 0);

	ret = RmDir("a/b");
	assert("Failed simple RmDir with dirs: b", ret == 0);

	ret = RmDir("a");
	assert("Failed simple RmDir with dirs: a", ret == 0);

	ret = RmDir("c");
	assert("Tried to remove nonexistent dir", ret == ERROR);

	// RmDir tests with tricky names
	fprintf(stderr, "RmDir tests with tricky names\n");
	ret = RmDir("");
	assert("Tried to rmdir empty pathname", ret == ERROR);

	ret = RmDir(NULL);
	assert("Tried to rmdir NULL pathname", ret == ERROR);

	ret = RmDir("/");
	assert("Tried to remove root dir", ret == ERROR);

	ret = RmDir(".");
	assert("Tried to remove . dir", ret == ERROR);

	ret = RmDir("..");
	assert("Tried to remove .. dir", ret == ERROR);

	ret = RmDir("01234567890123456789");
	assert("Failed simple RmDir with numbers?", ret == 0);

	printf("All tests passed\n");
	return Shutdown();
}
