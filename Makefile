CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -Iinclude -Isrc

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
