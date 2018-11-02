CC=gcc
CXX=g++
RM=rm -f

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

DATASET_COMPRESSED=$(DATADIR)/data.tar.xz
DATASET=$(DATADIR)/imdb_tokenized.csv

.PHONY: all depend tests clean distclean dataset

all: dataset tp2

test_tp2: CPPFLAGS+=-DTEST
test_tp2: clean tp2
	$(RM) $(OBJS)

dataset: $(DATASET)

$(DATASET): $(DATASET_COMPRESSED)
	@echo "Descomprimiendo el dataset"
	tar xf $(DATASET_COMPRESSED) -C data/
	@touch "$@"

tp2: depend $(OBJS)
	$(CXX) $(LDFLAGS) $(CPPFLAGS) -o tp2 $(OBJS) $(LDLIBS)

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
