PACKAGES = cairomm-1.0 gtkmm-2.4
CXXFLAGS = -g -Wall `pkg-config --cflags $(PACKAGES)`
LDFLAGS = `pkg-config --libs $(PACKAGES)`

all : ssparse ssjuggle

ssparse : Siteswap.o SSParse.o Utility.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

ssjuggle : GTKMain.o Juggler.o Siteswap.o Utility.o
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

GOODFILE = patterns
BADFILE = badpatterns
check : ssparse $(GOODFILE) $(BADFILE)
	for p in `cat $(GOODFILE)` ; do ./$< "$$p" || exit 1 ; done
	for p in `cat $(BADFILE)` ; do ./$< "$$p" && exit 1 ; done || true

%.d : %.cpp
	$(CXX) -MM $(CPPFLAGS) $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

-include $(patsubst %.cpp,%.d,$(wildcard *.cpp))
