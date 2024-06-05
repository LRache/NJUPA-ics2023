CC=gcc
ISA=riscv32

examples.txt:
	python3 gen_expr.py 1000 > examples.txt

test: test_expr.out examples.txt 
	./test_expr.out ./examples.txt

test_expr.out: test_expr.c ../sdb/expr.c
	$(CC) ./test_expr.c ../sdb/expr.c -o test_expr.out -I$(NEMU_HOME)/include/generated -I$(NEMU_HOME)/include -I$(NEMU_HOME)/src/isa/$(ISA)/include -D__GUEST_ISA__=$(ISA)  -Werror
