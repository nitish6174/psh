### Method 1
# psh: src/builtin.c src/history.c src/main.c src/print_format.c src/process.c src/read_line.c src/split_line.c src/header.h
# 	gcc -o psh src/builtin.c src/history.c src/main.c src/print_format.c src/process.c src/read_line.c src/split_line.c src/header.h -Wall

### Method 2 (More efficient)
psh: builtin.o history.o main.o print_format.o process.o read_line.o split_line.o
	gcc -o psh builtin.o history.o main.o print_format.o process.o read_line.o split_line.o
builtin.o: src/builtin.c src/header.h
	gcc -c src/builtin.c src/header.h
history.o: src/history.c src/header.h
	gcc -c src/history.c src/header.h
main.o: src/main.c src/header.h
	gcc -c src/main.c src/header.h
print_format.o: src/print_format.c src/header.h
	gcc -c src/print_format.c src/header.h
process.o: src/process.c src/header.h
	gcc -c src/process.c src/header.h
read_line.o: src/read_line.c src/header.h
	gcc -c src/read_line.c src/header.h
split_line.o: src/split_line.c src/header.h
	gcc -c src/split_line.c src/header.h