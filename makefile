CC=gcc
PROGRAMNAME=main
SRCDIR=src
OBJDIR=build
CFLAGS=-Wall -g -no-pie
LFLAGS=
OBJNAMES=memalloc.o main.o
# Concatenação de arquivos de objeto com diretório de build
OBJS=$(foreach OBJ, $(OBJNAMES),  $(OBJDIR)/$(OBJ)) 

all: $(PROGRAMNAME) teste

$(PROGRAMNAME): $(OBJS)
	$(CC) -o $(PROGRAMNAME) $(OBJS) $(CFLAGS) $(LFLAGS)

teste: $(OBJDIR)/teste.o $(OBJDIR)/memalloc.o
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.s $(SRCDIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@ 

run: $(PROGRAMNAME)
	./$(PROGRAMNAME)

clean:
	rm -rf $(OBJDIR)/*.o

purge: clean
	rm -f $(PROGRAMNAME)
