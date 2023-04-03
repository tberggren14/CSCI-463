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
#include <iostream>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <string>

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
* Prints the response the server sent back 
*
* @param int fd for the file descript
* @return int 0 for success and -1 for fail
*
* @note The server will send back information for the checksum
********************************************************************************/
static int print_response(int fd)
{
     char buf[2048];
     int rval = 1;

     while(rval > 0)
     {
          if((rval = read(fd,buf,sizeof(buf)-1)) == -1)
          {
               perror("Reading stream message");
               return -1;
          } 
          else if(rval > 0)
          {
               std::cout << buf;
          }
     }
     return 0;
}
     
/********************************************************************************
* Main function for the client
*
* @param argc an int for the argument count
* @param argv a char for the argument values
* @return int
*
* @note During the request phase of the transaction the client application will read binary input data from stdin 
* and send it to the server using a TCP socket connection until it encounters EOF on stdin. Then call 
* print_response to print the response from the server
********************************************************************************/
int main(int argc, char *argv[])
{
     int sock;
     struct sockaddr_in server;
     char address[] = "127.0.0.1";
     ssize_t len;

     /* Connect socket using name specified by command line. */
     server.sin_family = AF_INET;

     int opt;
	while ((opt = getopt(argc, argv, "s:")) != -1)
    {
        switch(opt)
        {
            case 's':
            {
               std::istringstream iss(optarg);
            	iss >> address;
               server.sin_port = htons(atoi(argv[3]));               
            }
            break;

        }
    }

    if(argc == 2)
    {
          server.sin_port = htons(atoi(argv[1]));
    }

     /* Create socket */
     sock = socket(AF_INET, SOCK_STREAM, 0);
     if (sock < 0) {
	  perror("opening stream socket");
	  exit(1);
     }
     

     // IP version 4
     if(inet_pton(AF_INET, address, &server.sin_addr) <=0)
     {
          perror("invalid address/format\n");
          exit (2);
     }

     //server.sin_port = htons(atoi(argv[1]));
     if (connect (sock, (sockaddr*)&server, sizeof(server)) < 0) {
          perror ("connecting stream socket");
          exit(1);
      } 

          char buf[2048];
          while((len = read(fileno(stdin), buf, sizeof(buf)-1)) > 0)


      if(safe_write(sock, buf, len) < 0)
          perror ("writing on stream socket");

     shutdown(sock, SHUT_WR);
     print_response(sock);
     close(sock);
}

