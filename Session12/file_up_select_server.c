#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 32
#define MAX_CONN 10
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout;
	fd_set reads, cpy_reads;
	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i;
	char buf[BUF_SIZE];
	
	int read_cnt;
	char file_name[MAX_CONN][BUF_SIZE];
	FILE *fp[MAX_CONN];
	int read_first[MAX_CONN];

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);   
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));
	
	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	for (i = 0; i < MAX_CONN; i++)
		read_first[i] = 1;

	// TODO: init descriptor 
	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	fd_max = serv_sock;

	while (1)
	{
		cpy_reads = reads;

		// TODO: set timeer
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		// TODO: select()
		if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1)
			break;

		if (fd_num == 0)
			continue; 

		for (i = 0; i < fd_max + 1; i++)
		{
			if (FD_ISSET(i, &cpy_reads))// TODO: check file descriptor
			{
				if (i == serv_sock)		// connection request! 
				{
					// TODO: when connecion request 
					adr_sz = sizeof(clnt_adr);
					clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);

					FD_SET(clnt_sock, &reads);
					if(fd_max < clnt_sock)
						fd_max = clnt_sock;

					printf("Connected client IP(%d): %s \n", i, inet_ntoa(clnt_adr.sin_addr)); 
					read_first[i] = 1;
				}
				else if (read_first[i]) // read file name 
				{
					memset(file_name[i], 0, BUF_SIZE);
					read(i, file_name[i], BUF_SIZE);
					fp[i] = fopen(file_name[i], "wb");
					printf("Received File name:%s \n", file_name[i]);
					read_first[i] = 0;
				}
				else					// write file data 
				{
					// TODO: receive file data from socket
					read_cnt = read(i, buf, 1);
					
					if (read_cnt == 0)	// close request!
					{
						printf("Complete!: %s \n", file_name[i]);

						// TODO: when connection close (send "Thank you" message to client)
						write(i, "Thank you", BUF_SIZE); 	
						FD_CLR(i, &reads);
						close(i);
						fclose(fp[i]);

						printf("Closed client: %d \n", i);
						read_first[i] = 1;
					}
					else 
					{
						// TODO: write data into file		
						fwrite(buf, read_cnt, 1 , fp[i]);
						fflush(fp[i]);
						
					}
				}
			}
		}
	}
	
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
