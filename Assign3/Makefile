#
# PROGRAM: 3
# AUTHOR:  Trevor Berggren
#

# 	Trevor Berggren
#	z1906236
#	CSCI 340 
#	I certify that this is my own work and where appropriate an extension 
#	of the starter code provided for the assignment.

 memsim: main.cpp hex.cpp memory.cpp hex.h memory.h
	g++ -g -ansi -pedantic -Wall -Werror -Wextra -std=c++14 -c -o main.o main.cpp
	g++ -g -ansi -pedantic -Wall -Werror -Wextra -std=c++14 -c -o memory.o memory.cpp
	g++ -g -ansi -pedantic -Wall -Werror -Wextra -std=c++14 -c -o hex.o hex.cpp
	g++ -g -ansi -pedantic -Wall -Werror -Wextra -std=c++14 -o memsim main.o memory.o hex.o


clean:
	rm memsim main.o memory.o hex.o