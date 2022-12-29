#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

int main()
{
    MYSQL mysql;
    mysql_init(&mysql);
    printf("vresion = %s\n", mysql_get_client_info());

    if (mysql_real_connect(&mysql, "localhost", "root", "root.806", "gean_iot_server", 0, NULL, 0) == NULL)
    {
        printf("mysql connect error, %s\n", mysql_error(&mysql));
        return -1;
    }
    printf("connect success\n");

    char *data = "insert into client_pid(did, pid) values('1234567890ab', 5555)";
    printf("data = %s\n", data);
    mysql_set_character_set(&mysql, "utf8");

    int flag = mysql_real_query(&mysql, data, (unsigned int)strlen(data)); // 插入
    if (!flag)
    {
        printf("insert success\n");
    }
    else
    {
        printf("insert failed\n");
    }

    data = "select * from client_pid where did='1234567890ab'";
    printf("data = %s\n", data);

    flag = mysql_real_query(&mysql, data, (unsigned int)strlen(data)); // 插入
    if (!flag)
    {
        printf("select success\n");
    }
    else
    {
        printf("select failed\n");
    }

    MYSQL_RES *result = mysql_store_result(&mysql);
    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    int i = 0;
    while ((row = mysql_fetch_row(result)))
    {
        for (i = 0; i < num_fields; i++)
        {
            printf("%s ", row[i] ? row[i] : "NULL");
        }

        printf("\n");
    }

    mysql_free_result(result);

    mysql_close(&mysql); // 关闭数据库

    return 0;
}