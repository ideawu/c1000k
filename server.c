/*****************************************************
 * The TCP socket server to help you to test if your
 * OS supports c1000k(1 million connections).
 * @author: ideawu
 * @link: http://www.ideawu.com/
 *****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/select.h>

#define MAX_PORTS 100

int main(int argc, char **argv){
	if(argc <  2){
		printf("Usage: %s port\n", argv[0]);
		exit(0);
	}

	struct sockaddr_in addr;
	const char *ip = "0.0.0.0";
	int opt = 1;
	int bufsize;
	socklen_t optlen;
	int connections = 0;
	int base_port = 7000;
	if(argc >= 2){
		base_port = atoi(argv[1]);
	}

	int server_socks[MAX_PORTS];

	int i;
	int port;
	for(i=0; i<MAX_PORTS; i++){
		port = base_port + i;
		memset(&addr, sizeof(addr), 0);
		addr.sin_family = AF_INET;
		addr.sin_port = htons((short)port);
		inet_pton(AF_INET, ip, &addr.sin_addr);

		int serv_sock;
		if((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
			goto sock_err;
		}
		if(setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1){
			goto sock_err;
		}
		if(bind(serv_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){
			goto sock_err;
		}
		if(listen(serv_sock, 1024) == -1){
			goto sock_err;
		}

		server_socks[i] = serv_sock;
		printf("server listen on port: %d\n", port);
	}

	while(1){
		fd_set readset;
		FD_ZERO(&readset);
		int maxfd = 0;
		for(int i=0; i<MAX_PORTS; i++){
			FD_SET(server_socks[i], &readset);
			if(server_socks[i] > maxfd){
				maxfd = server_socks[i];
			}
		}
		int ret = select(maxfd + 1, &readset, NULL, NULL, NULL);
		if(ret < 0){
			if(errno == EINTR){
				continue;
			}else{
				printf("select error! %s\n", strerror(errno));
				exit(0);
			}
		}
		if(ret == 0){
			continue;
		}

		for(int i=0; i<MAX_PORTS; i++){
			if(!FD_ISSET(server_socks[i], &readset)){
				continue;
			}
			socklen_t addrlen = sizeof(addr);
			int sock = accept(server_socks[i], (struct sockaddr *)&addr, &addrlen);
			if(sock == -1){
				goto sock_err;
			}
			connections ++;
			if(connections % 1000 == 999){
				//printf("press Enter to continue: ");
				//getchar();
				printf("connections: %d, fd: %d\n", connections, sock);
			}

			bufsize = 5000;
			setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize));
			setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));
		}
	}

	return 0;
sock_err:
	printf("connections: %d\n", connections);
	printf("error: %s\n", strerror(errno));
	return 0;
}
