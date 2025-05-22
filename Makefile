.PHONY: all clean

CXXFLAGS += -O3 -Wall -Wextra -Wpedantic -Werror -std=c++20 \
	-I./include -I./solid-require/include

LIB = liblinereader.a
L_SOURCES = line-reader.cpp
L_OBJECTS = $(L_SOURCES:.cpp=.o)
L_HEADER = include/line-reader/line-reader.h

all: $(LIB)

line-reader.o: include/line-reader/line-reader.h

%.o:%.cpp
	@echo "  buliding" $@
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB): $(L_OBJECTS) $(L_HEADER)
	@echo building $@
	@$(AR) -cr $@ $^

clean:
	@echo remove temporaries
