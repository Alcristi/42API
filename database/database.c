#include "../include/42api.h"

int check_query(int query,MYSQL *con)
{
    if(query)
    {
        printf("Query failed with the following message:\n");
        printf("%s\n",mysql_error(con));
        return (1);
    }
    else
        return (0);
}

int mysql_check_cpf(char * cpf, MYSQL *con)
{
    MYSQL_ROW row;
    MYSQL_RES *result;
    char sql_statement[2048];
    int number_of_rows;
	int i;

	i = 0;
	sprintf(sql_statement,"SELECT * FROM cadete WHERE cpf = %s",cpf);
	if (mysql_query(con,sql_statement) != 0)
    if (check_query(mysql_query(con,sql_statement),con))
        return (0);
	result = mysql_store_result(con);
	if (mysql_num_rows(result) == 0)
	{
		mysql_free_result(result);
		return (1);
	}
	else
	{
		mysql_free_result(result);
		return (0);
	}
}

char *mysql_get(char* id,MYSQL *con)
{
    
    MYSQL_ROW row;
    MYSQL_RES *result;
    char sql_statement[2048];
	char *json;
	char *aux;
    int number_of_rows;
	int i;

	i = 0;
	if (id)
		sprintf(sql_statement,"SELECT id, name,lastname,data_nasc,cpf,dd,phone,email FROM cadete WHERE id = %s",id);
	else
		sprintf(sql_statement,"SELECT id, name,lastname,data_nasc,cpf,dd,phone,email FROM cadete");
	if (check_query(mysql_query(con,sql_statement),con))
        return (NULL);
    result = mysql_store_result(con);
	if (!mysql_num_rows(result))
		return (NULL);
    row = mysql_fetch_row(result);
	json = strdup("");
    while(row != NULL)
    {
		aux = malloc(sizeof(char)*2048);
		if (mysql_num_rows(result)!= 1)
		{
			if (i == 0)
				sprintf(aux,"[\n {\n   \"id\": %s,\n   \"name\": \"%s\",\n   \"lastname\": \"%s\",\n   \"data nascimento\": \"%s\",\n   \"cpf\": \"%s\",\n   \"dd\": %s,\n   \"phone\": \"%s\",\n   \"email\": \"%s\"\n },\n",row[0],row[1],row[2],row[3],row[4],row[5],row[6],row[7]);
			else if( i == mysql_num_rows(result) - 1)
				sprintf(aux," {\n   \"id\": %s,\n   \"name\": \"%s\",\n   \"lastname\": \"%s\",\n   \"data nascimento\": \"%s\",\n   \"cpf\": \"%s\",\n   \"dd\": %s,\n   \"phone\": \"%s\",\n   \"email\": \"%s\"\n }\n]",row[0],row[1],row[2],row[3],row[4],row[5],row[6],row[7]);
			else
				sprintf(aux," {\n   \"id\": %s,\n   \"name\": \"%s\",\n   \"lastname\": \"%s\",\n   \"data nascimento\": \"%s\",\n   \"cpf\": \"%s\",\n   \"dd\": %s,\n   \"phone\": \"%s\",\n   \"email\": \"%s\"\n },\n",row[0],row[1],row[2],row[3],row[4],row[5],row[6],row[7]);
			i++;
		}
		else
			sprintf(aux,"{\n   \"id\": %s,\n   \"name\": \"%s\",\n   \"lastname\": \"%s\",\n   \"data nascimento\": \"%s\",\n   \"cpf\": \"%s\",\n   \"dd\": %s,\n   \"phone\": \"%s\",\n   \"email\": \"%s\"\n}",row[0],row[1],row[2],row[3],row[4],row[5],row[6],row[7]);
		json = ft_strjoin(json,aux);
		free(aux);
        row = mysql_fetch_row(result);
    }
    free(row);
	mysql_free_result(result);
    mysql_close(con);
	return (json);
}

char * mysql_post(t_model_user user,MYSQL *con)
{
    MYSQL_ROW row;
    MYSQL_RES *result;
    char sql_statement[2048];
	char *json;
	int i;

	i = 0;
	if(mysql_check_cpf(user.cpf,con))
		sprintf(sql_statement,"INSERT INTO cadete (name,lastname,data_nasc,cpf,dd,phone,email) VALUES('%s','%s','%s',%s, %d, %s,'%s')",
		user.name,user.lastname,user.data_nascimento,user.cpf,user.dd,user.phone,user.email);
	else
	{
		mysql_close(con);
		return(strdup("{\n \"Cadete já cadastrado\" \n}"));
	}
	if (check_query(mysql_query(con,sql_statement),con))
        return (NULL);
	sprintf(sql_statement,"SELECT id, name FROM cadete WHERE cpf = '%s'",user.cpf);
	if (check_query(mysql_query(con,sql_statement),con))
        return (NULL);
	json = malloc(sizeof(char)*100);
    result = mysql_store_result(con);
	row = mysql_fetch_row(result);
    if(row)
	    sprintf(json,"{\n   \"id\": %s,\n   \"name\": \"%s\" \n}",row[0],row[1]);
	mysql_free_result(result);
    mysql_close(con);
	return (json);
}

void create_mysql()
{
	MYSQL *con;
	MYSQL_RES *result;
	MYSQL_ROW row;
	char* aux;
	static int flag;

	con = mysql_init(NULL);
	if (con == NULL)
	{
		fprintf(stderr,"%s\n",mysql_error(con));
		exit(1);
	}
	if(mysql_real_connect(con,"localhost","Admin","admin1234",NULL,0,NULL,0) == NULL)
	{
		fprintf(stderr,"%s\n",mysql_error(con));
		mysql_close(con);
		exit(1);
	}
	if(check_query(mysql_real_query(con,"CREATE DATABASE IF NOT EXISTS 42api",strlen("CREATE DATABASE IF NOT EXISTS 42api")),con)) 
		exit(1);
    if(check_query(mysql_real_query(con,"USE 42api",strlen("USE 42api")),con)) 
		exit(1);
	aux = strdup("CREATE TABLE IF NOT EXISTS cadete (id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,name VARCHAR(30) NOT NULL,lastname VARCHAR(30),data_nasc DATE NULL, cpf CHAR(11) NOT NULL, dd INT NOT NULL, phone CHAR(9) NOT NULL,email VARCHAR(40) NOT NULL)");
    if(check_query(mysql_real_query(con,aux,strlen(aux)),con))
        exit(1);
	free(aux);
	if (check_query(mysql_real_query(con,"SELECT * FROM cadete WHERE id = 1",strlen("SELECT * FROM cadete WHERE id = 1")),con))
		exit(1);
	result = mysql_store_result(con);
	if(mysql_num_rows(result) == 0)
	{
		aux = strdup("INSERT cadete VALUES(1,'Allan','Nascimento','1999-05-15','20030050020',81,'100200300','alcristi@gmail.com')");
    	if (check_query(mysql_real_query(con,aux,strlen(aux)),con))
        	exit(1);	
		free(aux);
	}
	mysql_free_result(result);
    mysql_close(con);
}

MYSQL *init_my_sql()
{
    MYSQL * con;

    con = mysql_init(NULL);
    if(mysql_real_connect(con,"localhost","Admin","admin1234","42api",3306,NULL,0) == NULL)
    {
        printf("The authentication failed with the following message\n");
        printf("%s %d\n",mysql_error(con),mysql_errno(con));
    	exit(1);
    }
    return (con);
}
