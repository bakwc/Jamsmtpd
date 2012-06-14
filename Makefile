jamsmtpd: main.o accounts.o account.o misc.o letter.o log.o params.o server.o
	g++ -std=c++0x main.o accounts.o account.o misc.o letter.o log.o params.o server.o -o jamsmtpd

main.o: main.cpp params.h log.h accounts.h server.h
	g++ -std=c++0x -c main.cpp -o main.o

accounts.o: accounts.cpp accounts.h account.h misc.h exception.h
	g++ -std=c++0x -c accounts.cpp -o accounts.o

account.o: account.cpp account.h misc.h exception.h
	g++ -std=c++0x -c account.cpp -o account.o

misc.o: misc.cpp misc.h exception.h
	g++ -std=c++0x -c misc.cpp -o misc.o

log.o: log.cpp log.h params.h exception.h
	g++ -std=c++0x -c log.cpp -o log.o

params.o: params.cpp params.h misc.h exception.h
	g++ -std=c++0x -c params.cpp -o params.o

letter.o: letter.cpp letter.h misc.h accounts.h account.h exception.h
	g++ -std=c++0x -c letter.cpp -o letter.o

server.o: server.cpp server.h accounts.h params.h log.h letter.h
	g++ -std=c++0x -c server.cpp -o server.o

clean:
	rm -rf *.o
	rm -rf tests
