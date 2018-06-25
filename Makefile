CXXFLAGS += -std=c++11 -O3

SOURCES := $(shell find . -name '*.cpp')

HEADERS := $(shell find . -name '*.h')

OUTPUT := wordcounter

all: $(OUTPUT)

$(OUTPUT): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS)  -o $(OUTPUT) $(SOURCES)

clean:
	$(RM) $(OUTPUT)	

