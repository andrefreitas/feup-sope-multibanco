all: server client admin

server: server.o account.o logs.o request.o
	@echo Assembling $^
	@mkdir -p bin
	@gcc obj/server*.o obj/account.o obj/logs.o obj/request.o -I./include/ -Wall -o bin/$@

client: client.o account.o request.o 
	@echo Assembling $^
	@mkdir -p bin
	@gcc obj/client*.o obj/account.o obj/request.o -I./include/ -Wall -o bin/$@

admin: admin.o account.o
	@echo Assembling $^
	@mkdir -p bin
	@gcc obj/admin*.o obj/account.o -I./include/ -Wall -o bin/$@
	
%.o:
	@echo Compiling $(basename $@).c
	@mkdir -p obj
	@gcc src/$(basename $@).c -I./include/ -D_REENTRANT -lpthread -Wall -c
	@mv *.o obj/

clean:
	@rm -r bin obj
