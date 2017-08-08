CFLAGS = -Wall -pedantic -Werror
LFLAGS = -lm -lSDL2 -lSDL2_image -lSDL_mixer -lSDL_ttf
OBJS   = main.o GameCore.o
FOLDER = src
PROG   = SoapGamesDawn_v1.0
CC     = g++

# top-level rule to create the program.
all: $(PROG)

# compiling other source files.
%.o: $(FOLDER)/%.cpp $(FOLDER)/%.h $(FOLDER)/defines.h $(FOLDER)/includes.h $(FOLDER)/structs.h
	$(CC) $(CFLAGS) -c $<

# linking the program.
$(PROG): $(OBJS)
	$(CC) $(OBJS) -o $(PROG) $(LFLAGS)

# cleaning everything that can be automatically recreated with "make".
clean:
	rm -f $(OBJS)
