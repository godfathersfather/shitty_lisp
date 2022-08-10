CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wno-unused-result
TARGET := target/lisp
OUT := out/*.o

all: build run

build: main builtin token node error data text
	$(CXX) $(OUT) -o $(TARGET) $(CXXFLAGS)

run:
	./$(TARGET) script.lisp

main:
	$(CXX) -c $(CXXFLAGS) src/$@.cc -o out/$@.o

token:
	$(CXX) -c $(CXXFLAGS) src/$@.cc -o out/$@.o

node:
	$(CXX) -c $(CXXFLAGS) src/$@.cc -o out/$@.o

error:
	$(CXX) -c $(CXXFLAGS) src/$@.cc -o out/$@.o

data:
	$(CXX) -c $(CXXFLAGS) src/$@.cc -o out/$@.o

text:
	$(CXX) -c $(CXXFLAGS) src/$@.cc -o out/$@.o

builtin:
	$(CXX) -c $(CXXFLAGS) src/$@.cc -o out/$@.o

clean:
ifneq ("$(wildcard $(OUT))", "")
	rm -f $(OUT)
endif

ifneq ("$(wildcard $(TARGET))", "")
	rm -f $(TARGET)
endif
