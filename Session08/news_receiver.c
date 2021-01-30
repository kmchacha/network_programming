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
	int recv_sock;
	int str_len;
	char buf[BUF_SIZE];
	struct sockaddr_in adr;
	struct ip_mreq join_adr;
	// struct ip_mreq{
	// 	struct in_addr imr_multiaddr;
	// 	struct in_addr imr_interface;
	// };

	if (argc != 3) {
		printf("Usage : %s <GroupIP> <PORT>\n", argv[0]);
		exit(1);
	 }
  
	recv_sock = socket(PF_INET, SOCK_DGRAM, 0);	// UDP
 	memset(&adr, 0, sizeof(adr));
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = htonl(INADDR_ANY);	
	adr.sin_port = htons(atoi(argv[2]));
	
	if (bind(recv_sock, (struct sockaddr*) &adr, sizeof(adr)) == -1)
		error_handling("bind() error");
	
	join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);	// GroupIp
	join_adr.imr_interface.s_addr = htonl(INADDR_ANY);	
  	
	setsockopt(recv_sock, IPPROTO_IP, 
		IP_ADD_MEMBERSHIP, (void*)&join_adr, sizeof(join_adr));	
	// 커널에 우리가 관심있는 그룹의 정보를 알려준다 (IP_ADD_MEMBERSHIP)
	// 그룹에 관심있는 프로세스가 하나도 없다면, 그 그룹을 향해서 전송되어 오는 패킷은 모두 폐기된다.
	// 따라서 커널에 우리의 관심 그룹을 알리고, 그룹의 member가 되기 위해서는 다음 과장이 필요하다
	// ip_mreq 구조체의 내용을 채운 후 optval 설정일 사용하여 setsocketopt()를 호출해서 커널에 구조체를 넘겨주는 과정이 필요하다
  
	while (1)
	{
		str_len = recvfrom(recv_sock, buf, BUF_SIZE-1, 0, NULL, 0);
		if (str_len < 0) 
			break;
		buf[str_len] = 0;
		fputs(buf, stdout);
	}
	close(recv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}