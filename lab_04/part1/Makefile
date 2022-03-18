CFLAGS := -fPIC
FILE_O := $(subst .c,.o,$(wildcard lib/*.c))
HEADERS_DIR := lib/

app.exe:  main.o  $(FILE_O)
	gcc -o app.exe $^  $(CFLAGS)

%.o: %.c
	gcc -c -I $(HEADERS_DIR) $^

lib/%.o: lib/%.c
	gcc -o $@ -c -I $(HEADERS_DIR) $^ $(CFLAGS)

clean:
	rm -f *.o *.exe src/*.o

run: app.exe
	sudo ./app.exe result.txt
