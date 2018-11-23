CC=gcc
CXX=g++
RM=rm -f

EXEC=tp3

SRCDIR=src
TESTDIR=test
DATADIR=data

IMGS=$(shell find $(DATADIR) -type f -name *.png)
IMGSCSV=$(subst .png,.csv,$(IMGS))

INC=$(shell pkg-config --cflags --libs opencv)

CPPFLAGS=$(INC) -g -Wall -Wextra -O2 -std=c++14 -pthread
LDFLAGS=
LDLIBS=

SRCS=$(shell find $(SRCDIR) -type f -name *.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

.PHONY: all depend tests clean distclean imgs

all: $(EXEC) imgs

$(EXEC): depend $(OBJS)
	$(CXX) $(LDFLAGS) $(CPPFLAGS) -o $(EXEC) $(OBJS) $(LDLIBS)

tests: tp1 test/tester.py
	cd test;
	python3 tester.py

imgs:
	python3 conversor_csv/csv_converter.py $(DATADIR) $(DATADIR) .png

depend: .depend

.depend: $(SRCS)
	@$(RM) ./.depend
	@$(CXX) $(CPPFLAGS) -MM $^ | \
		sed 's|[a-zA-Z0-9_-]*\.o: \([\a-zA-Z0-9_-]*\)\.cpp|\1.o: \1.cpp|' >> ./.depend;

clean:
	$(RM) $(OBJS) $(IMGSCSV)

distclean: clean
	$(RM) *~ .depend

include .depend
