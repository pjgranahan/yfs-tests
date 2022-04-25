/*****************************************************
* Copyright 2016 Bumjin Im, Patrick Granahan         *
*                                                    *
* COMP 421/521 Lab 3 (Yalnix yfs) test code          *
*    read_write_seek_stat.c                          *
*                                                    *
* Author                                             *
*  Bumjin Im (imbumjin@rice.edu)                     *
*  Patrick  (pjgranahan@rice.edu)                    *
*                                                    *
* Created Apr. 16. 2016.                             *
*****************************************************/

#include <comp421/filesystem.h>
#include <comp421/iolib.h>
#include <comp421/yalnix.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define assert(message, test) do { if (!(test)) { Shutdown(); printf(message); return 1; } } while (0)
#define SIZE 8 * SECTORSIZE

int main()
{
	// Set up for the tests
	fprintf(stderr, "Set up for the read tests\n");

	int fd, ret, i, j;
	char buf[SIZE + 1], ref[SIZE], zero[SIZE], fff[SIZE];
	struct Stat mystat;

	// Fill buf with 'A' and ref with 'B'
	for(i = 0 ; i < SIZE ; i++) {
		buf[i] = 'A';
		ref[i] = 'B'; // used later
		zero[i] = '\0'; //used later
		fff[i] = 'F'; //used later
	}

	fd = Create("a");

	for(i = 0 ; i < 5 ; i++) {	// Indirect block write
		for(j = 0 ; j < SIZE ; j++)
			buf[j]++;
		ret = Write(fd, buf, SIZE);
		assert("Write position not advancing correctly in indirect block writes\n", ret == SIZE);
	}

	// Seeking to the end of those 5 blocks, skipping a block, and then writing another block
	ret = Seek(fd, 0, SEEK_END);
	assert("Seek end not working correctly\n", ret == SIZE * 5);

	ret = Seek(fd, SIZE, SEEK_CUR);
	assert("Seek not allowed to seek past end of file?\n", ret == SIZE * 6);

	ret = Write(fd, buf, SIZE);
	assert("Write position not advancing correctly after seeking past end of file\n", ret == SIZE);

	ret = Seek(fd, 0, SEEK_END);
	assert("Seek set not working correctly after skipping\n", ret == SIZE * 7);

	ret = Seek(fd, 0, SEEK_SET);
	assert("Seek set not working correctly\n", ret == 0);

	ret = Seek(fd, -1, SEEK_SET);
	assert("Seek set allowed to seek before beginning of file\n", ret == ERROR);

	ret = Seek(fd, -1, SEEK_CUR);
	assert("Seek cur allowed to seek before beginning of file\n", ret == ERROR);
	Close(fd);

	// Test stat of a
	fprintf(stderr, "Test stat of a\n");
	ret = Stat("a", &mystat);
	fprintf(stderr, "Stat a: [%d], inode=%d, type=%d, size=%d, nlink=%d\n", ret, mystat.inum, mystat.type, mystat.size, mystat.nlink);
	assert("Stats failed on a", ret == 0 && mystat.inum == 2 && mystat.type == INODE_REGULAR && mystat.size == SIZE * 7 && mystat.nlink == 1);

	// Begin read tests on a file
	fprintf(stderr, "Begin read tests on a file\n");
	fd = Open("a");
	ret = Read(fd, buf, 16);
	assert("Read wrong number of bytes: BBB\n", ret == 16);
	buf[16] = 0;
	assert("Failed read on file: BBB\n", strncmp(buf, "BBBBBBBBBBBBBBBB", 16) == 0);

	ret = Seek(fd, SIZE, SEEK_SET);
	ret = Read(fd, buf, 16);
	assert("Read wrong number of bytes: CCC\n", ret == 16);
	buf[16] = 0;
	assert("Failed read on file: CCC\n", strncmp(buf, "CCCCCCCCCCCCCCCC", 16) == 0);

	ret = Seek(fd, SIZE * 2, SEEK_SET);
	ret = Read(fd, buf, 16);
	assert("Read wrong number of bytes: DDD\n", ret == 16);
	buf[16] = 0;
	assert("Failed read on file: DDD\n", strncmp(buf, "DDDDDDDDDDDDDDDD", 16) == 0);

	ret = Seek(fd, 0, SEEK_SET);
	ret = Read(fd, buf, SIZE);
	assert("Read wrong number of bytes: BBB\n", ret == SIZE);
	assert("Failed read on file: ref\n", strncmp(buf, ref, SIZE) == 0);

	// Here we're attempting to read the '\0' gap block in the 6th block
	ret = Seek(fd, SIZE * -2, SEEK_END);
	assert("Seeking to wrong place in file: 000\n", ret == SIZE * 5);
	ret = Read(fd, buf, SIZE);
	assert("Read wrong number of bytes: 000\n", ret == SIZE);
	assert("Failed read on file: 000\n", strncmp(buf, zero, SIZE) == 0);

	// And then we attempt to read the FFF which should be in the 7th block, right past the gap block
	ret = Read(fd, buf, SIZE);
	assert("Read wrong number of bytes: FFF\n", ret == SIZE);
	assert("Failed read on file: FFF\n", strncmp(buf, fff, SIZE) == 0);

	ret = Seek(fd, 0, SEEK_END);
	ret = Read(fd, buf, SIZE);
	assert("Read wrong number of bytes: seek end\n", ret == 0);
	Close(fd);

	// Test Reading and writing on directories
	fprintf(stderr, "Test Reading and writing on directories\n");
	fd = Open(".");
	ret = Read(fd, buf, 16);
	assert("Read wrong number of bytes: . dir\n", ret == 16);
	ret = Write(fd, buf, 16);
	assert("Wrote to directory\n", ret == ERROR);
	Close(fd);

	fd = Open("/");
	ret = Read(fd, buf, 16);
	assert("Read wrong number of bytes: / dir\n", ret == 16);
	ret = Write(fd, buf, 16);
	assert("Wrote to directory\n", ret == ERROR);
	Close(fd);

	// Null tests
	fprintf(stderr, "null tests\n");
	fd = Open(NULL);
	assert("Opened null file\n", fd == ERROR);
	ret = Read(fd, buf, 16);
	assert("Read null file\n", ret == ERROR);
	ret = Write(fd, buf, 16);
	assert("Wrote null file\n", ret == ERROR);
	ret = Stat(NULL, &mystat);
	assert("Stats on null", ret == ERROR);
	ret = Stat("a", NULL);
	assert("Stats with null stat struct", ret == ERROR);

	// Test more stat things
	fprintf(stderr, "Test more stat things\n");
	fd = Create("b");
	for(j = 0 ; j < SIZE ; j++)
		buf[j]++;
	ret = Write(fd, buf, 16);
	Close(fd);
	ret = Stat("b", &mystat);
	assert("Stats failed on b", ret == 0 && mystat.inum == 3 && mystat.type == INODE_REGULAR && mystat.size == 16 && mystat.nlink == 1);

	ret = MkDir("c");
	ret = Stat("c", &mystat);
	assert("Stats failed on c", ret == 0 && mystat.inum == 4 && mystat.type == INODE_DIRECTORY && mystat.size == 64 && mystat.nlink == 2);

	ret = MkDir("c/d");
	ret = Stat("c/d", &mystat);
	assert("Stats failed on c/d", ret == 0 && mystat.inum == 5 && mystat.type == INODE_DIRECTORY && mystat.size == 64 && mystat.nlink == 2);

	ret = Stat("f", &mystat);
	assert("Stats failed on f", ret == ERROR);

	ret = Sync();
	assert("Sync failed", ret == 0);

	printf("All tests passed\n");
	return Shutdown();
}
