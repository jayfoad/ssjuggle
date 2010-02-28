CXXFLAGS = -Wall

juggle : Siteswap.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
