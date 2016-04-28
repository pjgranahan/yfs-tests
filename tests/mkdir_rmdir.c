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

#include <comp421/yalnix.h>
#include <comp421/iolib.h>

#include <stdio.h>

#include <yfs.h>

#define assert(message, test) do { if (!(test)) { Shutdown(); printf(message); return 1; } } while (0)

int main()
{
	int fd;

	//Simplest mkdir tests
	fprintf(stderr, "Simplest mkdir tests\n");
	fd = MkDir("a");
	assert("Simple mkdir failed a", fd == 0);

	fd = MkDir("a");
	assert("Simple mkdir failed a2", fd == ERROR);

	fd = MkDir("b");
	assert("Simple mkdir failed b", fd == 0);

	fd = MkDir("01234567890123456789");
	assert("Simple mkdir failed 01234567890123456789", fd == 0);
	// Mkdir tests with directories involved
	fprintf(stderr, "Mkdir tests with directories involved\n");

	fd = MkDir("a/b");
	assert("Mkdir tests with directories involved failed ab", fd == 0);
	fd = MkDir("a/b");

	assert("Mkdir tests with directories involved failed: didn't error on directory remake", fd == ERROR);

	fd = MkDir("a/b/c");
	assert("Mkdir tests with directories involved failed abc", fd == 0);

	fd = MkDir("a/b/c/d");
	assert("Mkdir tests with directories involved failed abcd", fd == 0);

	fd = MkDir("/a/b/c/d/e");
	assert("Mkdir tests with directories involved failed abcde", fd == 0);

	fd = MkDir("/a/b/c/d/e/f");
	assert("Mkdir tests with directories involved failed abcdef", fd == 0);

	fd = MkDir("b/a/c");
	assert("Mkdir tests with directories involved failed: shouldn't create intermediate directories. see piazza @311", fd == ERROR);

	// Tricky edge cases
	fprintf(stderr, "Mkdir tricky edge case tests\n");
	fd = MkDir("0123456789012345678901234567891");
	assert("Expected pathname to be too long", fd == ERROR);

	fd = MkDir("");
	assert("Expected fail on empty directory", fd == ERROR);

	fd = MkDir("/");
	assert("Expected fail on making root dir", fd == ERROR);

	fd = MkDir(".");
	assert("Expected fail on mkdir(.)", fd == ERROR);

	fd = MkDir("..");
	assert("expected fail on mkdir(..)", fd == ERROR);

	// Moving on to RmDir tests
	fprintf(stderr, "Moving on to RmDir tests\n");
	fd = RmDir("b");
	assert("Failed simple RmDir", fd == 0);

	fd = RmDir("b");
	assert("Failed simple RmDir: tried to remove previously removed dir", fd == ERROR);

	fd = RmDir("a");
	assert("Tried to remove nonempty directory", fd == ERROR);

	// RmDir tests with directories
	fprintf(stderr, "RmDir tests with directories\n");
	fd = RmDir("a/b/c/d/e/f");
	assert("Failed simple RmDir with dirs: f", fd == 0);

	fd = RmDir("a/b/c/d/e");
	assert("Failed simple RmDir with dirs: e", fd == 0);

	fd = RmDir("a/b/c/d");
	assert("Failed simple RmDir with dirs: d", fd == 0);

	fd = RmDir("a/b/c");
	assert("Failed simple RmDir with dirs: c", fd == 0);

	fd = RmDir("a/b");
	assert("Failed simple RmDir with dirs: b", fd == 0);

	fd = RmDir("a");
	assert("Failed simple RmDir with dirs: a", fd == 0);

	fd = RmDir("c");
	assert("Tried to remove nonexistent dir", fd == ERROR);

	// RmDir tests with tricky names
	fprintf(stderr, "RmDir tests with tricky names\n");
	fd = RmDir("");
	assert("Tried to rmdir empty pathname", fd == ERROR);

	fd = RmDir(NULL);
	assert("Tried to rmdir NULL pathname", fd == ERROR);

	fd = RmDir("/");
	assert("Tried to remove root dir", fd == ERROR);

	fd = RmDir(".");
	assert("Tried to remove . dir", fd == ERROR);

	fd = RmDir("..");
	assert("Tried to remove .. dir", fd == ERROR);

	fd = RmDir("01234567890123456789");
	assert("Failed simple RmDir with numbers?", fd == 0);

	// TODO: Test RmDir on free entries

	printf("All tests passed\n");
	return Shutdown();
}
