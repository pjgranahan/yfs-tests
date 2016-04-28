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

#include <yfs.h>

#define assert(message, test) do { if (!(test)) { Shutdown(); printf(message); return 1; } } while (0)

int main()
{
	int fd, ret;
	char buf[MAXPATHNAMELEN];

	// Creating targets
	fd = Create("/file1");
	assert("Failed Create(1)\n", fd > 0);
	ret = Write(fd, "ABCDEFGHIJ", 10);
	assert("Failed Write(1)\n", ret == 10);
	Close(fd);
	fd = MkDir("/dir1");
	assert("Failed Mkdir(1)", fd == 0);
	fd = Create("/dir1/file2");
	assert("Failed Create(2)\n", fd > 0);
	ret = Write(fd, "0123456789", 10);
	assert("Failed Write92)\n", ret == 10);
	Close(fd);
	fd = MkDir("/dir2");
	assert("Failed Mkdir(2)", fd == 0);
	fd = Create("/dir1/file3");
	assert("Failed Create(3)\n", fd > 0);
	ret = Write(fd, "abcdefghijk", 10);
	assert("Failed Write(3)\n", ret == 10);
	Close(fd);
	fd = MkDir("/dir1/dir3");
	assert("Failed Mkdir(3)", fd == 0);

	// Simple Symlink
	fprintf(stderr, "Simple SymLink 1\n");
	fd = SymLink("/file1", "/dir2/symlink1");
	assert("Failed simple Link\n", fd == 0);

	// Another link
	fprintf(stderr, "Simple SymLink 2\n");
	fd = SymLink("/dir1/file2", "/symlink2");
	assert("Failed another simple Link\n", fd == 0);

	// Try to link a directory
	fprintf(stderr, "Linking directory\n");
	fd = SymLink("/dir1/dir3", "/symlink3");
	assert("Failed Link Dir\n", fd == 0);

	// Try to link an existing file
	fprintf(stderr, "Linking with existing file name\n");
	fd = SymLink("/file1", "/dir1/file3");
	assert("Failed Link an existing file\n", fd < 0);

	// Try to link unreachable dir
	fprintf(stderr, "Linking with existing file name\n");
	fd = SymLink("/file1", "/dir3/file1");
	assert("Failed Link not existing directory\n", fd < 0);

	// Try to link in a same directory
	fprintf(stderr, "oldname and new name are in the same directory\n");
	fd = SymLink("/dir1", "/symlink3");
	assert("Failed Link Dir\n", fd < 0);

	// Simple readlink
	fprintf(stderr, "Simple ReadLink\n");
	fd = ReadLink("/dir2/symlink1", buf, MAXPATHNAMELEN);
	assert("Failed simple ReadLink\n", fd > 0);
	buf[fd] = 0;
	fprintf(stderr, "Read Link: %s\n", buf);

	// Another readlink
	fprintf(stderr, "Another ReadLink\n");
	fd = ReadLink("/symlink3", buf, MAXPATHNAMELEN);
	assert("Failed another ReadLink\n", fd > 0);
	buf[fd] = 0;
	fprintf(stderr, "Read Link: %s\n", buf);

	// Open a symlink file
	fprintf(stderr, "Open a symlink file\n");
	fd = Open("/symlink2");
	assert("Failed Open a symlink\n", fd > 0);
	ret = Read(fd, buf, 10);
	Close(fd);
	buf[10] = 0;
	assert("Failed Read from a symlink\n", ret > 0);
	assert("Failed to read a symlink.\n", strncmp(buf, "0123456789", 10) == 0);

	printf("All tests passed. Syncing...\n");
	Sync();
	return Shutdown();
}
