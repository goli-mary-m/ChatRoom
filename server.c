#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdbool.h>


int main (int argc, char *argv[]){
	
	if(argc < 2){
		fprintf(stderr, "Port No not provided. Program terminated\n");
		exit(1);
	}
	
	int n_client = 3;
	
	int sockfd, newsockfd1, newsockfd2, newsockfd3, portno, n;
	char buffer1[255];
	char buffer2[255];
	char buffer3[255];
	
	char buffer_tmp [255];
	
	struct sockaddr_in serv_addr;
	
	struct sockaddr_in cli1_addr, cli2_addr, cli3_addr;
	socklen_t clilen1, clilen2, clilen3;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("Error opening socket");
		exit(0);
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		perror("Binding Failed");
		exit(0);
	}
	
	listen(sockfd, 3);	
	clilen1 = sizeof(cli1_addr);
	clilen2 = sizeof(cli2_addr);
	clilen3 = sizeof(cli3_addr);
	
	newsockfd1 = accept(sockfd, (struct sockaddr *) &cli1_addr, &clilen1);	
	if(newsockfd1 < 0){
		perror("Error on accept client1");
		exit(0);
	}
	
	newsockfd2 = accept(sockfd, (struct sockaddr *) &cli2_addr, &clilen2);	
	if(newsockfd2 < 0){
		perror("Error on accept client2");
		exit(0);
	}
	
	newsockfd3 = accept(sockfd, (struct sockaddr *) &cli3_addr, &clilen3);	
	if(newsockfd3 < 0){
		perror("Error on accept client3");
		exit(0);
	}
	
	printf("3 clients connected ...\n");


	// 2 group
	// each element of group array: 1=is-in-group , 0=is-not-in-group
	int group1 [n_client];
	int group2 [n_client];
	// initialize -> 0
	for(int i=0; i < n_client; i++){
		group1[i] = 0;
		group2[i] = 0;
	}
	
	// command for join group             : join  [group_id]
	// command for leave group            : leave [group_id]
	// command for send message           : send  [group_id] [message]
	// command for quit chat application  : quit
	
	
	char message_buffer1[255];
	char message_buffer2[255];	
	char message_buffer3[255];	
	
	char outbuffer1[255];
	char outbuffer2[255];	
	char outbuffer3[255];
				
	int group_id;
	
	bool flag1 = true;
	bool flag2 = true;
	bool flag3 = true;
	
	while(flag1 || flag2 || flag3){
	
			bzero(outbuffer1, 255);
			bzero(outbuffer2, 255);
			bzero(outbuffer3, 255);

			
			printf("------------------------------------------------------\n");
		
			bzero(buffer1, 255);		
			if(flag1 == true){
				n = read(newsockfd1, buffer1, 255);
				if(n < 0){
					perror("Error on reading");
					exit(0);
				}			
			}
			
			
			bzero(buffer2, 255);
			if(flag2 == true){
				n = read(newsockfd2, buffer2, 255);
				if(n < 0){
					perror("Error on reading");
					exit(0);
				}			
			}
			
			
			bzero(buffer3, 255);			
			if(flag3 == true){
				n = read(newsockfd3, buffer3, 255);
				if(n < 0){
					perror("Error on reading");
					exit(0);
				}			
			}
			
									
					
		// client-1
		//-------------------------------------------------------------------------------------------			

			
			// join client-1 to group with id = group_id
			if (strstr(buffer1, "join") != NULL){
				group_id = buffer1[5] - '0';
				if (group_id == 1){
					if(group1[0] == 0){
						group1[0] = 1;
						printf("-> client-1 added to group %d\n", group_id);		
					}else{
						printf("-> client-1 is in group 1\n");	
					}					
				}
				if (group_id == 2){
					if(group2[0] == 0){
						group2[0] = 1;
						printf("-> client-1 added to group %d\n", group_id);		
					}else{
						printf("-> client-1 is in group 2\n");	
					}
				}
				printf("   group1 -> %d %d %d\n",group1[0], group1[1], group1[2]);
				printf("   group2 -> %d %d %d\n",group2[0], group2[1], group2[2]);
				
				
				strcat(outbuffer1, "-> Server: done\n");
				
			}
			
			
			
			// leave from group with id = group_id
			if (strstr(buffer1, "leave") != NULL){
				group_id = buffer1[6] - '0';
				if (group_id == 1){
					if(group1[0] == 1){
						group1[0] = 0;
						printf("-> client-1 left the group %d\n", group_id);		
					}else{
						printf("-> client-1 is not in group 1\n");	
					}
				}
				if (group_id == 2){
					if(group2[0] == 1){
						group2[0] = 0;
						printf("-> client-1 left the group %d\n", group_id);		
					}else{
						printf("-> client-1 is not in group 2\n");	
					}
				}
				printf("   group1 -> %d %d %d\n",group1[0], group1[1], group1[2]);
				printf("   group2 -> %d %d %d\n",group2[0], group2[1], group2[2]);	
			
				
				strcat(outbuffer1, "-> Server: done\n");								
			}
			
			
			

 			// send message to group with id = group_id
			if (strstr(buffer1, "send") != NULL){
				group_id = buffer1[5] - '0';
				bzero(message_buffer1, 255);
				strncpy(message_buffer1, (char *)(buffer1+7), strlen(buffer1)-7);
				
				bzero(buffer_tmp, 255);
				strcpy(buffer_tmp, "-> client-1: ");
				strcat(buffer_tmp, message_buffer1);
							
				// client-1 -> send message to member of group
				if (group_id == 1){
					// send for client-2
					if (group1[0] == 1 && group1[1] == 1){
						strcat(outbuffer2, buffer_tmp);
						printf("-> client-1 send message to client-2: %s", message_buffer1);
					}
					
					// send for client-3
					if (group1[0] == 1 && group1[2] == 1){
						strcat(outbuffer3, buffer_tmp);
						printf("-> client-1 send message to client-3: %s", message_buffer1);
					}
				}
				
				
				if (group_id == 2){
					// send for client-2
					if (group2[0] == 1 && group2[1] == 1){	
						strcat(outbuffer2, buffer_tmp);	
						printf("-> client-1 send message to client-2: %s", message_buffer1);
					}
					
					// send for client-3
					if (group2[0] == 1 && group2[2] == 1){
						strcat(outbuffer3, buffer_tmp);
						printf("-> client-1 send message to client-3: %s", message_buffer1);
					}
				}
				
				
				strcat(outbuffer1, "-> Server: done\n");				

			}
	
	
			// quit client-1 
			if (strstr(buffer1, "quit") != NULL){
				printf("-> client-1 quit chat application\n");				
				flag1 = false;
			}

		//-------------------------------------------------------------------------------------------
		
		
		// client-2
		//-------------------------------------------------------------------------------------------
			
			
			// join client-2 to group with id = group_id
			if (strstr(buffer2, "join") != NULL){
				group_id = buffer2[5] - '0';
				if (group_id == 1){
					if(group1[1] == 0){
						group1[1] = 1;
						printf("-> client-2 added to group %d\n", group_id);		
					}else{
						printf("-> client-2 is in group 1\n");	
					}
				}
				if (group_id == 2){
					if(group2[1] == 0){
						group2[1] = 1;
						printf("-> client-2 added to group %d\n", group_id);		
					}else{
						printf("-> client-2 is in group 2\n");	
					}				
				}
				printf("   group1 -> %d %d %d\n",group1[0], group1[1], group1[2]);
				printf("   group2 -> %d %d %d\n",group2[0], group2[1], group2[2]);	
				
				strcat(outbuffer2, "-> Server: done\n");						
			}
			
			
			
			// leave from group with id = group_id
			if (strstr(buffer2, "leave") != NULL){
				group_id = buffer2[6] - '0';
				if (group_id == 1){
					if(group1[1] == 1){
						group1[1] = 0;
						printf("-> client-2 left the group %d\n", group_id);		
					}else{
						printf("-> client-2 is not in group 1\n");	
					}
				}
				if (group_id == 2){
					if(group2[1] == 1){
						group2[1] = 0;
						printf("-> client-2 left the group %d\n", group_id);		
					}else{
						printf("-> client-2 is not in group 2\n");	
					}
				}
				printf("   group1 -> %d %d %d\n",group1[0], group1[1], group1[2]);
				printf("   group2 -> %d %d %d\n",group2[0], group2[1], group2[2]);
				
				strcat(outbuffer2, "-> Server: done\n");								
			}
			
			

 
 			// send message to group with id = group_id
			if (strstr(buffer2, "send") != NULL){
				group_id = buffer2[5] - '0';
				bzero(message_buffer2, 255);
				strncpy(message_buffer2, (char *)(buffer2+7), strlen(buffer2)-7);
				
				bzero(buffer_tmp, 255);
				strcpy(buffer_tmp, "-> client-2: ");
				strcat(buffer_tmp, message_buffer2);
										
				// client-2 -> send message to member of group
				if (group_id == 1){
					// send for client-1
					if (group1[1] == 1 && group1[0] == 1){
						strcat(outbuffer1, buffer_tmp);
						printf("-> client-2 send message to client-1: %s", message_buffer2);			
					}
					
					// send for client-3
					if (group1[1] == 1 && group1[2] == 1){
						strcat(outbuffer3, buffer_tmp);
						printf("-> client-2 send message to client-3: %s", message_buffer2);
					}
				}
				
				
				if (group_id == 2){
					// send for client-1
					if (group2[1] == 1 && group2[0] == 1){
						strcat(outbuffer1, buffer_tmp);
						printf("-> client-2 send message to client-1: %s", message_buffer2);
					}
					
					// send for client-3
					if (group2[1] == 1 && group2[2] == 1){
						strcat(outbuffer3, buffer_tmp);
						printf("-> client-2 send message to client-3: %s", message_buffer2);
					}
				}
				
				
				strcat(outbuffer2, "-> Server: done\n");								
			}		


			// quit client-2
			if (strstr(buffer2, "quit") != NULL){
				printf("-> client-2 quit chat application\n");			
				flag2 = false;							
			}

		//-------------------------------------------------------------------------------------------
		
		
		// client-3
		//-------------------------------------------------------------------------------------------
		
			
			// join client-3 to group with id = group_id
			if (strstr(buffer3, "join") != NULL){
				group_id = buffer3[5] - '0';
				if (group_id == 1){
					if(group1[2] == 0){
						group1[2] = 1;
						printf("-> client-3 added to group %d\n", group_id);		
					}else{
						printf("-> client-3 is in group 1\n");	
					}
				}
				if (group_id == 2){
					if(group2[2] == 0){
						group2[2] = 1;
						printf("-> client-3 added to group %d\n", group_id);		
					}else{
						printf("-> client-3 is in group 2\n");	
					}
				}
				printf("   group1 -> %d %d %d\n",group1[0], group1[1], group1[2]);
				printf("   group2 -> %d %d %d\n",group2[0], group2[1], group2[2]);
				
				strcat(outbuffer3, "-> Server: done\n");								
			}
			
			
			
			// leave from group with id = group_id
			if (strstr(buffer3, "leave") != NULL){
				group_id = buffer3[6] - '0';
				if (group_id == 1){
					if(group1[2] == 1){
						group1[2] = 0;
						printf("-> client-3 left the group %d\n", group_id);		
					}else{
						printf("-> client-3 is not in group 1\n");	
					}
				}
				if (group_id == 2){
					if(group2[2] == 1){
						group2[2] = 0;
						printf("-> client-3 left the group %d\n", group_id);		
					}else{
						printf("-> client-3 is not in group 2\n");	
					}
				}
				printf("   group1 -> %d %d %d\n",group1[0], group1[1], group1[2]);
				printf("   group2 -> %d %d %d\n",group2[0], group2[1], group2[2]);
				
				strcat(outbuffer3, "-> Server: done\n");							
			}
			
			
			

 			// send message to group with id = group_id
			if (strstr(buffer3, "send") != NULL){
				group_id = buffer3[5] - '0';
				bzero(message_buffer3, 255);
				strncpy(message_buffer3, (char *)(buffer3+7), strlen(buffer3)-7);
				
				bzero(buffer_tmp, 255);
				strcpy(buffer_tmp, "-> client-3: ");
				strcat(buffer_tmp, message_buffer3);
										
				// client-3 -> send message to member of group
				if (group_id == 1){
					// send for client-1
					if (group1[2] == 1 && group1[0] == 1){
						strcat(outbuffer1, buffer_tmp);
						printf("-> client-3 send message to client-1: %s", message_buffer3);			
					}
					
					// send for client-2
					if (group1[2] == 1 && group1[1] == 1){
						strcat(outbuffer2, buffer_tmp);
						printf("-> client-3 send message to client-2: %s", message_buffer3);
					}
				}
				
				
				if (group_id == 2){
					// send for client-1
					if (group2[2] == 1 && group2[0] == 1){
						strcat(outbuffer1, buffer_tmp);
						printf("-> client-3 send message to client-1: %s", message_buffer3);
					}
					
					// send for client-2
					if (group2[2] == 1 && group2[1] == 1){
						strcat(outbuffer2, buffer_tmp);
						printf("-> client-3 send message to client-2: %s", message_buffer3);
					}
				}
				
				strcat(outbuffer3, "-> Server: done\n");							
			}
	
		
		
			// quit client-3
			if (strstr(buffer3, "quit") != NULL){
				printf("-> client-3 quit chat application\n");				
				flag3 = false;
			}
			
		//-------------------------------------------------------------------------------------------		
		
				
			strcat(outbuffer1, "\n");
			strcat(outbuffer2, "\n");
			strcat(outbuffer3, "\n");		
						
			
			if (flag1 == false){
				close(newsockfd1);
			}else{
				n = write(newsockfd1, outbuffer1, strlen(outbuffer1));
				if(n < 0){
					perror("Error on writing");
					exit(0);
				}			
			}
			
			
			if (flag2 == false){
				close(newsockfd2);
			}else{
				n = write(newsockfd2, outbuffer2, strlen(outbuffer2));
				if(n < 0){
					perror("Error on writing");
					exit(0);
				}			
			}
			
			
			if (flag3 == false){
				close(newsockfd3);
			}else{
				n = write(newsockfd3, outbuffer3, strlen(outbuffer3));
				if(n < 0){
					perror("Error on writing");
					exit(0);
				}			
			}
	}

	
	close(newsockfd1);
	close(newsockfd2);
	close(newsockfd3);	
	close(sockfd);
	return 0;


}