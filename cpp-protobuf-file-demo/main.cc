
// gcc -std=c++11 -Wall -fsyntax-only main.cc

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include "metrics.pb.h"

using namespace std;

int errm(const string& message) {
  cerr << message << endl;
  return -1;
}

int usage(char * argv[]) {
  cerr << "Usage: " << argv[0] << " w|r" << endl;
  return -1;
}

int main(int argc, char * argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 3) {
    return usage(argv);
  }

  metrics::Header header;

  if (strcmp(argv[1], "r") == 0) {
    fstream input(argv[2], ios::in | ios::binary);
    if (!header.ParseFromIstream(&input)) {
      return errm("Failed to read from istream");
    }
    cout << "OK: read from istream succeeded, header.contentLength=" << header.contentlength() << ", header.contentType=" << header.contenttype()  << endl;
  } else if (strcmp(argv[1], "w") == 0) {
    fstream output(argv[2], ios::out | ios::trunc | ios::binary);
    
    header.set_contentlength(123);
    header.set_contenttype("test");

    if (!header.SerializeToOstream(&output)) {
      return errm("Failed to serialize to ostream");
    }

    cout << "OK: write to ostream succeeded" << endl;
  } else {
    return usage(argv);
  }

  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}

