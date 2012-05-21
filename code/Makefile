all: server client admin

server: server.o account.o logs.o
	@echo Assembling $^
	@mkdir -p bin
	@gcc obj/server*.o obj/account.o obj/logs.o -I./include/ -Wall -o bin/$@

client: client.o
	@echo Assembling $^
	@mkdir -p bin
	@gcc obj/client*.o -I./include/ -Wall -o bin/$@

admin: admin.o
	@echo Assembling $^
	@mkdir -p bin
	@gcc obj/admin*.o -I./include/ -Wall -o bin/$@
	
%.o:
	@echo Compiling $(basename $@).c
	@mkdir -p obj
	@gcc src/$(basename $@).c -I./include/ -Wall -c
	@mv *.o obj/

clean:
	@rm -r bin obj
