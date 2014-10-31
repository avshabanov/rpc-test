
CXX_FLAGS					= -std=c++11 -Wall -pedantic -Itarget/gen
CXX								= g++
LFLAGS						= -lprotobuf
LINKER						= g++

PROTOC						= protoc

## Rules

.PHONY: clean dirs

target/mcli: target/obj/main.o target/obj/metrics.pb.o
	$(LINKER) -o target/mcli target/obj/main.o target/obj/metrics.pb.o $(LFLAGS)

target/obj/metrics.pb.o:
	$(CXX) $(CXX_FLAGS) -c target/gen/metrics.pb.cc -o target/obj/metrics.pb.o

target/obj/main.o: dirs main.cc target/gen/metrics.pb.cc
	$(CXX) $(CXX_FLAGS) -c main.cc -o target/obj/main.o

target/gen/metrics.pb.cc: dirs proto/metrics.proto
	$(PROTOC) -I=proto/ --cpp_out=target/gen/ proto/metrics.proto

# Object files (obj) and generated sources (gen)
dirs:
	mkdir -p target/obj
	mkdir -p target/gen

clean:
	rm -rf target


