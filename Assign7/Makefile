all:	client	server

client: client.cpp
	g++ -g -Wall -Werror -std=c++14 client.cpp -o client
server: server.cpp
	g++ -g -Wall -Werror -std=c++14  server.cpp -o server

clean:
	rm server client
