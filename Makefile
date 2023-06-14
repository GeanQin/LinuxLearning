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

# yum install mariadb-server
# yum install mariadb-devel
# git clone git@github.com:DaveGamble/cJSON.git
# yum install python3-devel
# pip3 install mysqlclient
db_test:
	gcc -g db_test.c -lmysqlclient -lcjson -L/usr/lib64/mysql/

signal_test:
	gcc -g signal_test.c

ffmpeg_test:
	gcc ffmpeg_test.c -lavformat -lavcodec -lavutil -lpthread -lm -lz -L/root/tool/ffmpeg-4.2.9/install/lib -I/root/tool/ffmpeg-4.2.9/install/include

clean:
	rm -f a.out
	rm -rf *test
	rm -rf *.so
