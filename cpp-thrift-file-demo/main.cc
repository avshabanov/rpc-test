
// gcc -std=c++11 -Wall -fsyntax-only main.cc

#include <iostream>
#include <string>
#include <cstring>

#include <unistd.h>
#include <fcntl.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TDenseProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
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
  cerr << "Usage: " << argv[0] << " w|r" << endl;
  return -1;
}

int main(int argc, char * argv[]) {
  if (argc != 3) {
    return usage(argv);
  }

  //metrics::Header header;

  if (strcmp(argv[1], "r") == 0) {
    int fd = open(argv[2], O_RDONLY);
    if (fd < 0) {
      return errm("Failed to open a file");
    }

    shared_ptr<TFDTransport> innerTransport(new TFDTransport(fd));
    shared_ptr<TBufferedTransport> transport(new TBufferedTransport(innerTransport));

    shared_ptr<TBinaryProtocol> protocol(new TBinaryProtocol(transport));

    transport->open();

    metrics::Header header;
    header.read(protocol.get());

    transport->close();

    cout << "OK: read from file succeeded, header.contentLength=" << header.contentLength << ", header.contentType=" << header.contentType  << endl;
  } else if (strcmp(argv[1], "w") == 0) {
    int fd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IXUSR);
    if (fd < 0) {
      return errm("Failed to open a file");
    }
    shared_ptr<TFDTransport> innerTransport(new TFDTransport(fd));
    shared_ptr<TBufferedTransport> transport(new TBufferedTransport(innerTransport));

    shared_ptr<TBinaryProtocol> protocol(new TBinaryProtocol(transport));

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

