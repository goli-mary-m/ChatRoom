#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[]){

	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	char buffer[256];
	if(argc < 4){
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(1);
	}
	
	char * client_name = argv[3];
	
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("Error opening socket");
		exit(0);
	}
	
	server = gethostbyname(argv[1]);
	if(server == NULL){
		fprintf(stderr, "Error, no such host");
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		perror("Connection failed");
		exit(0);
	}
	

		
	while(1){
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		n = write(sockfd, buffer, strlen(buffer));
		if(n < 0){
			perror("Error on writing");
			exit(0);
		}
		
		
		bzero(buffer, 255);
		n = read(sockfd, buffer, 255);
		if(n < 0){
			perror("Error on reading");
			exit(0);
		}

		printf ("%s", buffer);
	
	}
	
	close(sockfd);
	return 0;
}