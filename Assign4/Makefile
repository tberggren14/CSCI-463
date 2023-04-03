#
# PROGRAM: 3
# AUTHOR:  Trevor Berggren
#

# 	Trevor Berggren
#	z1906236
#	CSCI 340 
#	I certify that this is my own work and where appropriate an extension 
#	of the starter code provided for the assignment.

 rv32i: main.cpp hex.cpp rv32i_decode.cpp memory.cpp hex.h rv32i_decode.h memory.h
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o main.o main.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o rv32i_decode.o rv32i_decode.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o memory.o memory.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -c -o hex.o hex.cpp
	g++ -g -ansi -pedantic -Wall -Werror -std=c++14 -o rv32i main.o rv32i_decode.o memory.o hex.o
clean:
	rm rv32i main.o memory.o hex.o rv32i_decode.o