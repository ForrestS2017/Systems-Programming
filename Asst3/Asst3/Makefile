bankingServer:
	gcc -g -Wall -Werror -fsanitize=address -lpthread -o bankingServer bankingServer.c

bankingClient:
	gcc -g -Wall -Werror -fsanitize=address -lpthread -o bankingClient bankingClient.c

clean: 
	rm -rf bankingServer bankingClient
