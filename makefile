include config.mk

SOURCES=$(addprefix $(SRCDIR)/,$(SRC))
OBJECTS=$(SOURCES:.c=.o)

all: $(BIN)

install: all
	install -D -s $(BIN) $(DESTDIR)$(BINDIR)/$(BIN)

uninstall:
	-rm -f $(BINDIR)/$(BIN)

clean:
	-rm -f $(OBJECTS)
	-rm -f $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BIN) $(LDLIBS) $(OBJECTS)

.PHONY: all install uninstall clean
