TARGET  := libephyscripts.so
PREFIX  ?= /usr
DESTDIR ?= lib/epiphany/web-extensions
CFLAGS  ?= -O2 -pipe -march=native -Wall -Wextra
LDFLAGS ?= -s
CFLAGS  += $(shell pkg-config --cflags webkit2gtk-web-extension-4.0)
LDFLAGS += $(shell pkg-config --libs webkit2gtk-web-extension-4.0)

$(TARGET): $(wildcard *.c)
	$(CC) -fPIC $(CFLAGS) -shared $(LDFLAGS) -o $@ $^

install: $(TARGET)
	install -D $^ $(PREFIX)/$(DESTDIR)/$(TARGET)

uninstall:
	rm -f $(PREFIX)/$(DESTDIR)/$(TARGET)
