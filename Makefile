# These could be changed to make some settings
CC = gcc
CFLAGS = -c -Wall
LDFLAGS =
INSTALL_PATH = /usr/bin
MAN_PATH = /usr/share/man/man1

# Below this point no changes should be needed
SOURCES = fork.c
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = fork
MANPAGE = fork.1

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)

install: $(EXECUTABLE) $(MANPAGE)
	cp $(EXECUTABLE) $(INSTALL_PATH)
	cp $(MANPAGE) $(MAN_PATH)

uninstall:
	rm $(INSTALL_PATH)/$(EXECUTABLE)
	rm $(MAN_PATH)/$(MANPAGE)

