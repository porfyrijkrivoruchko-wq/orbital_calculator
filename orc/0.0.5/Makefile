orc:	orc.o skymech.o expr.tab.o units.o
	g++ -o orc orc.o units.o skymech.o expr.tab.o

orc.o:	orc.cpp skymech.hpp phisval.hpp units.hpp
	g++ -c orc.cpp

skymech.o:	skymech.cpp skymech.hpp phisval.hpp units.hpp
	g++ -c skymech.cpp

units.o:	units.cpp units.hpp
	g++ -c units.cpp

expr.tab.o: expr.tab.c
	g++ -c expr.tab.c

expr.tab.c: expr.y
	bison expr.y

orc-dbg: orc.cpp skymech.cpp skymech.hpp phisval.hpp units.cpp units.hpp expr.tab.c
	g++ -o orc-dbg orc.cpp units.cpp skymech.cpp expr.tab.c -g

clear:
	rm *.o
