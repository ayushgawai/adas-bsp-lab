#include "gateway.hpp"
#include <sstream>
std::vector<Signal> parse_signals(const std::string& t){ std::vector<Signal> v; std::istringstream in(t); std::string line;
 while(std::getline(in,line)){ if(line.rfind("SIGNAL",0)!=0) continue; std::istringstream ls(line); std::string kw; Signal s;
   if(ls>>kw>>s.name>>s.id>>s.start>>s.len>>s.scale>>s.offset){ ls>>s.is_signed; v.push_back(s);} } return v; }
double decode_signal(const Signal& s,const Frame& f){ uint64_t raw=0;
 for(int i=0;i<s.len;i++){ int bit=s.start+i; int byte=bit/8; if(byte>=f.len) break; if((f.data[byte]>> (bit%8))&1) raw|=(1ULL<<i); }
 if(s.is_signed && s.len>0 && (raw>>(s.len-1))) raw |= (~0ULL<<s.len);
 return (double)(int64_t)raw * s.scale + s.offset; }
std::optional<Frame> Gateway::route(const Frame& f,long long now){ auto it=routes_.find(f.id); if(it==routes_.end()) return std::nullopt;
 auto lt=last_.find(f.id); if(lt!=last_.end() && now-lt->second < it->second.min_interval_ms) return std::nullopt;
 last_[f.id]=now; Frame o=f; o.id=it->second.out_id; return o; }
