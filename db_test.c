#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <cjson/cJSON.h>

static MYSQL mysql;

int m_mysql_init()
{
    mysql_init(&mysql);
    printf("vresion = %s\n", mysql_get_client_info());

    if (mysql_real_connect(&mysql, "localhost", "root", "root.806", "gean_iot_server", 0, NULL, 0) == NULL)
    {
        printf("mysql connect error, %s\n", mysql_error(&mysql));
        return -1;
    }
    printf("connect success\n");
    mysql_set_character_set(&mysql, "utf8");

    return 0;
}

int m_mysql_cmd(char *cmd, char *ret_json, int ret_size)
{
    int ret = 0;
    MYSQL_RES *result;

    ret = mysql_real_query(&mysql, cmd, (unsigned int)strlen(cmd));
    if (!ret)
    {
        printf("cmd success\n");
    }
    else
    {
        printf("cmd failed\n");
    }

    result = mysql_store_result(&mysql);
    if (result == NULL)
    {
        printf("result null\n");
    }
    else{
        int num_fields = mysql_num_fields(result);
        MYSQL_ROW row;
        cJSON* cjson_test = cJSON_CreateObject();
        cJSON* cjson_all = cJSON_CreateArray();

        int i = 0;
        while ((row = mysql_fetch_row(result)))
        {
            cJSON* cjson_array = cJSON_CreateArray();
            for (i = 0; i < num_fields; i++)
            {
                // printf("%s ", row[i] ? row[i] : "NULL");
                if (row[i])
                {
                    cJSON_AddItemToArray(cjson_array, cJSON_CreateString(row[i]));
                }
                else
                {
                    cJSON_AddItemToArray(cjson_array, cJSON_CreateString("NULL"));
                }
            }

            cJSON_AddItemToArray(cjson_all, cjson_array);
            // printf("\n");
        }
        cJSON_AddItemToObject(cjson_test, "get", cjson_all);
        printf("%s\n", cJSON_Print(cjson_test));

        cJSON_Delete(cjson_test);
        mysql_free_result(result);
    }
}

void m_mysql_deinit()
{
    mysql_close(&mysql); // 关闭数据库
}

int main()
{
    m_mysql_init();

    char *data = "insert into client_pid(did, pid) values('3456789abcde', 5561)";
    // printf("data = %s\n", data);

    // m_mysql_cmd(data, NULL, 0);

    data = "select * from client_pid";
    printf("data = %s\n", data);
    m_mysql_cmd(data, NULL, 0);

    m_mysql_deinit();

    return 0;
}