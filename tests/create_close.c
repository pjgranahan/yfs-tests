/*****************************************************
* Copyright 2016 Bumjin Im, Patrick Granahan         *
*                                                    *
* COMP 421/521 Lab 3 (Yalnix yfs) test code          *
*    create_close.c                                  *
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
	int fd;

	// Simple create
	fprintf(stderr, "Simple create\n");
	fd = Create("a");
	assert("Failed simple create\n", fd == 0);

	// Tests all close cases
	fprintf(stderr, "Simple open FD close\n");
	assert("Expected close on open FD to succeed\n", Close(fd) == 0);

	fprintf(stderr, "Simple closed FD close\n");
	assert("Expected close on closed FD to fail\n", Close(fd) == ERROR);

	// More simple create cases
	fprintf(stderr, "Simple create\n");
	fd = Create("a");
	assert("Failed simple create a\n", fd == 0);

	fprintf(stderr, "Simple open FD close\n");
	assert("Expected close on open FD to succeed\n", Close(fd) == 0);

	fprintf(stderr, "Simple create\n");
	fd = Create("b");
	assert("Failed simple create b\n", fd == 0);

	fprintf(stderr, "Simple open FD close\n");
	assert("Expected close on open FD to succeed\n", Close(fd) == 0);

	fprintf(stderr, "Simple create\n");
	fd = Create("01234567890123456789");
	assert("Failed simple create 01234567890123456789\n", fd == 0);

	fprintf(stderr, "Simple open FD close\n");
	assert("Expected close on open FD to succeed\n", Close(fd) == 0);

	fprintf(stderr, "Simple create, but pathname too long\n");
	fd = Create("0123456789012345678901234567890");
	assert("Failed simple create 0123456789012345678901234567890\n", fd == ERROR);

	// Creates on tricky pathnames
	fprintf(stderr, "Tricky pathname create\n");
	fd = Create("");
	assert("Failed tricky pathname create \"\"\n", fd == ERROR);

	fprintf(stderr, "Tricky pathname create\n");
	fd = Create("/");
	assert("Failed tricky pathname create /\n", fd == ERROR);

	fprintf(stderr, "Tricky pathname create\n");
	fd = Create(".");
	assert("Failed tricky pathname create .\n", fd == ERROR);

	fprintf(stderr, "Tricky pathname create\n");
	fd = Create("..");
	assert("Failed tricky pathname create ..\n", fd == ERROR);

	fprintf(stderr, "Tricky pathname create\n");
	fd = Create(NULL);
	assert("Failed tricky pathname create NULL\n", fd == ERROR);

	// Test Create in conjunction with MkDir
	Unlink("/a");
	MkDir("/a");
	MkDir("/a/b");
	fprintf(stderr, "Create in conjunction with directories\n");
	fd = Create("/a/b/c");
	assert("Create in conjunction with directories\n", fd == 0);

	fprintf(stderr, "Simple open FD close\n");
	assert("Expected close on open FD to succeed\n", Close(fd) == 0);

	fprintf(stderr, "Create in conjunction with directories\n");
	fd = Create("/a/b/c/d");
	assert("Treated a created file as a directory\n", fd == ERROR);

	printf("All tests passed\n");
	return Shutdown();
}
