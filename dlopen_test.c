#include <stdio.h>
#include <dlfcn.h>

#define PATH "./lib_test.so"

typedef int(* FUNC_HELLO)();

int main(){
	/*
	 * 打开一个so文件 
	 * void *dlopen(const char *filename, int flags);
	 * filename：动态库文件名
	 * flags：
	 * 	RTLD_LAZY 暂缓决定，等有需要时再解出符号 
	 * 	RTLD_NOW 立即决定，返回前解除所有未决定的符号
	 * 返回一个句柄。
	 */
	void *handle = dlopen(PATH, RTLD_LAZY);
	/*
	 * 可以获取函数地址，也可以获取变量地址
	 * void *dlsym(void *handle, const char *symbol);
	 * handle：由dlopen打开动态链接库后返回的指针
	 * symbol：要求获取的函数或全局变量的名称
	 * void* 指向函数的地址，供调用使用
	 */
	FUNC_HELLO hello = (FUNC_HELLO)dlsym(handle, "hello");
	hello();
	dlclose(handle);
	return 0;
}
