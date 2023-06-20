SRC = BadLoginMessage.h BadLoginRegistr.h Demo_perms.h User.h User.cpp Message.h Message.cpp Chat.h Chat.cpp main.cpp

chat : $(SRC)
	g++ -std=c++17 -o chat $(SRC)

install:
	install ./chat /usr/local/bin

uninstall:
	rm - rf /usr/local/bin/chat
