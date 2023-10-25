CC=gcc
ASMC=as
PROGRAMNAME=main
SRCDIR=src
OBJDIR=build
CFLAGS=-Wall -g -no-pie
LFLAGS=-lm
ASMFLAGS=-fPIC
OBJNAMES=memalloc.o main.o
OBJS=$(foreach OBJ, $(OBJNAMES),  $(OBJDIR)/$(OBJ))

all: $(PROGRAMNAME) teste

$(PROGRAMNAME): $(OBJS)
	$(CC) -o $(PROGRAMNAME) $(OBJS) $(CFLAGS) $(LFLAGS)

teste: build/teste.o build/memalloc.o
	$(CC) -o teste $^ $(CFLAGS) $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.s $(SRCDIR)/%.h
	$(CC) -c $< -o $@ $(CFLAGS)

run: $(PROGRAMNAME)
	./$(PROGRAMNAME)

clean:
	rm -rf $(OBJDIR)/*.o

purge: clean
	rm -f $(PROGRAMNAME)
