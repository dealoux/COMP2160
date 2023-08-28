CC = clang
CFLAGS = -g -Wall

PROG = unit_test
HDRS = table.h
SRCS = main.c table.c

OBJDIR = object
OBJS = $(OBJDIR)/main.o $(OBJDIR)/table.o

# compiling rules

# WARNING: *must* have a tab before each definition
$(PROG): $(OBJS) $(OBJDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJDIR)/table.o: table.c $(HDRS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o: main.c $(HDRS) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -f $(PROG) $(OBJS)



