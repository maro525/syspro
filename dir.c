/*
SFC授業課題
あるディレクトリ以下にあるすべてのファイルを表示する 
*/

#include <stdio.h>
#include <dirent.h>
#include <string.h>

void search_dir(char *dir)
{
    printf("\nFOLDER : %s\n", dir);
    
    DIR * dirp;
    struct dirent *dp;
    char filename[512];

    dirp = opendir(dir);
    if(dirp == NULL)
    {
        printf("NO DIR\n");
        return;
    }

    while((dp = readdir(dirp)) != NULL)
    {
        switch(dp->d_type)
        {
            case DT_DIR:
                if(strcmp(dp->d_name,".") == 0 || strcmp(dp->d_name,"..") == 0)
                    continue;

                printf("%s", dp->d_name);

                strcpy(filename, dir);
                strcat(filename, "/");
                strcat(filename, dp->d_name);
                search_dir(filename);
                
                break;
            
            case DT_REG:
                printf("%s\n", dp->d_name);
                break;
            
            default:
                break;
        }
    }
    closedir(dirp);
    return;
}

int main(int argc, char **argv)
{
    search_dir(argv[1]);
}