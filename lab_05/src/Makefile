CFLAGS := -pthread


app1: app1.o
	gcc -o $@.exe  $^ $(CFLAGS)
	./$@.exe


app1_threads: app1_threads.o
	gcc -o $@.exe  $^ $(CFLAGS)
	./$@.exe


app2: app2.o
	gcc -o $@.exe  $^ $(CFLAGS)
	./$@.exe


app2_threads: app2_threads.o
	gcc -o $@.exe  $^ $(CFLAGS)
	./$@.exe


app2_threads_right: app2_threads_right.o
	gcc -o $@.exe  $^ $(CFLAGS)
	./$@.exe


app3: app3.o
	gcc -o $@.exe  $^ $(CFLAGS)
	./$@.exe


app3_threads: app3_threads.o
	gcc -o $@.exe  $^ $(CFLAGS)
	./$@.exe


%.o: %.c
	gcc -c $^


clean:
	rm -f *.o *.exe src/*.o
