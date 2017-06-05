.PHONY: clean tests protobufs

clean:
	rm proto/*.pb.h proto/*.pb.cc

protobufs:
	~/Workspace/protobuf-3.3.0/bin/protoc -Iproto proto/telemetry.proto --cpp_out=proto
	~/Workspace/protobuf-3.3.0/bin/protoc -Iproto proto/telecommand.proto --cpp_out=proto
	~/Workspace/protobuf-3.3.0/bin/protoc -Iproto proto/subsystems.proto --cpp_out=proto

tests:
	c++ tests.cpp ./proto/*.pb.cc ./subsystems/*.cpp cdh.cpp checksum.cpp crc32.c -o tests `pkg-config --cflags --libs protobuf`
	./tests && rm tests
