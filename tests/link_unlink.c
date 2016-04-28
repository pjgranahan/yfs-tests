/*****************************************************
* Copyright 2016 Bumjin Im, Patrick Granahan         *
*                                                    *
* COMP 421/521 Lab 3 (Yalnix yfs) test code          *
*    link_unlink.c                                   *
*                                                    *
* Author                                             *
*  Bumjin Im (imbumjin@rice.edu)                     *
*  Patrick  (pjgranahan@rice.edu)                    *
*                                                    *
* Created Apr. 21. 2016.                             *
*****************************************************/

#include <comp421/yalnix.h>
#include <comp421/iolib.h>

#include <stdio.h>

#include <yfs.h>

#define assert(message, test) do { if (!(test)) { Shutdown(); printf(message); return 1; } } while (0)

int main()
{
	int fd, ret;

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

	// Simple link
	fprintf(stderr, "Simple Link 1\n");
	fd = Link("/file1", "/dir2/link1");
	assert("Failed simple Link\n", fd == 0);

	// Another link
	fprintf(stderr, "Simple Link 2\n");
	fd = Link("/dir1/file2", "/link2");
	assert("Failed another simple Link\n", fd == 0);

	// Try to link directory
	fprintf(stderr, "Linking directory\n");
	fd = Link("/dir1", "/link3");
	assert("Failed Link Dir\n", fd < 0);

	// Try to link an existing file
	fprintf(stderr, "Linking with existing file name\n");
	fd = Link("/file1", "/dir1/file3");
	assert("Failed Link an existing file\n", fd < 0);

	// Try to link unreachable dir
	fprintf(stderr, "Linking with existing file name\n");
	fd = Link("/file1", "/dir3/file1");
	assert("Failed Link not existing directory\n", fd < 0);

	// Simple unlink
	fprintf(stderr, "Simple Unlink\n");
	fd = Unlink("/dir2/link1");
	assert("Failed simple Unlink\n", fd == 0);

	// Try to remove a file
	fprintf(stderr, "Remove a file\n");
	fd = Unlink("/file1");
	assert("Failed simple Link\n", fd == 0);

	// Another unlink. This time, try to unlink original file and check the inode is not removed
	fprintf(stderr, "Unlink original file\n");
	fd = Unlink("/dir1/file2");
	assert("Failed Unlink original file\n", fd == 0);

	// Try to remove the file by unlinking the hard link
	fprintf(stderr, "Remove a file by unlinking a hard link\n");
	fd = Unlink("/link2");
	assert("Failed Remove a file by unlinking a hard link\n", fd == 0);

	// Try to unlink a directory
	fprintf(stderr, "Try to Unlink a directory\n");
	fd = Unlink("/dir1");
	assert("Failed Unlink a directory\n", fd < 0);

	printf("All tests passed. Syncing...\n");
	Sync();
	return Shutdown();
}
