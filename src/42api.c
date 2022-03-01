#include "../include/42api.h"

static int s_signal;

static void storeLog(char *pathFile,resquest *hm,connection *c)
{
    int fd;
    time_t curtime;
    time(&curtime);
    fd =open(pathFile,O_WRONLY | O_APPEND | O_CREAT,0664);
    
    write(fd,ctime(&curtime),strlen(ctime(&curtime)));
    write(fd,hm->message.ptr,hm->message.len);
    if(!hm->body.len)
        write(fd,c->send.buf,13);
    else
    {
        write(fd,"\n",1);
        write(fd,c->send.buf,13);
    }
    write(fd,"\n",1);
    write(fd,"|||||||||||||||||||||||||||||||||||||||||||||\n",46);


    close(fd);

}

static void fn(connection *c, int ev, void *ev_data, void *fn_data)
{
    if(ev == MG_EV_HTTP_MSG)
    {
        MYSQL *con;

        con = init_my_sql();
        resquest *hm = (resquest *)ev_data;
        if (mg_http_match_uri(hm, "/api/user/*") || mg_http_match_uri(hm, "/api/user"))
            router_user(c,hm,con);
        else if(mg_http_match_uri(hm,"/api/register"))
            router_register(c,hm,con);
        else
        {
            resquest_static opts = {.root_dir = "./"};
            mg_http_serve_dir(c,ev_data,&opts);
            mysql_close(con);
        }
        storeLog("./log/logApi.txt",hm,c);
    }
}

void sig_int(int signo)
{
    printf("\n");
    s_signal = signo;
}

int main(void)
{
    serve mgr;

    create_mysql();
    signal(SIGINT,sig_int);
    signal(SIGTERM,sig_int);
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr,"http://localhost:8000",fn,&mgr);
    while(s_signal == 0)
        mg_mgr_poll(&mgr,1000);
    mysql_library_end();
    mg_mgr_free(&mgr);
}