/*****************************************************
* Copyright 2016 Bumjin Im, Patrick Granahan         *
*                                                    *
* COMP 421/521 Lab 3 (Yalnix yfs) test code          *
*    symlink_readlink.c                              *
*                                                    *
* Author                                             *
*  Bumjin Im (imbumjin@rice.edu)                     *
*  Patrick  (pjgranahan@rice.edu)                    *
*                                                    *
* Created Apr. 22. 2016.                             *
*****************************************************/

#include <comp421/yalnix.h>
#include <comp421/iolib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define assert(message, test) do { if (!(test)) { Shutdown(); printf(message); return 1; } } while (0)

int main()
{
	int fd, ret;
	char buf[MAXPATHNAMELEN];

	// Creating targets
	fd = Create("/file1");
	assert("Failed Create(1)\n", fd == 0);
	ret = Write(fd, "ABCDEFGHIJ", 10);
	assert("Failed Write(1)\n", ret == 10);
	Close(fd);
	ret = MkDir("/dir1");
	assert("Failed Mkdir(1)", ret == 0);

	fd = Create("/dir1/file2");
	assert("Failed Create(2)\n", fd == 0);
	ret = Write(fd, "0123456789", 10);
	assert("Failed Write(2)\n", ret == 10);
	Close(fd);
	ret = MkDir("/dir2");
	assert("Failed Mkdir(2)", ret == 0);

	fd = Create("/dir1/file3");
	assert("Failed Create(3)\n", fd == 0);
	ret = Write(fd, "abcdefghijk", 10);
	assert("Failed Write(3)\n", ret == 10);
	Close(fd);
	ret = MkDir("/dir1/dir3");
	assert("Failed Mkdir(3)", ret == 0);

	// Simple Symlink
	fprintf(stderr, "Simple SymLink 1\n");
	ret = SymLink("/file1", "/dir2/symlink1");
	assert("Failed simple Link\n", ret == 0);

	// Another link
	fprintf(stderr, "Simple SymLink 2\n");
	ret = SymLink("/dir1/file2", "/symlink2");
	assert("Failed another simple Link\n", ret == 0);

	// Try to link a directory
	fprintf(stderr, "Linking directory\n");
	ret = SymLink("/dir1/dir3", "/symlink3");
	assert("Failed Link Dir\n", ret == 0);

	// Try to link an existing file
	fprintf(stderr, "Linking with existing file name\n");
	ret = SymLink("/file1", "/dir1/file3");
	assert("Failed Link an existing file\n", ret == ERROR);

	// Try to link unreachable dir
	fprintf(stderr, "Linking with nonexistent directory\n");
	ret = SymLink("/file1", "/dir3/file1");
	assert("Failed Link nonexistent directory\n", ret == ERROR);

	// Try to link in a same directory
	fprintf(stderr, "Linking with existing symlink\n");
	ret = SymLink("/dir1", "/symlink3");
	assert("Failed Link existing symlink\n", ret == ERROR);

	// Simple readlink
	fprintf(stderr, "Simple ReadLink\n");
	ret = ReadLink("/dir2/symlink1", buf, MAXPATHNAMELEN);
	assert("Failed simple ReadLink\n", ret == 6);
	buf[ret] = 0;
	fprintf(stderr, "Read Link: %s\n", buf);

	// Another readlink
	fprintf(stderr, "Another ReadLink\n");
	ret = ReadLink("/symlink3", buf, MAXPATHNAMELEN);
	assert("Failed another ReadLink\n", ret == 10);
	buf[ret] = 0;
	fprintf(stderr, "Read Link: %s\n", buf);

	// Open a symlink file
	fprintf(stderr, "Open a symlink file\n");
	fd = Open("/symlink2");
	assert("Failed Open a symlink\n", fd == 0);
	ret = Read(fd, buf, 10);
	Close(fd);
	buf[10] = 0;
	assert("Failed Read from a symlink\n", ret == 10);
	assert("Failed to read a symlink.\n", strncmp(buf, "0123456789", 10) == 0);

	printf("All tests passed\n");
	return Shutdown();
}
