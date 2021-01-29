file_test:
	gcc -g file_test.c

mm_test:
	gcc -g mm_test.c

pthread_test:
	gcc -g pthread_test.c -lpthread

socket_test:
	gcc -g socket_client_test.c -o client_test
	gcc -g socket_server_test.c -o server_test

clean:
	rm -f a.out
	rm -rf *test
