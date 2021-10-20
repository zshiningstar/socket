#include <ros/ros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "std_msgs/String.h"

#define MESSAGE_FREQ 1

void error(const char *msg) {
    perror(msg);
    exit(0);
}

class Listener {
private:
    char topic_message[256] = { 0 };
public:
    void callback(const std_msgs::String::ConstPtr& msg);
    char* getMessageValue();
};

void Listener::callback(const std_msgs::String::ConstPtr& msg) {
    memset(topic_message, 0, 256);
    strcpy(topic_message, msg->data.c_str());
//    ROS_INFO("I heard:[%s]", msg->data.c_str());
}

char* Listener::getMessageValue() {
    return topic_message;
}

int main(int argc, char *argv[]) {
	ros::init(argc, argv, "client_node");
	ros::NodeHandle nh("~");//node之间的需要私有句柄获取
    ros::Rate loop_rate(MESSAGE_FREQ); // set the rate as defined in the macro MESSAGE_FREQ
	Listener listener;
    ros::Subscriber client_sub = nh.subscribe("/client_messages", 1, &Listener::callback, &listener);
    int sockfd, portno, n, choice = 1;
    struct sockaddr_in serv_addr;
    char *server = "192.168.6.235";
    char buffer[256];
    
    nh.param<int>("portno", portno, 8080);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    std::cout << "How do you want the client to behave?:\n1. Be able to send messages manually\n2. Subscribe to /client_messages and send whatever's available there\nYour choice:";
    std::cin >> choice;
	while(ros::ok()) {
        bzero(buffer,256);
        if (choice == 1) {
            printf("Please enter the message: ");
            fgets(buffer,255,stdin);
        } else if (choice == 2) {
            strcpy(buffer, listener.getMessageValue());
            loop_rate.sleep();
        }
	    n = write(sockfd,buffer,strlen(buffer));//发送给服务器端进行请求
	    if (n < 0) 
	         error("ERROR writing to socket");
	    ros::spinOnce();
	}
	return 0;
}
