CXXFLAGS = -g -Wall

all : ssparse

ssparse : Siteswap.o SSParse.o Utility.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

GOODFILE = patterns
BADFILE = badpatterns
check : ssparse $(GOODFILE) $(BADFILE)
	for p in `cat $(GOODFILE)` ; do ./$< "$$p" || exit 1 ; done
	for p in `cat $(BADFILE)` ; do ./$< "$$p" && exit 1 ; done || true
