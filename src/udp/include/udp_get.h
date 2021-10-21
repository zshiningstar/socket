#ifndef __UDP_GET_H
#define __UDP_GET_H
#include <iostream>
#include <stdlib.h>
#include <string.h>
//udp
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8888


 struct udp_data
{
  std::string ch;
 //   char  ch[10];
  int flag;       

};     
 
struct udp_data udp_data_buf;

#endif
