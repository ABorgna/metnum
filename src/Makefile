CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS= -Wall -Wextra -O3 -std=c++11
LDFLAGS=
LDLIBS=

SRCS=main.cpp Parser.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: main

main: $(OBJS)
	$(CXX) $(LDFLAGS) -o main $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend
