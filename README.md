# yfs-tests
Automated unit tests for Rice University's COMP 421 Lab 3 - Yalnix File System (YFS)

## Test Coverage
The following user process file system operations are tested, thought we make no claim to have 100% test coverage.
- Close
- Create
- Read
- Write
- Seek
- Link
- Unlink
- SymLink
- ReadLink
- MkDir
- RmDir
- ChDir
- Stat
- Sync
- Shutdown

## Usage
1. Move the *tests* directory into your YFS project directory.
2. Adjust your YFS Makefile to build the tests.
3. Build YFS and the tests using the YFS Makefile.
4. Adjust the parameters at the top of yfs-tests.c to match your environment.
5. Build yfs-tests using the yfs-tests Makefile.
6. Run yfs-tests (from a Unix environment, *not* a Yalnix environment).
7. Report bugs here and/or fix them and submit a pull request.

## License
Tests are copyright Bumjin Im and Patrick Granahan, 2016.
Everything else is copyright Patrick Granahan, 2016.
You're welcome to use this project while working on YFS.
