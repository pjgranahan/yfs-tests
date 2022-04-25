/*****************************************************
* Copyright 2016 Bumjin Im, Patrick Granahan         *
*                                                    *
* COMP 421/521 Lab 3 (Yalnix yfs) test code          *
*    open.c                                          *
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
	int fd, fd2;

	// Simple open tests
	fprintf(stderr, "Simple open tests\n");
	fd = Create("a");
	assert("Failed simple open test a\n", fd == 0);
	Close(fd);

	fd = Open("a");
	assert("Failed simple open test a2\n", fd == 0);
	Close(fd);

	fd = Open("b");
	assert("Failed simple open test b\n", fd == ERROR);

	// Check that FDs increase as we open more files
	fprintf(stderr, "Check that FDs increase as we open more files\n");
	fd = Create("b");
	Close(fd);
	fd = Open("a");
	fd2 = Open("b");
	assert("Failed to increase FDs as we open more files\n", fd == 0 && fd2 == 1);
	Close(fd);
	Close(fd2);

	// Test open on and in directories
	fprintf(stderr, "Test open on and in directories\n");
	Unlink("/a");
	MkDir("/a");
	MkDir("/a/b");
	fd = Create("/a/b/c");
	assert("Failed directory open abc\n", fd == 0);
	Close(fd);

	fd = Open("/a/b/c");
	assert("Failed directory open abc2\n", fd == 0);
	Close(fd);

	fd = Open("/a/b");
	assert("Failed to open a directory ab\n", fd == 0);
	Close(fd);

	fd = Open("/a/b/d");
	assert("Tried to open a non-existent file\n", fd == ERROR);
	Close(fd);

	// Test tricky open edge cases
	fprintf(stderr, "Test tricky open edge cases\n");
	fd = Open("/");
	assert("Failed to open a directory /\n", fd == 0);
	Close(fd);

	fd = Open(".");
	assert("Failed to open a directory .\n", fd == 0);
	Close(fd);

	fd = Open("..");
	assert("Failed to open a directory ..\n", fd == 0);
	Close(fd);

	fd = Open(NULL);
	assert("Tried to open NULL pathname\n", fd == ERROR);

	fd = Open("");
	assert("Tried to open empty pathname\n", fd == ERROR);

	printf("All tests passed\n");
	return Shutdown();
}
