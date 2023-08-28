CC = clang
CFLAGS = -g -Wall

PROG = tester
HDRS = set.h
SRCS = main.c set.c

OBJDIR = object
OBJS = $(OBJDIR)/main.o $(OBJDIR)/set.o

# compiling rules

# WARNING: *must* have a tab before each definition
$(PROG): $(OBJS) $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG)

$(OBJDIR)/set.o: set.c $(HDRS) $(OBJDIR)
	$(CC) $(CFLAGS) -c set.c -o $(OBJDIR)/set.o

$(OBJDIR)/main.o: main.c $(HDRS) $(OBJDIR)
	$(CC) $(CFLAGS) -c main.c -o $(OBJDIR)/main.o

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -f $(PROG) $(OBJS)



