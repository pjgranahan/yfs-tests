/*****************************************************
* Copyright 2016 Bumjin Im, Patrick Granahan         *
*                                                    *
* COMP 421/521 Lab 3 (Yalnix yfs) test code          *
*    chdir.c                                         *
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

/*
 * This test performs a series of MkDir() and ChDir() calls, and verifies that YFS
 * is performing them as expected.
 */
int main()
{
	int ret;

	// Simple MkDirs that are expected to succeed.
	fprintf(stderr, "Simple MkDirs that are expected to succeed\n");
	ret = MkDir("a");
	assert("Failed simple MkDir()\n", ret == 0);

	fprintf(stderr, "Simple MkDirs that are expected to succeed\n");
	ret = MkDir("b");
	assert("Failed simple MkDir()\n", ret == 0);

	fprintf(stderr, "Simple MkDirs that are expected to succeed\n");
	ret = MkDir("c");
	assert("Failed simple MkDir()\n", ret == 0);

	// Simple ChDir
	fprintf(stderr, "Simple ChDir\n");
	ret = ChDir("a");
	assert("Failed simple ChDir()\n", ret == 0);

	// More simple MkDirs and ChDirs
	fprintf(stderr, "More simple MkDirs and ChDirs\n");
	ret = MkDir("b");
	assert("Failed to make a directory in a directory\n", ret == 0);

	fprintf(stderr, "More simple MkDirs and ChDirs\n");
	ret = ChDir("b");
	assert("Failed to change to a directory once in a directory\n", ret == 0);

	// One more time with simple MkDirs and ChDirs
	fprintf(stderr, "One more time with simple MkDirs and ChDirs\n");
	ret = MkDir("c");
	assert("Failed to make a directory 3 deep\n", ret == 0);

	fprintf(stderr, "One more time with simple MkDirs and ChDirs\n");
	ret = ChDir("c");
	assert("Failed to change to a directory 3 deep\n", ret == 0);

	// Try ChDir up one dir
	fprintf(stderr, "Try ChDir up one dir\n");
	ret = ChDir("..");
	assert("Failed to ChDir(\"..\")\n", ret == 0);

	// Try ChDir same directory
	fprintf(stderr, "Try ChDir same directory\n");
	ret = ChDir(".");
	assert("Failed to ChDir(\".\")\n", ret == 0);

	// Try to remake the b directory, but we expect it already exists
	fprintf(stderr, "Try to remake the b directory, but we expect it already exists\n");
	ret = MkDir("c");
	assert("Expected error when pathname already exists\n", ret == ERROR);

	// Try an absolute ChDir
	fprintf(stderr, "Try an absolute ChDir\n");
	ret = ChDir("/c");
	assert("Failed simple absolute ChDir\n", ret == 0);

	// Try a root ChDir
	fprintf(stderr, "Try a root ChDir\n");
	ret = ChDir("/");
	assert("Failed simple root ChDir\n", ret == 0);

	// Try a complex ChDir to existing directory
	fprintf(stderr, "Try a complex ChDir to existing directory\n");
	ret = ChDir("/a/b/c");
	assert("Failed complex ChDir to existing directory\n", ret == 0);

	// Try a complex ChDir to a non-existent directory
	fprintf(stderr, "Try a complex ChDir to a non-existent directory\n");
	ret = ChDir("/a/b/c/e");
	assert("Failed complex ChDir to non-existent directory\n", ret == ERROR);

	// Try an empty pathname ChDir
	fprintf(stderr, "Try an empty pathname ChDir\n");
	ret = ChDir(NULL);
	assert("Failed empty pathname ChDir\n", ret == ERROR);

	fprintf(stderr, "Try an empty pathname ChDir\n");
	ret = ChDir("");
	assert("Failed empty pathname ChDir\n", ret == ERROR);

	// Try a complex ChDir to existing directory with dot at end
	fprintf(stderr, "Try a complex ChDir to existing directory with dot at end\n");
	ret = ChDir("/a/b/c/.");
	assert("Failed complex ChDir to existing directory with dot at end\n", ret == 0);

	// Try a complex ChDir to existing relative directory
	fprintf(stderr, "Try a complex ChDir to existing relative directory\n");
	ret = ChDir("../../b/c/../../b/../../.");
	assert("Failed complex ChDir to existing relative directory\n", ret == 0);

	printf("All tests passed\n");
	return Shutdown();
}
