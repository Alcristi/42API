#ifndef API_H
# define API_H
# include "./mongoose.h"
# include "./mjson.h"
# include <time.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include "./mysql.h"
# include "./mysqld_error.h"

typedef struct  mg_http_message resquest;
typedef struct  mg_mgr serve;
typedef struct  mg_connection connection;
typedef struct mg_http_serve_opts resquest_static;

typedef struct s_model_user
{
	int		id;
	char	*name;
	char	*lastname;
	char	*data_nascimento;
    char    *cpf;
	int		dd;
	char	*phone;
	char	*email;

}t_model_user;

size_t		count_letters(char const *s, char c);
char		**ft_split_long(char const *s, char c);
char		*ft_substrsplit(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char *s1, char *s2);
int      	mysql_check_cpf(char * cpf, MYSQL *con);
char    	*mysql_get(char* id, MYSQL *con);
char    	*mysql_post(t_model_user user, MYSQL *con);
void 		router_user(connection *c, resquest *hm, MYSQL *con);
void		router_register(connection *c,resquest *hm, MYSQL *con);
void 		create_mysql();
MYSQL 		*init_my_sql();
#endif