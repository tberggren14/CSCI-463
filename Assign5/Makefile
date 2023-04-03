#
# PROGRAM: 3
# AUTHOR:  Trevor Berggren
#

# 	Trevor Berggren
#	z1906236
#	CSCI 340 
#	I certify that this is my own work and where appropriate an extension 
#	of the starter code provided for the assignment.

 rv32i: main.cpp hex.cpp rv32i_decode.cpp memory.cpp rv32i_hart.cpp registerfile.cpp cpu_single_hart.cpp hex.h rv32i_decode.h memory.h rv32i_hart.h registerfile.h cpu_single_hart.h
	g++ -g -Wall -Werror -std=c++14 -c -o main.o main.cpp
	g++ -g -Wall -Werror -std=c++14 -c -o rv32i_decode.o rv32i_decode.cpp
	g++ -g -Wall -Werror -std=c++14 -c -o memory.o memory.cpp
	g++ -g -Wall -Werror -std=c++14 -c -o hex.o hex.cpp
	g++ -g -Wall -Werror -std=c++14 -c -o registerfile.o registerfile.cpp
	g++ -g -Wall -Werror -std=c++14 -c -o rv32i_hart.o rv32i_hart.cpp
	g++ -g -Wall -Werror -std=c++14 -c -o cpu_single_hart.o cpu_single_hart.cpp
	g++ -g -Wall -Werror -std=c++14 -o rv32i main.o rv32i_decode.o memory.o hex.o registerfile.o rv32i_hart.o cpu_single_hart.o

clean:
	rm rv32i main.o rv32i_decode.o memory.o hex.o registerfile.o rv32i_hart.o cpu_single_hart.o