
// gcc -std=c++11 -Wall -fsyntax-only main.cc

#include <iostream>
#include <string>
#include <cstring>

#include <unistd.h>
#include <fcntl.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/transport/TFDTransport.h>

// Thrift Generated Sources
#include "thrift/gen-cpp/metrics_constants.h"
#include "thrift/gen-cpp/metrics_types.h"

using std::cerr;
using std::cout;
using std::endl;

using namespace boost;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

int errm(const std::string& message) {
  cerr << message << endl;
  return -1;
}

int usage(char * argv[]) {
  cerr << "Usage: " << argv[0] << " b|c(Binary or Compact) w|r(Write or Read)" << endl;
  return -1;
}

shared_ptr<TProtocol> createProtocol(const shared_ptr<TTransport>& transport, bool useCompact) {
  if (useCompact) {
    return shared_ptr<TProtocol>(new TCompactProtocol(transport));
  }
  return shared_ptr<TProtocol>(new TBinaryProtocol(transport));
}

int main(int argc, char * argv[]) {
  if (argc != 4) {
    return usage(argv);
  }

  const char * protocolType = argv[1];
  const char * mode = argv[2];
  const char * fileName = argv[3];

  bool useCompact = (strcmp(protocolType, "c") == 0);

  if (strcmp(mode, "r") == 0) {
    int fd = open(fileName, O_RDONLY);
    if (fd < 0) {
      return errm("Failed to open a file");
    }

    shared_ptr<TFDTransport> innerTransport(new TFDTransport(fd));
    shared_ptr<TBufferedTransport> transport(new TBufferedTransport(innerTransport));
    shared_ptr<TProtocol> protocol = createProtocol(transport, useCompact);

    transport->open();

    metrics::Header header;
    header.read(protocol.get());

    transport->close();

    cout << "OK: read from file succeeded, header.contentLength=" << header.contentLength << ", header.contentType=" << header.contentType  << endl;
  } else if (strcmp(mode, "w") == 0) {
    int fd = open(fileName, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IXUSR);
    if (fd < 0) {
      return errm("Failed to open a file");
    }
    shared_ptr<TFDTransport> innerTransport(new TFDTransport(fd));
    shared_ptr<TBufferedTransport> transport(new TBufferedTransport(innerTransport));
    shared_ptr<TProtocol> protocol = createProtocol(transport, useCompact);

    metrics::Header header;
    header.__set_contentLength(123);
    header.__set_contentType("test");

    header.write(protocol.get());

    transport->close();

    cout << "OK: write to file succeeded" << endl;
  } else {
    return usage(argv);
  }

  return 0;
}

