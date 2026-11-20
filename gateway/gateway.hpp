#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
struct Frame{ uint32_t id=0; uint8_t len=0; uint8_t data[64]{}; bool fd=false; };
struct Signal{ std::string name; uint32_t id=0; int start=0,len=0; double scale=1,offset=0; bool is_signed=false; };
std::vector<Signal> parse_signals(const std::string& text);
double decode_signal(const Signal& s, const Frame& f);
struct Route{ uint32_t out_id; int min_interval_ms; };
class Gateway{
  std::unordered_map<uint32_t,Route> routes_; std::unordered_map<uint32_t,long long> last_;
 public:
  void add_route(uint32_t in,uint32_t out,int ms){ routes_[in]={out,ms}; }
  std::optional<Frame> route(const Frame& f,long long now_ms);
};
