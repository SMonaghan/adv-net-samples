#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc , char *argv[])
{
	int sockfd;
	struct addrinfo *res = 0; //Setting addrinfo struct for getaddrinfo.
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_in *h;
	char *hostname = malloc(sizeof(char) * 1000), *ip = malloc(sizeof(char) * 100);;
	int rc, ctr;

	//if argc size is less than 2, then the hostname was not included as a command line input.	
	if(argc <3){
		printf("Error: Missing Hostname/IP Address or flag to decide IP->Hostname or Hostname->IP\n");
		exit(1);
	}
	//if command line input is "-h", then hostname was given.
	int o;
       while((o = getopt (argc, argv, "h:i:")) != -1){
		switch(o){
			case 'h':
				hostname = optarg; //hostname from command line input.

				memset(&hints, 0, sizeof hints); //setting space for hints.
				hints.ai_family = AF_INET;	 //setting the family type to IPv4 only.
				hints.ai_socktype = SOCK_STREAM; //setting socket type to streams.

				//if the return value is not zero, there is an error.
				if((rc = getaddrinfo(hostname, "http", &hints, &servinfo)) != 0)
				{
					perror(gai_strerror(rc)); //print the error. gai_strerror translates the error to readable human text.
					exit(-1);
				}

				ctr = 1;
				//find the first connection from the results
				for(p = servinfo; p != NULL; p = p->ai_next)
				{
					h = (struct sockaddr_in *) p->ai_addr; //set h to the pointer of the sockaddr struct. ai_addr is a pointer to a filled-in socket address struct.
					strcpy(ip, inet_ntoa(h->sin_addr)); //sin_addr holds the IPv4 address. this is copied to ip.
					printf("%s - IPv4 address #%d: %s\n" ,hostname , ctr, ip);
					ctr++;
				}
				break;

			case 'i':
				
				ip = optarg;     //command line input
				printf("Command line input: %s\n", ip); //output to show user their input.

				rc = getaddrinfo(ip, 0, 0, &res);   //getaddrinfo is used to supply parameters to getnameinfo.
				//error checking getaddrinfo.
				if(rc != 0)
				{
					perror(gai_strerror(rc)); //print the error. gai_strerror translates the error to readable human text.
					exit(-1);
				}

				rc = getnameinfo(res->ai_addr, res->ai_addrlen, hostname, 1000, 0, 0, 0);
				//error checking getnameinfo.
				if(rc != 0)
				{
					perror(gai_strerror(rc)); //print the error. gai_strerror translates the error to readable human text.
					exit(-1);
				}
				printf("Hostname found: %s\n", hostname);
				break;

		}
	}
}
