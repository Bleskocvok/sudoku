
CXXFLAGS = -std=c++17 -pedantic -Wall -Wextra -O2 -DNDEBUG
# CXXFLAGS = -std=c++17 -pedantic -Wall -Wextra -g

TARGET = sudoku

SRC = main.cpp

OBJ    = $(SRC:.cpp=.o)
DEPEND = $(OBJ:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

%.o: CXXFLAGS += -MMD -MP

-include $(DEPEND)


clean:
	$(RM) $(OBJ) $(DEPEND)

distclean: clean
	$(RM) $(TARGET)

.PHONY: all clean distclean


