CFLAGS=-O3
INCLUDE=-Iinclude -Ibuild

main: parser src/backend/latex.c
	gcc -o build/main \
		src/utils.c \
		src/backend/latex.c \
		build/parser.l.c \
		build/parser.y.c \
		$(INCLUDE) $(CFLAGS)

parser: src/parser/parser.l src/parser/parser.y
	flex -o build/parser.l.c src/parser/parser.l
	bison -d -o build/parser.y.c src/parser/parser.y
