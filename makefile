psh: src/builtin.c src/history.c src/main.c src/print_format.c src/process.c src/read_line.c src/split_line.c src/header.h
	gcc -o psh src/builtin.c src/history.c src/main.c src/print_format.c src/process.c src/read_line.c src/split_line.c src/header.h -Wall
