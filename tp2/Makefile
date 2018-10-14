CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS= -Wall -Wextra -O2 -std=c++11
LDFLAGS=
LDLIBS=
INC=lib/eigen/Eigen
INC_PARAMS=$(foreach d, $(INC), -I$d)

SRCS=src/tp2.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: tp2

tp2: $(OBJS)
	$(CXX) $(LDFLAGS) $(INC_PARAMS) -o tp2 $(OBJS) $(LDLIBS)

tests: tp1 test/tester.py
	cd test;
	python3 tester.py
depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend
