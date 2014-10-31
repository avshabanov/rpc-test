
CXX_FLAGS					= -std=c++11 -Wall -pedantic -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H -Itarget/gen
CXX								= g++
LFLAGS						= -lthrift
LINKER						= g++

# Folder with object files
TO								= target/obj

# Folder with generated thrift classes
TG								= target/gen/thrift/gen-cpp

THRIFT						= thrift

## Rules

.PHONY: clean thrift-gen dirs

target/mcli: $(TO)/main.o $(TO)/metrics_constants.o $(TO)/metrics_types.o
	$(LINKER) -o target/mcli $(TO)/main.o $(TO)/metrics_constants.o $(TO)/metrics_types.o $(LFLAGS)

# Thrift Generated Sources Compilation

$(TO)/metrics_constants.o: $(TG)/metrics_constants.cpp
	$(CXX) $(CXX_FLAGS) -c $(TG)/metrics_constants.cpp -o $(TO)/metrics_constants.o

$(TO)/metrics_types.o: $(TG)/metrics_types.cpp
	$(CXX) $(CXX_FLAGS) -c $(TG)/metrics_types.cpp -o $(TO)/metrics_types.o


# Main Sources Compilation

$(TO)/main.o: thrift-gen main.cc
	$(CXX) $(CXX_FLAGS) -c main.cc -o $(TO)/main.o



thrift-gen: dirs thrift/metrics.thrift
	$(THRIFT) -o target/gen/thrift --gen cpp thrift/metrics.thrift

# Object files (obj) and generated sources (gen)
dirs:
	mkdir -p target/obj
	mkdir -p target/gen/thrift

clean:
	rm -rf target


