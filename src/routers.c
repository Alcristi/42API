#include "../include/42api.h"

t_model_user model(resquest *hm)
{
	t_model_user date;
	double dd;
	char buff[100];
	int a;
	a = mjson_get_string(hm->body.ptr,hm->body.len,"$.name",buff,100);
	if(a < 0)
		date.name = strdup("");
	else
		date.name = strdup(buff);
	a = mjson_get_string(hm->body.ptr,hm->body.len,"$.lastname",buff,100);
	if(a < 0)
		date.lastname = strdup("");
	else
		date.lastname = strdup(buff);
	a = mjson_get_string(hm->body.ptr,hm->body.len,"$.data nascimento",buff,100);
	if(a < 0)
		date.data_nascimento = strdup("");
	else
		date.data_nascimento = strdup(buff);
	a = mjson_get_string(hm->body.ptr,hm->body.len,"$.cpf",buff,100);
	if(a < 0)
		date.cpf = strdup("");
	else
		date.cpf = strdup(buff);
	a = mjson_get_number(hm->body.ptr,hm->body.len,"$.dd",&dd);
	if(a < 0)
		date.dd = 0;
	else
		date.dd = (int)dd;
	a = mjson_get_string(hm->body.ptr,hm->body.len,"$.phone",buff,100);
	if(a < 0)
		date.phone = strdup("");
	else
		date.phone = strdup(buff);
	a = mjson_get_string(hm->body.ptr,hm->body.len,"$.email",buff,100);
	if(a < 0)
		date.email = strdup("");
	else
		date.email = strdup(buff);
	return (date);
}

char *get_id(resquest *hm)
{
    char **tmp;
    int i;
    int id;
	char *id_char;
	char *info;

    i = 0;
	info = ft_substrsplit(hm->uri.ptr,0,hm->uri.len);
    tmp = ft_split_long(info,'/');
	while(tmp[i])
		i++;
    if(i!= 3)
    {
		i = 0;
		free(info);
        while(tmp[i])
            free(tmp[i++]);
        free(tmp);
        return(0);
    }
    else
    {
		 i = 0;
		id_char = strdup(tmp[2]);
		free(info);
        while(tmp[i])
            free(tmp[i++]);
        free(tmp);
        return(id_char);
    }
}

void router_user(connection *c, resquest *hm,MYSQL *con)
{
    char *id;
	char *json;
	char *method;

	method = ft_substrsplit(hm->method.ptr,0,hm->method.len);
	json = NULL;
	if(!strcmp(method,"GET"))
	{
    	if (mg_http_match_uri(hm, "/api/user/*"))
    	{ 
    		id = get_id(hm);
			json = mysql_get(id,con);
			if(json)
    	    	mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s",json);
			else
				mg_http_reply(c, 404, strdup(""), "Not Found");
			free(id);
    	}
    	else
		{
			json = mysql_get(NULL,con);
    	    mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", json);
		}
	}
	else
	{
		mg_http_reply(c, 400, NULL, "Cannot %.*s %.*s",(int)hm->method.len,hm->method.ptr,(int)hm->uri.len,hm->uri.ptr);
		mysql_close(con);
	}
	if(json)
		free(json);
	free(method);
}

void free_model(t_model_user* cadete)
{
	free(cadete->name);
	free(cadete->lastname);
	free(cadete->data_nascimento);
	free(cadete->cpf);
	free(cadete->phone);
	free(cadete->email);
}

void router_register(connection *c,resquest *hm,MYSQL *con)
{
	char *method;
	char *json;
	t_model_user user;
	method = ft_substrsplit(hm->method.ptr,0,hm->method.len);
	json = NULL;
	if(!strcmp(method,"POST"))
	{
		user = model(hm);
		json = mysql_post(user,con);
		mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s",json);
	}
	else
	{
		mg_http_reply(c, 400, NULL, "Cannot %.*s %.*s",(int)hm->method.len,hm->method.ptr,(int)hm->uri.len,hm->uri.ptr);
		mysql_close(con);
	}
	if(json)
	{
		free_model(&user);
		free(json);
	}
	free(method);
}