CC=gcc
CXX=g++
RM=rm -f

EXEC=tp3

SRCDIR=src
TESTDIR=test
DATADIR=data

INC=
INC_PARAMS=$(foreach d,$(INC),-I$d)

CPPFLAGS=$(INC_PARAMS) -g -Wall -Wextra -O2 -std=c++14 -pthread
LDFLAGS=
LDLIBS=

SRCS=$(shell find $(SRCDIR) -type f -name *.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

.PHONY: all depend tests clean distclean

all: $(EXEC)

$(EXEC): depend $(OBJS)
	$(CXX) $(LDFLAGS) $(CPPFLAGS) -o $(EXEC) $(OBJS) $(LDLIBS)

tests: tp1 test/tester.py
	cd test;
	python3 tester.py

depend: .depend

.depend: $(SRCS)
	@$(RM) ./.depend
	@$(CXX) $(CPPFLAGS) -MM $^ | \
		sed 's|[a-zA-Z0-9_-]*\.o: \([\a-zA-Z0-9_-]*\)\.cpp|\1.o: \1.cpp|' >> ./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend
