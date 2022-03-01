#include "cli.h"

const char head[5][18] = {{"Identificador"}, {"HTTP Method"}, {"Path"},{"Status"},{"Data"}};

size_t	count_letters(char const *s, char c)
{
	int	amount_of_letters;

	amount_of_letters = 0;
	while (s[amount_of_letters] != c && s[amount_of_letters])
		amount_of_letters++;
	return (amount_of_letters);
}

static size_t	count_words(char const *s, char c)
{
	int		total;
	int		i;

	i = 0;
	total = 0;
	while (s[i] && s[i] == c)
		i++;
	while (s[i])
	{
		while (s[i] && s[i] != c)
			i++;
		total++;
		while (s[i] && s[i] == c)
			i++;
	}
	return (total);
}

char	*ft_substrsplit(char const *s, unsigned int start, size_t len)
{
	size_t	size;
	size_t	i;
	char	*sub;

	if (!s)
		return (NULL);
	i = 0;
	size = 0;
	while (s[size])
		size++;
	if ((size + 1) < len + 1)
		sub = (char *)malloc(size + 1);
	else if (len == size)
		sub = (char *)malloc((len - start) + 1);
	else
		sub = (char *)malloc(len + 1);
	if (!sub)
		return (NULL);
	while ((i < len) && (start + i) < size)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

char	**ft_split_long(char const *s, char c)
{
	char	**list;
	size_t	c_words;
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	c_words = count_words(s, c);
	list = malloc(sizeof(char *) * (c_words + 1));
	if (!list)
		return (0);
	while (i < c_words && *s)
	{
		if (*s != c)
		{
			list[i] = ft_substrsplit(s, 0, count_letters(s, c));
			if (!list[i++])
				return (0);
			s += count_letters(s, c);
		}
		else
			s++;
	}
	list[c_words] = NULL;
	return (list);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*conc;
	size_t	i;
	size_t	size_s1;
	size_t	size_s2;

	i = -1;
	size_s1 = 0;
	size_s2 = 0;
	while (s1[size_s1])
		size_s1++;
	while (s2[size_s2])
		size_s2++;
	conc = malloc((size_s2 + size_s1 + 1) * sizeof(char));
	while (++i < size_s1)
		conc[i] = s1[i];
	i = -1;
	while (++i < size_s2)
		conc[i + size_s1] = s2[i];
	conc[size_s2 + size_s1] = '\0';
	free(s1);
	return (conc);
}

char	*ft_strnstr(const char *str, const char *to_find, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (*to_find == '\0')
		return ((char *)str);
	while (str[i] && i < n)
	{
		j = 0;
		while (to_find[j] && str[j + i] && (to_find[j] == str[j + i])
			&& (j + i) < n)
			j++;
		if (to_find[j] == '\0')
			return ((char *)&str[i]);
		i++;
	}
	return (NULL);
}

int lenghtRequest(char* file)
{
    int total_request;
    int i;

    total_request = 0;
    i = 0;
     while(file[i])
    {
        if(!strncmp(&file[i],"HTTP/1.1",strlen("HTTP/1.1")))
        {
            total_request++;
            i+=strlen("HTTP/1.1");
        }
        else
            i++;
    }
    return (total_request);
}

void readFile(int fd,char** file)
{
    char *tmp;

    tmp = strdup(" ");
    while(read(fd,tmp,1))
       file[0] = ft_strjoin(file[0],tmp);
    free(tmp);
}

int fuc_log(infoLog *info, char *request)
{
	int i;
	int j;
	int k;
    char *aux;
	char **tmp;
	i = 0;
	j = 0;
	k = 0;
	tmp = ft_split_long(request,'\n');
	while(tmp[i])
	{
		if (i == 0)
			info->time = strdup(tmp[i]);
		else if(i == 1)
		{
			j = count_letters(tmp[i],' ');
			info->methodHttp = ft_substrsplit(tmp[i],0,j++);
			k += j;
			j = count_letters(&tmp[i][j],' ');
			info->path = ft_substrsplit(tmp[i],k,j++);
		}
		else if(!strncmp(tmp[i],"Host:",strlen("Host:")))
		{
			k = strlen("Host: ");
			j = count_letters(&tmp[i][k],'\0');
			info->host = ft_substrsplit(tmp[i],k,j);
		}
		else if(!strncmp(tmp[i],"{",strlen("{")))
		{
			j = 0;
			k = 0;
			while(request[j] !=  '{')
			 	j++;
			while(request[k] != '}')
				k++;
			info->bodyResquest = ft_substrsplit(request,j,k - j + 1);
			info->bodylen = strlen(info->bodyResquest);
		}
		else if(!strncmp(tmp[i],"HTTP/1.1",strlen("HTTP/1.1")))
		{
			k = strlen("HTTP/1.1 ");
			j = count_letters(&tmp[i][k],'\0');
            aux = ft_substrsplit(tmp[i],k,j);
            info->status = atoi(aux);
            free(aux);
		}
		i++;
	}
	i = 0; 
	while(tmp[i])
		free(tmp[i++]);
	free(tmp);
	return(0);
}

static int tamanho_number(int i)
{
	int j;
	j = 1;
	while(i/=10)
		j++;
	return (j);
}

static int maiorPath(infoLog *info,int totalRequest)
{
	int maior;
	int i;

	i = -1;
	maior = 0;
	while(++i < totalRequest)
		if(maior < strlen(info[i].path))
			maior =  strlen(info[i].path);
	return(maior);
}

static int maiorData(infoLog *info, int totalRequest)
{
	int maior;
	int i;

	i = -1;
	maior = 0;
	while(++i < totalRequest)
		if(maior < strlen(info[i].time))
			maior =  strlen(info[i].time);
	return(maior);
}

void print_table(infoLog *info, int totalRequest)
{
	int i;
	int j;
	int p;
	int k;
	int space;
	int mData;
	int mPath;

	mData = maiorData(info,totalRequest);
	mPath = maiorPath(info,totalRequest);
	for(i = 0;i < totalRequest + 1;i++)
    {
		k = 0;
		p = 0;
        for(j = 0;j < 5;j++)
        {
			printf("\e[0m  |  ");
			if(i == 0)
			{
				if ( p == 2)
				{
					printf("\e4\e[31m\e[21m\e[1m%s",head[p]);
					space = mPath - strlen(head[p]);
					for(int k = 0;k < space;k++)
						printf(" ");
					
				}
				else if ( p == 4)
				{
					printf("\e[31m\e[21m\e[1m%s",head[p]);
					space = mData - strlen(head[p]);
					for(int k = 0;k < space;k++)
						printf(" ");
				}
				else
					printf("\e[31m\e[21m\e[1m%s",head[p]);
			}
			else
			{
				if (p == 0)
				{
					printf("\e[1m%d",i);
					space = strlen(head[p]) - tamanho_number(i);
				}
				else if(p == 1)
				{
					if(!strcmp(info[i - 1].methodHttp,"GET"))
						printf("\e[33m\e[1m%s",info[i - 1].methodHttp);
					else if(!strcmp(info[i - 1].methodHttp,"POST"))
						printf("\e[32m\e[1m%s",info[i - 1].methodHttp);
					else if(!strcmp(info[i - 1].methodHttp,"PUT"))
						printf("\e[35m\e[1m%s",info[i - 1].methodHttp);
					else if(!strcmp(info[i - 1].methodHttp,"DELETE"))
						printf("\e[31m\e[1m%s",info[i - 1].methodHttp);
					space = strlen(head[p]) - strlen(info[i - 1].methodHttp);
				}
				else if (p == 2)
				{
					printf("\e[36m\e[1m%s",info[i - 1].path);
					space = mPath - strlen(info[i - 1].path);
				}
				else if ( p == 3)
				{
					if(info[i - 1].status < 300)
						printf("\e[32m\e[1m%d",info[i - 1].status);
					else if(info[i - 1].status < 400)
						printf("\e[35m\e[1m%d",info[i - 1].status);
					else if(info[i - 1].status < 500)
						printf("\e[33m\e[1m%d",info[i - 1].status);
					space = strlen(head[p]) - tamanho_number(info[i - 1].status);
				}
				else 
				{
					printf("\e[1m%s",info[i - 1].time);
					space = mData - strlen(head[p]);
				}
				for(int k = 0;k < space;k++)
					printf(" ");
			}
			p++;
        }
        printf("\e[0m\n");
    }
}

void print_table_data(infoLog *info, int totalRequest,char* data)
{
	int i;
	int j;
	int p;
	int k;
	int space;
	int mData;
	int mPath;


	mData = maiorData(info,totalRequest);
	mPath = maiorPath(info,totalRequest);
	for(i = 0;i < totalRequest + 1;i++)
    {
		k = 0;
		p = 0;
		if (i > 0)
			if(!ft_strnstr(info[i - 1].time,data,12))
				continue;
        for(j = 0;j < 5;j++)
        {
			printf("\e[0m\e[1m  |  \e[0m");
			if(i == 0)
			{
				if ( p == 2)
				{
					printf("\e[31m\e[4m\e[1m%s",head[p]);
					space = mPath - strlen(head[p]);
					for(k = 0;k < space;k++)
						printf(" ");
					
				}
				else if ( p == 4)
				{
					printf("\e[31m\e[4m\e[1m%s",head[p]);
					space = mData - strlen(head[p]);
					for(k = 0;k < space;k++)
						printf(" ");
				}
				else
					printf("\e[31m\e[4m\e[1m%s",head[p]);
			}
			else
			{
				if(ft_strnstr(info[i - 1].time,data,12))
				{
					if (p == 0) //printf pos
					{
						printf("\e[1m%d",i);
						space = strlen(head[p]) - tamanho_number(i);
					}
					else if(p == 1) // printf method
					{
						if(!strcmp(info[i - 1].methodHttp,"GET"))
							printf("\e[33m\e[1m%s",info[i - 1].methodHttp);
						else if(!strcmp(info[i - 1].methodHttp,"POST"))
							printf("\e[32m\e[1m%s",info[i - 1].methodHttp);
						else if(!strcmp(info[i - 1].methodHttp,"PUT"))
							printf("\e[35m\e[1m%s",info[i - 1].methodHttp);
						else if(!strcmp(info[i - 1].methodHttp,"DELETE"))
							printf("\e[31m\e[1m%s",info[i - 1].methodHttp);
						space = strlen(head[p]) - strlen(info[i - 1].methodHttp);
					}
					else if (p == 2)//print path
					{
						printf("\e[36m\e[1m%s",info[i - 1].path);
						space = mPath - strlen(info[i - 1].path);
					}
					else if ( p == 3)//print status
					{
						if(info[i - 1].status < 300)
							printf("\e[32m\e[1m%d",info[i - 1].status);
						else if(info[i - 1].status < 400)
							printf("\e[35m\e[1m%d",info[i - 1].status);
						else if(info[i - 1].status < 500)
							printf("\e[33m\e[1m%d",info[i - 1].status);
						space = strlen(head[p]) - tamanho_number(info[i - 1].status);
					}
					else //print data
					{
						printf("\e[1m%s",info[i - 1].time);
						space = mData - strlen(head[p]);
					}
					for(k = 0;k < space;k++)
						printf(" ");
				}
			}
			p++;
        }
		if (k)
        	printf("\e[0m\n");
    }
}

void print_table_method(infoLog *info,int totalRequest, char* method)
{
	int i;
	int j;
	int p;
	int k;
	int space;
	int mData;
	int mPath;


	mData = maiorData(info,totalRequest);
	mPath = maiorPath(info,totalRequest);
	for(i = 0;i < totalRequest + 1;i++)
    {
		k = 0;
		p = 0;
		if (i > 0)
			if(strcmp(method,info[i - 1].methodHttp))
				continue;
        for(j = 0;j < 5;j++)
        {
			printf("\e[0m\e[1m  |  \e[0m");
			if(i == 0)
			{
				if (p == 2)
				{
					printf("\e[31m\e[4m\e[1m%s",head[p]);
					space = mPath - strlen(head[p]);
					for(k = 0;k < space;k++)
						printf(" ");
					
				}
				else if (p == 4)
				{
					printf("\e[31m\e[4m\e[1m%s",head[p]);
					space = mData - strlen(head[p]);
					for(k = 0;k < space;k++)
						printf(" ");
				}
				else
					printf("\e[31m\e[4m\e[1m%s",head[p]);
			}
			else
			{
				if (p == 0) //printf pos
				{
					printf("\e[1m%d",i);
					space = strlen(head[p]) - tamanho_number(i);
				}
				else if(p == 1) // printf method
				{
					if(!strcmp(info[i - 1].methodHttp,"GET"))
						printf("\e[33m\e[1m%s",info[i - 1].methodHttp);
					else if(!strcmp(info[i - 1].methodHttp,"POST"))
						printf("\e[32m\e[1m%s",info[i - 1].methodHttp);
					else if(!strcmp(info[i - 1].methodHttp,"PUT"))
						printf("\e[35m\e[1m%s",info[i - 1].methodHttp);
					else if(!strcmp(info[i - 1].methodHttp,"DELETE"))
						printf("\e[31m\e[1m%s",info[i - 1].methodHttp);
					space = strlen(head[p]) - strlen(info[i - 1].methodHttp);
				}
				else if (p == 2)//print path
				{
					printf("\e[36m\e[1m%s",info[i - 1].path);
					space = mPath - strlen(info[i - 1].path);
				}
				else if ( p == 3)//print status
				{
					if(info[i - 1].status < 300)
						printf("\e[32m\e[1m%d",info[i - 1].status);
					else if(info[i - 1].status < 400)
						printf("\e[35m\e[1m%d",info[i - 1].status);
					else if(info[i - 1].status < 500)
						printf("\e[33m\e[1m%d",info[i - 1].status);
					space = strlen(head[p]) - tamanho_number(info[i - 1].status);
				}
				else //print data
				{
					printf("\e[1m%s",info[i - 1].time);
					space = mData - strlen(head[p]);
				}
				for(k = 0;k < space;k++)
					printf(" ");
				
			}
			p++;
        }
		if (k)
        	printf("\e[0m\n");
    }
}

void print_table_path(infoLog *info,int totalRequest, char* path)
{
	int i;
	int j;
	int p;
	int k;
	int space;
	int mData;
	int mPath;


	mData = maiorData(info,totalRequest);
	mPath = maiorPath(info,totalRequest);
	for(i = 0;i < totalRequest + 1;i++)
    {
		k = 0;
		p = 0;
		if (i > 0)
			if(strcmp(path,info[i - 1].path))
				continue;
        for(j = 0;j < 5;j++)
        {
			printf("\e[0m\e[1m  |  \e[0m");
			if(i == 0)
			{
				if (p == 2)
				{
					printf("\e[31m\e[4m\e[1m%s",head[p]);
					space = mPath - strlen(head[p]);
					for(k = 0;k < space;k++)
						printf(" ");
					
				}
				else if (p == 4)
				{
					printf("\e[31m\e[4m\e[1m%s",head[p]);
					space = mData - strlen(head[p]);
					for(k = 0;k < space;k++)
						printf(" ");
				}
				else
					printf("\e[31m\e[4m\e[1m%s",head[p]);
			}
			else
			{
				if (p == 0) //printf pos
				{
					printf("\e[1m%d",i);
					space = strlen(head[p]) - tamanho_number(i);
				}
				else if(p == 1) // printf method
				{
					if(!strcmp(info[i - 1].methodHttp,"GET"))
						printf("\e[33m\e[1m%s",info[i - 1].methodHttp);
					else if(!strcmp(info[i - 1].methodHttp,"POST"))
						printf("\e[32m\e[1m%s",info[i - 1].methodHttp);
					else if(!strcmp(info[i - 1].methodHttp,"PUT"))
						printf("\e[35m\e[1m%s",info[i - 1].methodHttp);
					else if(!strcmp(info[i - 1].methodHttp,"DELETE"))
						printf("\e[31m\e[1m%s",info[i - 1].methodHttp);
					space = strlen(head[p]) - strlen(info[i - 1].methodHttp);
				}
				else if (p == 2)//print path
				{
					printf("\e[36m\e[1m%s",info[i - 1].path);
					space = mPath - strlen(info[i - 1].path);
				}
				else if ( p == 3)//print status
				{
					if(info[i - 1].status < 300)
						printf("\e[32m\e[1m%d",info[i - 1].status);
					else if(info[i - 1].status < 400)
						printf("\e[35m\e[1m%d",info[i - 1].status);
					else if(info[i - 1].status < 500)
						printf("\e[33m\e[1m%d",info[i - 1].status);
					space = strlen(head[p]) - tamanho_number(info[i - 1].status);
				}
				else //print data
				{
					printf("\e[1m%s",info[i - 1].time);
					space = mData - strlen(head[p]);
				}
				for(k = 0;k < space;k++)
					printf(" ");
				
			}
			p++;
        }
		if (k)
        	printf("\e[0m\n");
    }
}
