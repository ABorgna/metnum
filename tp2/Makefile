CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS= -g -Wall -Wextra -O2 -std=c++14
LDFLAGS=
LDLIBS=

SRCDIR=src
TESTDIR=test
OBJDIR=build
DATADIR=data

INC=lib lib/eigen/Eigen
INC_PARAMS=$(foreach d, $(INC), -I$d)

SRCS=$(shell find $(SRCDIR) -type f -name *.cpp)
OBJS=$(subst .cpp,.o,$(SRCS))

DATASET_COMPRESSED=$(DATADIR)/data.tar.xz
DATASET=$(DATADIR)/imdb_tokenized.csv

.PHONY: all tp2 depend tests clean distclean dataset

all: tp2

tp2: depend dataset $(OBJDIR)/tp2

dataset: $(DATASET)

$(DATASET): $(DATASET_COMPRESSED)
	@echo "Descomprimiendo el dataset"
	tar xf $(DATASET_COMPRESSED) -C data/
	@touch "$@"

$(OBJDIR)/tp2: $(OBJS)
	$(CXX) $(LDFLAGS) $(INC_PARAMS) -o tp2 $(OBJS) $(LDLIBS)

tests: tp1 test/tester.py
	cd test;
	python3 tester.py

depend: .depend

.depend: $(SRCS)
	@$(RM) ./.depend
	@$(CXX) $(CPPFLAGS) -MM $^ | \
		sed 's|[a-zA-Z0-9_-]*\.o|$(OBJDIR)/&|' >>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend
