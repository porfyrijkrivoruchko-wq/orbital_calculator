orc:	orc.o skymech.o
	g++ -o orc orc.o skymech.o

orc.o:	orc.cpp skymech.hpp ../phisval.hpp
	g++ -c orc.cpp

skymech.o:	skymech.cpp skymech.hpp ../phisval.hpp
	g++ -c skymech.cpp
