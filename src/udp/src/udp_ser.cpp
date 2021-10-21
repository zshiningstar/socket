#include <ros/ros.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "std_msgs/String.h"

using namespace std;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main (int argc, char** argv)
{
  ros::init(argc, argv, "server_node");
  ros::NodeHandle nh("~");
  ros::Publisher server_pub = nh.advertise<std_msgs::String>("/server_messages/", 1000);
  int sockfd, newsockfd, portno; //Socket file descriptors and port number
  socklen_t clilen; //object clilen of type socklen_t
  char buffer[256]; //buffer array of size 256
  struct sockaddr_in serv_addr, cli_addr; ///two objects to store client and server address
  std_msgs::String message;
  std::stringstream ss;
  int n;
  ros::Duration d(0.01); // 100Hz
  
  nh.param<int>("portno", portno, 56780);
  cout << "Hello there! This node is listening on port " << portno << " for incoming connections" << endl;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)
      error("ERROR opening socket");
  int enable = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
      error("setsockopt(SO_REUSEADDR) failed");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0)
            error("ERROR on binding");
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd,
              (struct sockaddr *) &cli_addr,
              &clilen);//accept写在循环外
  if (newsockfd < 0)
       error("ERROR on accept");
  while(ros::ok()) {
     ss.str(std::string()); //Clear contents of string stream
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     // printf("Here is the message: %s\n",buffer);
     ss << buffer;
     message.data = ss.str();
     ROS_INFO("11111%s", message.data.c_str());//显示客户端发过来的数据
     server_pub.publish(message);
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     //close(newsockfd);
     //close(sockfd);
     //ros::spinOnce();
     //d.sleep();
  }
  return 0;
}
