orc:	orc.o skymech.o expr.tab.o
	g++ -o orc orc.o skymech.o expr.tab.o

orc.o:	orc.cpp skymech.hpp ../phisval.hpp
	g++ -c orc.cpp

skymech.o:	skymech.cpp skymech.hpp ../phisval.hpp
	g++ -c skymech.cpp

expr.tab.o: expr.tab.c
	g++ -c expr.tab.c

expr.tab.c: expr.y
	bison expr.y

orc-dbg: orc.cpp skymech.cpp skymech.hpp ../phisval.hpp expr.tab.c
	g++ -o orc-dbg orc.cpp skymech.cpp expr.tab.c -g
