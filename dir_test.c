#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
 
void listFiles(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);
 
    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }
 
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        // Check if the entry is a directory and recursively list its contents
        if (entry->d_type == DT_DIR) {
            char subpath[512];
            printf("dir %s\n", entry->d_name);
            // snprintf(subpath, sizeof(subpath), "%s/%s", path, entry->d_name);
            // listFiles(subpath);
        }
        else printf("file %s\n", entry->d_name);
    
    }
 
    closedir(dir);
}
 
int main() {
    const char *folderPath = "."; // Change this to the desired folder path
    listFiles(folderPath);
 
    return 0;
}