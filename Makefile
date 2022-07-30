
CXXFLAGS = -std=c++17 -pedantic -Wall -Wextra -O2 -DNDEBUG

TARGET = sudoku
SRC = main.cpp
OBJ    = $(SRC:.cpp=.o)

CXXTEST = -std=c++17 -pedantic -Wall -Wextra -O2

TEST = test
T_SRC = test.cpp
T_OBJ = $(T_SRC:.cpp=.o)

DEPEND = $(OBJ:.o=.d) $(T_OBJ:.o=.d)

all: $(TARGET)

$(TEST): $(T_OBJ)
	$(CXX) -o $@ $^

$(TEST): CXXFLAGS = $(CXXTEST)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

%.o: CXXFLAGS += -MMD -MP

-include $(DEPEND)


clean:
	$(RM) $(OBJ) $(T_OBJ) $(DEPEND)

distclean: clean
	$(RM) $(TARGET) $(TEST)

.PHONY: all clean distclean


