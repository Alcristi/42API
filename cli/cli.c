#include "cli.h"

static void free_info(infoLog *info, int totalResquest)
{	
	int i;
	
	i = 0;
	while (i < totalResquest)
	{

		free(info[i].host);
		free(info[i].methodHttp);
		free(info[i].path);
		free(info[i].time);
		i++;
	}
}

int main(int argc, char *argv[])
{
    int 	fd;
    int 	i;
	int 	j;
    int 	total_request;
    char 	*file;
    char	**listRequest;
    infoLog *info;

	i = 0;
	j = -1;
    total_request = 0;
    fd = open("./log/logApi.txt", O_RDONLY);
    file =strdup(" ");
    if (fd < 0)
        exit(1);
    readFile(fd,&file);
    total_request = lenghtRequest(file)/2;
    info = malloc(total_request*sizeof(infoLog));
	listRequest = ft_split_long(file,'|');
	while(i < total_request)
	{
		fuc_log(&info[i],listRequest[i]);
		i++;
	} 
	if(argc == 1)
		print_table(info,total_request);
	else
	{
		if(!strcmp(argv[1],"-d"))
			print_table_data(info,total_request,argv[2]);
		else if(!strcmp(argv[1],"-m"))
			print_table_method(info,total_request,argv[2]);
		else if(!strcmp(argv[1],"-p"))
			print_table_path(info,total_request,argv[2]);
		else
		{
			printf("flag invalida!\n");
		}
	}
	for (i = 0; listRequest[i];i++)
		free(listRequest[i]);
	free(listRequest);
	free(file);
	free_info(info,total_request);
	free(info);
}
