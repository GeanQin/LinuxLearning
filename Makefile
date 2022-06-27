file_test:
	gcc -g file_test.c

mm_test:
	gcc -g mm_test.c

pthread_test:
	gcc -g pthread_test.c -lpthread

socket_test:
	gcc -g socket_client_test.c -o client_test
	gcc -g socket_server_test.c -o server_test

dlopen_test:
	gcc lib_test.c -fPIC -shared -o lib_test.so
	gcc dlopen_test.c -ldl

fork_test:
	gcc -g fork_test.c

clean:
	rm -f a.out
	rm -rf *test
	rm -rf *.so
