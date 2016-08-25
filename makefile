CC=gcc
CFLAGS=-fPIC
LD=ld
LDFLAGS=
OBJ=pam_log_response.o
LIBS=-lpam
DESTDIR=/lib/*/security
TARGET=pam_log_response.so
CFGDIR=/etc/pam.d
CFGFILE=newsvc

%.o: %.c
	@$(CC) -c $^ $(CFLAGS)

$(TARGET): $(OBJ)
	@$(LD) -x --shared -o $@ $^ $(LDFLAGS) $(LIBS)

.PHONY: clean

clean:
	@rm -f *.o
	@rm -f $(TARGET)

.PHONY: install

install:
	@install -m 0644 $(TARGET) $(DESTDIR)
	@install -m 0644 $(CFGFILE) $(CFGDIR)

.PHONY: uninstall

uninstall:
	@-rm $(DESTDIR)/$(TARGET)
	@-rm $(CFGDIR)/$(CFGFILE)
