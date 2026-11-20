#include "gateway.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#ifdef __linux__
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
int main(int argc,char**argv){
 if(argc>=2 && std::string(argv[1])=="--simulate"){ // text frames: id len hexbytes... routes 256->512 @10ms
   Gateway g; g.add_route(256,512,10); long long t=0; uint32_t id; int len;
   while(std::cin>>id>>len){ Frame f; f.id=id; f.len=len; f.fd=len>8; for(int i=0;i<len;i++){int b;std::cin>>std::hex>>b>>std::dec; f.data[i]=b;}
     auto o=g.route(f,t); std::cout<<(o? "FORWARD "+std::to_string(o->id) : "DROP")<<"\n"; t+=5; } return 0; }
 std::cout<<"adas-gateway: use --simulate, or run on Linux with vcan (SocketCAN loop in docs)\n";
#ifdef __linux__
 std::cout<<"SocketCAN available: sudo ip link add dev vcan0 type vcan; sudo ip link set vcan0 up\n";
#endif
 return 0; }
