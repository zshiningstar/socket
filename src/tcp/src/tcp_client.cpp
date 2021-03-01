#include <ros/ros.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/shm.h>

#define    MYPORT     8080
#define    BUF_SIZE   1024

char* SERVER_IP = "192.168.1.50";
int result = 0;

using namespace std;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "client_node");
	ros::NodeHandle n;
	
	/*
	 *@fuc: socket()创建socket描述字
	 *
	 */
	int socket_cli = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;  //struct关键字可不写
	memset(&addr, 0, sizeof(addr));
	addr.sin_family      = AF_INET;
	addr.sin_port        = htons(MYPORT);
	addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	
	std::cout << "connecting %s   %d\n",SERVER_IP,MYPORT);
	
	if(connect(socket_cli, (struct sockaddr*) &saddr, sizeof(addr)) < 0)
	{ 
		std::cout << "connect error\n";
		return -1;
	}
	else
		std::cout << "connected successfullly!\n";
		

	while(ros::ok)
	{
		recv(socket_cli, recvbuf, sizeof(recvbuf), 0);
		if(recvbuf[0] == 0x66 && recvbuf[1] == 0xcc)
		{
			int fir_num = recvbuf[2];
			int sec_num = recvbuf[3];
			result = fir_num * sec_num;
			std::cout << "the result is:" << result;
		}
		close(socket_cli);
		return 0;
	}
}

