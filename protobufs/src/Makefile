.PHONY: clean protobufs tmtest tctest

clean:
	rm proto/*.pb.h proto/*.pb.cc

protobufs:
	~/Workspace/protobuf-3.3.0/bin/protoc -Iproto proto/telemetry.proto --cpp_out=proto
	~/Workspace/protobuf-3.3.0/bin/protoc -Iproto proto/telecommand.proto --cpp_out=proto
	~/Workspace/protobuf-3.3.0/bin/protoc -Iproto proto/subsystems.proto --cpp_out=proto

tmtest:
	g++ -std=gnu++11 -DTEST_MOCK_TELEMETRY tests/tmtest.cpp ./proto/*.pb.cc ./subsystems/*.cpp cdh.cpp utils.cpp crc32.c -o tmtest `pkg-config --cflags --libs protobuf`
	./tmtest && rm tmtest

tctest:
	g++ -std=gnu++11 -DTEST_MOCK_TELECOMMAND tests/tctest.cpp ./proto/*.pb.cc ./subsystems/*.cpp cdh.cpp utils.cpp crc32.c -o tctest `pkg-config --cflags --libs protobuf`
	./tctest && rm tctest
