# psh: src/builtin.c src/history.c src/main.c src/print_format.c src/process.c src/read_line.c src/split_line.c src/header.h
# 	gcc -o psh src/builtin.c src/history.c src/main.c src/print_format.c src/process.c src/read_line.c src/split_line.c src/header.h
psh: out/builtin.o out/history.o out/main.o out/print_format.o out/process.o out/read_line.o out/split_line.o
	gcc -o psh out/builtin.o out/history.o out/main.o out/print_format.o out/process.o out/read_line.o out/split_line.o
out/builtin.o: src/builtin.c src/header.h
	gcc -c src/builtin.c src/header.h
out/history.o: src/history.c src/header.h
	gcc -c src/history.c src/header.h
out/main.o: src/main.c src/header.h
	gcc -c src/main.c src/header.h
out/print_format.o: src/print_format.c src/header.h
	gcc -c src/print_format.c src/header.h
out/process.o: src/process.c src/header.h
	gcc -c src/process.c src/header.h
out/read_line.o: src/read_line.c src/header.h
	gcc -c src/read_line.c src/header.h
out/split_line.o: src/split_line.c src/header.h
	gcc -c src/split_line.c src/header.h