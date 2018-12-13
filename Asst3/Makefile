all: bankingServer bankingClient

bankingServer: bankingServer.c bankingServer.h utils.c
	gcc -g -Wall -Werror -fsanitize=address -lpthread -o bankingServer bankingServer.c utils.c

bankingClient: bankingClient.c bankingClient.h utils.c
	gcc -g -Wall -Werror -fsanitize=address -lpthread -o bankingClient bankingClient.c utils.c

clean: 
	rm -rf bankingServer bankingClient
