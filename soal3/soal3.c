#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <wait.h>
#include <dirent.h>
#include <pthread.h>

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

// void moveFile()

void categorize(char* filePath,char* curPath)
{
    // printf("%s %s masuk categorize\n",filePath,curPath);
    char buff[510];
    strcpy(buff,filePath);
    char* fileName = strrchr(filePath,'/');
    printf("%s\n",fileName);
    memmove(&fileName[0],&fileName[1],strlen(fileName) - 0);
    if (fileName[0] == '.')
    {
        // puts("masuk sini");
        mkdir("hidden",0777);
        strcat(curPath,"/");
        strcat(curPath,"hidden/");
        strcat(curPath,fileName);
        printf("%s %s\n",curPath,buff);
        rename(buff,curPath);
        return;
    }
    char buff2[510];
    strcpy(buff2,fileName);
    char* extension = strrchr(buff2,'.');
    memmove(&extension[0],&extension[1],strlen(extension) - 0);
    if (strlen(extension) > 0)
    {
        mkdir(extension,0777);
        strcat(curPath,"/");
        strcat(curPath,extension);
        strcat(curPath,"/");
        strcat(curPath,fileName);
        printf("%s %s\n",curPath,buff);
        rename(buff,curPath);
    }
    else
    {
        mkdir("unknown",0777);
        strcat(curPath,"/");
        strcat(curPath,"unknown/");
        strcat(curPath,fileName);
        printf("%s %s\n",curPath,buff);
        rename(buff,curPath);
    }
}

pthread_t id[510];

struct arguments
{
    char filePath[2010];
    char curPath[2010];
};

void *categoryThread(void *ptr)
{
    puts("masuk");
    struct arguments params = *(struct arguments*)ptr;
    printf("%s %s ini di fungsi thread\n",params.filePath,params.curPath);
    categorize(params.filePath,params.curPath);
    pthread_exit(0);
}

int ix;

void listFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            if (dp->d_type == DT_REG)
            {
                struct arguments params;
                strcpy(params.filePath,path);
                strcpy(params.curPath,"/home/pan/sisop/soal-shift-sisop-modul-3-A10-2021");
                if (strcmp(path,"/home/pan/sisop/soal-shift-sisop-modul-3-A10-2021/soal3.c") != 0)
                {
                    pthread_create(&id[ix],NULL,categoryThread,(void*)&params);
                    sleep(1);
                    ix++;
                }
            }
            else
            if (dp->d_name[0] != '.')
                listFilesRecursively(path);
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[])
{
    char cwd[2010];
    getcwd(cwd,sizeof(cwd));
    if (strcmp(argv[1],"-f") == 0)
    {
        for (int i = 2 ; i < argc ; i++)
        {
            struct arguments params;
            strcpy(params.filePath,argv[i]);
            strcpy(params.curPath,cwd);
            // printf("%d -> %s\n",i,argv[i]);
            // printf("%s %s\n",params.filePath,params.curPath);
            // puts("==========================\n");
            if (is_regular_file(argv[i]))
            {
                pthread_create(&id[ix],NULL,categoryThread,(void*)&params);
                printf("File %d: Berhasil Dikategorikan\n", i - 1);
                sleep(1);
                ++ix;
            }
            else
                printf("File %d: Sad, gagal :(\n",i - 1);
        }
        for (int i = 0 ; i < ix ; i++)
            pthread_join(id[i],NULL);
    }
    else
    if (strcmp(argv[1],"-d") == 0)
    {
        puts("masuk");
        listFilesRecursively(argv[2]);
    }
    else
    if (strcmp(argv[1],"-*") == 0)
    {
        puts("masuk");
        listFilesRecursively("/home/pan/sisop/soal-shift-sisop-modul-3-A10-2021");
    }
    return 0;
}