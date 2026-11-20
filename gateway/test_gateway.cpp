#include "gateway.hpp"
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
int main(){ std::ifstream in("signals.dbc"); std::stringstream ss; ss<<in.rdbuf();
 auto sigs=parse_signals(ss.str()); assert(sigs.size()==3);
 Frame f; f.id=256; f.len=8; f.data[0]=0x64; f.data[1]=0x00; f.data[2]=0x10; f.data[3]=0x27; // 100, 10000
 assert(std::abs(decode_signal(sigs[0],f)-10.0)<1e-9);
 assert(std::abs(decode_signal(sigs[1],f)-100.0)<1e-9);
 Gateway g; g.add_route(256,512,10); assert(g.route(f,0)); assert(!g.route(f,5)); assert(g.route(f,10)); assert(!g.route(Frame{999},20));
 std::cout<<"test_gateway: PASS\n"; }
