.PHONY: clean tests protobufs

clean:
	rm proto/*.pb.h proto/*.pb.cc

protobufs:
	~/Workspace/protobuf-3.3.0/bin/protoc -Iproto proto/telemetry.proto --cpp_out=proto

tests:
	c++ tests.cpp ./proto/telemetry.pb.cc -o tests `pkg-config --cflags --libs protobuf`
	./tests && rm tests
