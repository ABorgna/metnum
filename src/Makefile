CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS= -Wall -Wextra -O2 -std=c++11
LDFLAGS=
LDLIBS=

SRCS=tp1.cpp Parser.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: tp1

tp1: $(OBJS)
	$(CXX) $(LDFLAGS) -o tp1 $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend
