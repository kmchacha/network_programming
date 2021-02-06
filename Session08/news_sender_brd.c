#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int send_sock;
	struct sockaddr_in broad_adr;
	FILE *fp;
	char buf[BUF_SIZE];
	int so_brd = 1;
	
	if (argc != 3) {
		printf("Usage : %s <Boradcast IP> <PORT>\n", argv[0]);
		exit(1);
	}
  
	send_sock = socket(PF_INET, SOCK_DGRAM, 0);	
	memset(&broad_adr, 0, sizeof(broad_adr));
	broad_adr.sin_family = AF_INET;
	broad_adr.sin_addr.s_addr = inet_addr(argv[1]);
	broad_adr.sin_port=htons(atoi(argv[2]));
	
	setsockopt(send_sock, SOL_SOCKET, 
		SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));	
	// SO_BROADCAST : enable one disable of the process to send broadcast message(only datagram socket)
	
	//if ((fp = fopen("news.txt", "r")) == NULL)
	//	error_handling("fopen() error");

	while (1)
	{
		fputs("Input message(Q to quit): ", stdout); // receive from user
		fgets(buf, BUF_SIZE, stdin);
		if (!strcmp(buf,"q\n") || !strcmp(buf,"Q\n"))
			break;

		sendto(send_sock, buf, strlen(buf), 
			0, (struct sockaddr*)&broad_adr, sizeof(broad_adr));

		sleep(2);
	}

	close(send_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}