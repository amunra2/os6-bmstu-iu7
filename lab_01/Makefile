CFLAGS := -pthread
FILE_O := $(subst .c,.o,$(wildcard lib/*.c))
HEADERS_DIR := lib/

app.exe:  main.o $(FILE_O)
	gcc -o app.exe $^  $(CFLAGS)

%.o: %.c
	gcc -c -I $(HEADERS_DIR) $^

lib/%.o: lib/%.c
	gcc -o $@ -c -I $(HEADERS_DIR) $^ $(CFLAGS)

clean:
	rm -f *.o *.exe src/*.o

run: app.exe
	sudo ./app.exe


getDaemonId:
	sudo cat /var/run/daemon.pid


daemonInfo:
	@echo "\nInput daemon id (call -- make getDaemonId): "; \
	read daemonId; \
	ps -ajx | head -1 && ps -ajx | grep $$daemonId
	

printDaemonLog:
	cat /var/log/syslog | grep "my_daemon"


killDaemon:
	@echo "\nInput daemon id (call -- make getDaemonId): "; \
	read daemonId; \
	sudo kill -15 $$daemonId


callSIGHUP:
	@echo "\nInput daemon id (call -- make getDaemonId): "; \
	read daemonId; \
	sudo kill -1 $$daemonId
	