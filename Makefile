CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -pthread -Iinclude -Isrc

COMMON_SRCS = src/workload/DummyTask.cpp \
              src/engine/MockExecutionEngine.cpp \
              src/node_agent/MockNodeAgent.cpp \
              src/scheduler/BasicScheduler.cpp \
              src/verification/ConsensusVerificationEngine.cpp \
              src/billing/UsageBillingEngine.cpp \
              src/network/TcpNetworkEngine.cpp

SRCS = src/main.cpp $(COMMON_SRCS)
OBJS = $(SRCS:.cpp=.o)
TARGET = nebula_compute

TEST_SRCS = tests/test_main.cpp $(COMMON_SRCS)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)
TEST_TARGET = run_tests

.PHONY: all clean test run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)
