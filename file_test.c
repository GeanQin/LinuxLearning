#include <stdio.h>
#include <stdlib.h>
 
int main()
{
	char dataPtr[] = {'H','e','l','l','o','!','\n'};
	FILE *fpw = fopen("file_test","a");
	/*
	 * "r"	以“只读”方式打开文件。只允许读取，不允许写入。文件必须存在，否则打开失败。
	 * "w"	以“写入”方式打开文件。如果文件不存在，那么创建一个新文件；如果文件存在，那么清空文件内容（相当于删除原文件，再创建一个新文件）。
	 * "a"	以“追加”方式打开文件。如果文件不存在，那么创建一个新文件；如果文件存在，那么将写入的数据追加到文件的末尾（文件原有的内容保留）。
	 * "r+"	以“读写”方式打开文件。既可以读取也可以写入，也就是随意更新文件。文件必须存在，否则打开失败。
	 * "w+"	以“写入/更新”方式打开文件，相当于w和r+叠加的效果。既可以读取也可以写入，也就是随意更新文件。如果文件不存在，那么创建一个新文件；如果文件存在，那么清空文件内容
	 * "a+"	以“追加/更新”方式打开文件，相当于a和r+叠加的效果。既可以读取也可以写入，也就是随意更新文件。如果文件不存在，那么创建一个新文件；如果文件存在，那么将写入的数据追加到文件的末尾
	 */
	fwrite(dataPtr,sizeof(char),7,fpw);
	/*
	 * size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
	 * ptr -- 这是指向要被写入的元素数组的指针。
	 * size -- 这是要被写入的每个元素的大小，以字节为单位。
	 * nmemb -- 这是元素的个数，每个元素的大小为 size 字节。
	 * stream -- 这是指向 FILE 对象的指针，该 FILE 对象指定了一个输出流。
	 */
	fclose(fpw);
	
	FILE *fpo = fopen("file_test","r");
	char buffer[1024];
	int readCnt = fread(buffer,sizeof(buffer),1,fpo);
	/*
	 * size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
	 * ptr -- 这是指向带有最小尺寸 size*nmemb 字节的内存块的指针。
	 * size -- 这是要读取的每个元素的大小，以字节为单位。
	 * nmemb -- 这是元素的个数，每个元素的大小为 size 字节。
	 * stream -- 这是指向 FILE 对象的指针，该 FILE 对象指定了一个输入流。
	 */
	fclose(fpo);
	printf("%s\n",buffer);
	return 0;	
}
