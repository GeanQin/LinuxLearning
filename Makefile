file_test:
	gcc -g file_test.c

mm_test:
	gcc -g mm_test.c

pthread_test:
	gcc -g pthread_test.c -lpthread

clean:
	rm -f a.out
	rm -rf *test
