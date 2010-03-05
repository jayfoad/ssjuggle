CXXFLAGS = -g -Wall

ssparse : Siteswap.o SSParse.o Utility.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
