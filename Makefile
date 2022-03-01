NAME = Api42
CLI_NAME = Read_log
CC = gcc

APIFILE = ./src/42api.c ./src/mongoose.c ./src/mjson.c ./database/database.c ./src/routers.c ./src/function_aux.c
OBJAPI = $(APIFILE:.c=.o) 

CLIFILE = ./cli/cli.c ./cli/function_aux.c
OBJCLI = $(CLIFILE:.c=.o) 

LIB = -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -ldl -lssl -lcrypto
INCLUDE = -I/usr/include/mysql


all: $(NAME) $(CLI_NAME)

$(NAME):$(OBJAPI)
	$(CC) $(INCLUDE) $(APIFILE) $(LIB) -o $(NAME)

$(CLI_NAME): $(OBJCLI)
	$(CC) $(CLIFILE) -o $(CLI_NAME)

clean:
	rm -f ./src/*.o ./lib/*.o  ./database/*.o ./cli/*.o

fclean:clean
	rm -f $(NAME) $(CLI_NAME)

re: fclean $(NAME) $(CLI_NAME)

.PHONY: all clean fclean install