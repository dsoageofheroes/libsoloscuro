INCS = \
-I ext/libgff/include \
-I inc/

LIBS = \
-L ext/libgff/build/src/ \
-L ext/libgff/build/ext/ \
-lgff \
-lxmi2mid \
-lm

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=obj/%.o)

CFLAGS = $(INCS) -Wall -Werror -g -fsanitize=address -fPIC

all: ext/libgff/build/src/libgff.so $(OBJS) | lib
	gcc $(CFLAGS) $(OBJS) $(LIBS) -shared -o lib/libsoloscuro.so

ext/libgff/build/src/libgff.so:
	git submodule update --init
	cd ext/libgff ; make

lib:
	mkdir -p lib

obj:
	mkdir -p obj

obj/%.o: src/%.c inc/soloscuro/*.h | obj 
	gcc $(CFLAGS) -c $< -o $@

vi:
	 CPATH=`pwd`/inc/ vim src/main.c

clean:
	rm -rf obj lib
