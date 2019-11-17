
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = game

LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

INSTALL_TOP = C:/SFML
INSTALL_INC = -I $(INSTALL_TOP)/include
INSTALL_LIB = -L $(INSTALL_TOP)/lib $(LIBS)

CFLAGS = -w -g -DSFML_STATIC $(INSTALL_INC) $(INSTALL_LIB) 
LDFLAGS = -w $(CFLAGS)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	g++ $(OBJECTS) $(LDFLAGS) -o $@
 
.cpp.o:
	del  *.o &  g++ $(CFLAGS) -c $<