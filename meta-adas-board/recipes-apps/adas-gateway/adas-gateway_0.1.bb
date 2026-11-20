SUMMARY = "CAN-FD gateway"
LICENSE = "MIT"
SRC_URI = "file://gateway.cpp file://gateway.hpp file://main.cpp"
S = "${WORKDIR}"
do_compile() { ${CXX} -std=c++17 -O2 -o adas-gateway main.cpp gateway.cpp; }
do_install() { install -Dm755 adas-gateway ${D}${bindir}/adas-gateway; }
