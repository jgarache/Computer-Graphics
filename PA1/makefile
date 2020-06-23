
# CSE170 glutapp makefile v2.1 - M. Kallmann (not tested for a long time)

SOURCES := $(shell echo $(./)*.cpp) $(shell echo $(./)*/*.cpp)
DEPENDS = $(SOURCES:.cpp=.d)
OBJECTS = $(SOURCES:.cpp=.o)
OS = $(shell uname) # For Mac OS detection (by Rolando Yanez, Raymond Harris)
PROGRAM = glutapp

#######################################################################

# OPTFLAGS = -g  # for debug
OPTFLAGS =  -O3 -s
CC       = g++
CFLAGS   = -Wall -Wno-format $(OPTFLAGS) -I./ -std=c++11

ifeq ($(strip $(OS)),Darwin)
	LDFLAGS = -framework GLUT -framework OpenGL
else
	LDFLAGS = -lGL -lglut -lGLU -lGLEW
endif

#######################################################################

all: $(PROGRAM)

$(PROGRAM):$(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

clean:
	$(RM) $(OBJECTS) $(DEPENDS)
	$(RM) $(PROGRAM)
	$(RM) *~

#######################################################################

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

%.d: %.cpp
	$(CC) -MM $(CFLAGS) $< > $@

#######################################################################

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif

#######################################################################
