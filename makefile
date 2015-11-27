include config.mk

SOURCES=$(addprefix $(SRCDIR)/,$(SRC))
OBJECTS=$(SOURCES:.c=.o)

all: $(NAME)

install: all
	install -D -s $(NAME) $(BINDIR)/$(NAME)

uninstall:
	-rm -f $(BINDIR)/$(NAME)

clean:
	-rm -f $(OBJECTS)
	-rm -f $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(LDLIBS) $(OBJECTS)

.PHONY: all install uninstall clean
