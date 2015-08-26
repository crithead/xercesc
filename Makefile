CC = g++
CXXFLAGS = -Wall --std=c++11

SOURCES := parser.cpp
SOURCES += handler.cpp

OBJECTS := $(SOURCES:.cpp=.o)
DEPENDS := $(SOURCES:.cpp=.d)

EXEC = parser

LIB = -lxerces-c

all: $(EXEC) $(TEST)

$(EXEC): $(OBJECTS) $(DEPENDS)
	$(CC) $(CXXFLAGS) -o $(EXEC) $(OBJECTS) $(LIB)

%.d : %.cpp
	$(CC) -MM -E $< -o $@

-include $(DEPENDS)

.PHONY: print
print:
	@echo "SOURCES: $(SOURCES)"
	@echo "OBJECTS: $(OBJECTS)"
	@echo "DEPENDS: $(DEPENDS)"

.PHONY: clean
clean:
	@rm -f *.o *.d
	@rm -f $(EXEC)


