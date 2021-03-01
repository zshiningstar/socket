#include <ros/ros.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "std_msgs/Int8.h"

using namespace std;
#define PORT 10001

int main (int argc, char** argv)
{
	ros::init(argc, argv, "server_node");
	ros::NodeHandle nh;
	ros::Publisher server_pub = nh.advertise<std_msgs::Int8>("/server_messages/", 1000);
	
    struct sockaddr_in s_in;//server address structure
    struct sockaddr_in c_in;//client address structure
    int l_fd,c_fd;
    socklen_t len;
    char buf[100];//content buff area
    string tem;
    float x_value = 0.0;
    float y_value = 0.0;
    memset((void *)&s_in,0,sizeof(s_in));
 
    s_in.sin_family = AF_INET;//IPV4 communication domain
    s_in.sin_addr.s_addr = INADDR_ANY;//accept any address
    s_in.sin_port = htons(PORT);//change port to netchar
 
    l_fd = socket(AF_INET,SOCK_STREAM,0);//socket(int domain, int type, int protocol)
    bind(l_fd,(struct sockaddr *)&s_in,sizeof(s_in));
    listen(l_fd,1);//同时只能有一个连接
 
    cout<<"begin"<<endl;
    while(ros::ok){
        c_fd = accept(l_fd,(struct sockaddr *)&c_in,&len);
        
            int n = read(c_fd,buf,100);//read the message send by client

            write(c_fd,buf,n);//sent message back to client
        }
    }
    return 0;
}
