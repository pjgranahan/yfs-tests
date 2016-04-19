#################
# TODO: you just need to fill out these
#IOLIB = ../comp421/lab3/iolib.a
#YFS = ../comp421/lab3/yfs
#################

TEST_SRCS = $(wildcard tests/*.c)
TEST_TARGETS = $(patsubst %.c,%,$(TEST_SRCS))

ALL = yfs-tests $(TEST_TARGETS)

LANG = gcc
PUBLIC_DIR = /clear/courses/comp421/pub
CPPFLAGS = -I$(PUBLIC_DIR)/include -Iinclude -DDEBUG
CFLAGS = -g -Wall

%: %.o
	$(LINK.o) -o $@ $^ $(LOADLIBES) $(LDLIBS)

LINK.o = $(PUBLIC_DIR)/bin/link-user-$(LANG) $(LDFLAGS) $(TARGET_ARCH)

%: %.c
%: %.cc
%: %.cpp

all: $(ALL)

yfs-tests: yfs-tests.c $(TEST_SRCS)
	@echo "Building yfs-tests"
	$(CC) $(CFLAGS) $(CPPFLAGS) -o yfs-tests yfs-tests.c $(IOLIB) $(YFS)
	@echo "Done Building yfs-tests"

clean:
	rm -f $(ALL) TTY* TRACE core* DISK