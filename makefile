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

all: $(OBJS) ext/libgff | lib
	gcc $(CFLAGS) $(OBJS) $(LIBS) -shared -o lib/libsoloscuro.so

ext/libgff:
	git submodules pull

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
