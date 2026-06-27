CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -Isrc -Isrc/generated

# Dependency flags and libraries
DEPS_DIR = deps
PROTO_DIR = proto
GEN_DIR = src/generated

# When building from source, include paths must point to the deps/ directories
CXXFLAGS += -I$(DEPS_DIR)/grpc/include -I$(DEPS_DIR)/wasm-micro-runtime/core/iwasm/include \
            -I$(DEPS_DIR)/cpp-libp2p/include -I$(DEPS_DIR)/librdkafka/src

# For now, we stub out the actual linking since full builds of gRPC/WAMR/libp2p/librdkafka
# take extensive time and memory. The user requested they be added to the Makefile to build from source.
# We will create phony targets to build these dependencies.
LDFLAGS = -L$(DEPS_DIR)/grpc/cmake/build -L$(DEPS_DIR)/wasm-micro-runtime/product-mini/platforms/linux/build \
          -L$(DEPS_DIR)/cpp-libp2p/build -L$(DEPS_DIR)/librdkafka/src \
          -lgrpc++ -lgrpc -lprotobuf -lrdkafka++ -lrdkafka -lvmlib -llibp2p -pthread

# Protoc and gRPC plugin paths (use the ones built from source in deps/grpc)
PROTOC = $(DEPS_DIR)/grpc/cmake/build/third_party/protobuf/protoc
GRPC_CPP_PLUGIN_PATH = $(DEPS_DIR)/grpc/cmake/build/grpc_cpp_plugin

PROTO_SRCS = $(wildcard $(PROTO_DIR)/*.proto)
PROTO_GEN_SRCS = $(patsubst $(PROTO_DIR)/%.proto, $(GEN_DIR)/%.pb.cc, $(PROTO_SRCS)) \
                 $(patsubst $(PROTO_DIR)/%.proto, $(GEN_DIR)/%.grpc.pb.cc, $(PROTO_SRCS))
PROTO_GEN_OBJS = $(PROTO_GEN_SRCS:.cc=.o)

SRCS = src/main.cpp \
       src/workload/DummyTask.cpp \
       src/engine/MockExecutionEngine.cpp \
       src/node_agent/MockNodeAgent.cpp \
       src/scheduler/BasicScheduler.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = nebula_compute

TEST_SRCS = tests/test_main.cpp \
            src/workload/DummyTask.cpp \
            src/engine/MockExecutionEngine.cpp \
            src/node_agent/MockNodeAgent.cpp \
            src/scheduler/BasicScheduler.cpp

TEST_OBJS = $(TEST_SRCS:.cpp=.o)
TEST_TARGET = run_tests

.PHONY: all clean test run deps grpc-deps wamr-deps libp2p-deps kafka-deps generate-proto

all: deps $(TARGET)

# --- Dependency Management ---

$(DEPS_DIR):
	mkdir -p $(DEPS_DIR)

grpc-deps: | $(DEPS_DIR)
	@echo "Cloning and building gRPC..."
	@if [ ! -d "$(DEPS_DIR)/grpc" ]; then \
		git clone --recurse-submodules -b v1.54.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc $(DEPS_DIR)/grpc; \
	fi
	@mkdir -p $(DEPS_DIR)/grpc/cmake/build
	@cd $(DEPS_DIR)/grpc/cmake/build && cmake ../.. -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF && make -j4

wamr-deps: | $(DEPS_DIR)
	@echo "Cloning and building WAMR..."
	@if [ ! -d "$(DEPS_DIR)/wasm-micro-runtime" ]; then \
		git clone --depth 1 https://github.com/bytecodealliance/wasm-micro-runtime $(DEPS_DIR)/wasm-micro-runtime; \
	fi
	@mkdir -p $(DEPS_DIR)/wasm-micro-runtime/product-mini/platforms/linux/build
	@cd $(DEPS_DIR)/wasm-micro-runtime/product-mini/platforms/linux/build && cmake .. && make -j4

libp2p-deps: | $(DEPS_DIR)
	@echo "Cloning and building cpp-libp2p..."
	@if [ ! -d "$(DEPS_DIR)/cpp-libp2p" ]; then \
		git clone --depth 1 https://github.com/libp2p/cpp-libp2p $(DEPS_DIR)/cpp-libp2p; \
	fi
	@mkdir -p $(DEPS_DIR)/cpp-libp2p/build
	@cd $(DEPS_DIR)/cpp-libp2p/build && cmake .. -DTESTING=OFF && make -j4

kafka-deps: | $(DEPS_DIR)
	@echo "Cloning and building librdkafka..."
	@if [ ! -d "$(DEPS_DIR)/librdkafka" ]; then \
		git clone --depth 1 https://github.com/confluentinc/librdkafka $(DEPS_DIR)/librdkafka; \
	fi
	@cd $(DEPS_DIR)/librdkafka && ./configure && make -j4

deps: grpc-deps wamr-deps libp2p-deps kafka-deps

# --- Protocol Buffers Generation ---

$(GEN_DIR):
	mkdir -p $(GEN_DIR)

$(GEN_DIR)/%.pb.cc $(GEN_DIR)/%.pb.h: $(PROTO_DIR)/%.proto | $(GEN_DIR)
	$(PROTOC) -I$(PROTO_DIR) --cpp_out=$(GEN_DIR) $<

$(GEN_DIR)/%.grpc.pb.cc $(GEN_DIR)/%.grpc.pb.h: $(PROTO_DIR)/%.proto | $(GEN_DIR)
	$(PROTOC) -I$(PROTO_DIR) --grpc_out=$(GEN_DIR) --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

generate-proto: $(PROTO_GEN_SRCS)

# --- Compilation Rules ---

$(TARGET): $(PROTO_GEN_OBJS) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(PROTO_GEN_OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)
	rm -rf $(GEN_DIR)
