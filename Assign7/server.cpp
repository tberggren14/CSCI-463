//***************************************************************************
//
//  Trevor Berggren
//  Z1906236
//  CSCI 463 
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<signal.h>
#include <iostream>
#include<sstream>

/********************************************************************************
* safe_write function to write data over the socket
*
* @param fd an int for the file descript
* @param buf a char for buffer
* @param len a ssize_t for length
* @return ssize_t
*
* @note Writes to socket until the len is 0 meaning everything has been sent
********************************************************************************/
static ssize_t safe_write(int fd, const char *buf, size_t len)
{
     while(len > 0)
     {
          ssize_t wlen = write(fd,buf,len);
          if(wlen == -1)
               return -1;
          len -= wlen;
          buf += wlen;
     }
     return len;
}


/********************************************************************************
* Main function for the client
*
* @param argc an int for the argument count
* @param argv a char for the argument values
* @return int
*
* @note This program creates a socket and then begins an infinite loop. Each time
* through the loop it accepts a connection and prints out messages from it.
* When the connection breaks, or a termination message comes through, the
* program accepts a new connection.
********************************************************************************/
int main(int argc, char *argv[])
{
	int sock;
	socklen_t length;
	struct sockaddr_in server;
	int msgsock;
	char buf[2048];
	char str[INET_ADDRSTRLEN];
	int port = 0;
	int rval;
	uint8_t values = 0;
	uint32_t byteCount = 0;
	uint16_t sum = 0;

	signal(SIGPIPE,SIG_IGN);


	int opt;
	while ((opt = getopt(argc, argv, "l:")) != -1)
    {
        switch(opt)
        {
            case 'l':
            {									// if argument -l is given for the port
                std::istringstream iss(optarg);
            	iss >> std::dec >> port;
				server.sin_port = htons(port);
            }
            break;
        }
    }

     /* Create socket */
     sock = socket(AF_INET, SOCK_STREAM, 0);
     if (sock < 0) {
	  perror("opening stream socket");
	  exit(1);
     }
     /* Name socket using wildcards */
     server.sin_family = AF_INET;
     server.sin_addr.s_addr = inet_addr("127.0.0.1");						// default address
	 inet_ntop(AF_INET, &(server.sin_addr.s_addr), str, INET_ADDRSTRLEN);
    
     if (bind(sock, (sockaddr*)&server, sizeof(server))) {
	  perror("binding stream socket");
	  exit(1);
     }
     /* Find out assigned port number and print it out */
     length = sizeof(server);
     if (getsockname(sock, (sockaddr*)&server, &length)) {
	  perror("getting socket name");
	  exit(1);
     }
     std::cout << "Socket has port " <<  ntohs(server.sin_port) << std::endl;

     /* Start accepting connections */
     listen(sock, 5);
     do {
		struct sockaddr_in from;
		socklen_t from_len = sizeof(from);
		msgsock = accept(sock,(struct sockaddr*)&from, &from_len);
	  if (msgsock == -1)
	       perror("accept");
	  else { 
			inet_ntop(from.sin_family, &from.sin_addr, buf, sizeof (buf));
			std::cout << "Accepted connection from " << buf << " port In " << ntohs(from.sin_port) << std::endl;
			do {
				if ((rval = read(msgsock, &values, 1)) < 0)		// reads one byte at a time to be able to add the value to sum
					perror ("reading stream message");
				if (rval == 0)
					std::cout << "Ending connection\n";
				else
				{
					byteCount += rval;							// adds to the total byteCount
					sum += values;

				}
				} while (rval != 0);
					std::ostringstream os;						// declares and assigns to be able to write the data back to the 
      				os << "Sum: " << sum  << " Len: " << byteCount << "\n";	// client to be printed to stdout
      				std::string str = os.str();
      				const char *ch = str.c_str();

					rval = safe_write(msgsock, ch, str.length());
					close (msgsock);
	  		}

			 byteCount = 0;										// resets the values if the server keeps running
			 sum = 0;
			 values = 0;
		} while (true);
     /*
      * Since this program has an infinite loop, the socket "sock" is
      * never explicitly closed.  However, all sockets will be closed
      * automatically when a process is killed or terminates normally.
      */
}