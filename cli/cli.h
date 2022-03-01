#ifndef CLI_H
# define CLI_H
# include <stdio.h>
# include <time.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include<unistd.h>
# include<stdlib.h>

typedef struct  infoLog
{
    char    *methodHttp;
    char    *path;
    char    *host;
    char    *time;
	char  	*bodyResquest;
	int 	bodylen;
	int 	status;
    int     count_resquest;
    int     numberMethod;
}infoLog;

char	*ft_substrsplit(char const *s, unsigned int start, size_t len);
char	**ft_split_long(char const *s, char c);
char	*ft_strjoin(char *s1, char *s2);
size_t	count_letters(char const *s, char c);
char	*ft_strnstr(const char *str, const char *to_find, size_t n);
int lenghtRequest(char* file);
void readFile(int fd,char** file);
int fuc_log(infoLog *info, char *request);
void print_table(infoLog *info, int totalRequest);
void print_table_data(infoLog *info, int totalRequest,char* data);
void print_table_method(infoLog *info,int totalRequest, char* method);
void print_table_path(infoLog *info,int totalRequest, char* path);
#endif