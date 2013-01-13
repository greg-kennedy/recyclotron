CXX = g++
CXXFLAGS = -O2 -Wall -fomit-frame-pointer -funroll-loops -I. `allegro-config --cppflags release`
LDFLAGS = `allegro-config --libs release`
SOURCES = main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
NAME = recyclotron

all: $(SOURCES) $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

clean:
	rm -f *.o $(NAME)

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@
