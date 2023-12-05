EXT = cpp
CXX=g++
CFLAGS=-Wall -I/usr/local/include
LDFLAGS= -lGL -lGLU -lGLEW -lglut

SRCDIR = src
BINDIR = bin

SOURCES=$(wildcard $(SRCDIR)/*.$(EXT))
OBJECTS=$(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SOURCES))

DEBUBFLAG=-g
EXEC=$(BINDIR)/main


all: $(EXEC)

$(EXEC): $(OBJECTS)
		$(CXX) -g -o $@ $^ $(LDFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
		@mkdir -p $(BINDIR)
		$(CXX) -g -o $@ -c $< $(CFLAGS)

clean:
		rm -rf $(BINDIR)

mrproper: clean
		rm -rf $(EXEC)