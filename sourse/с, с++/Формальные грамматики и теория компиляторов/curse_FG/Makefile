
all:
	bison -d grammar.y
	flex lex.l
	cc lex.yy.c grammar.tab.c -o out
	./out tests/test.c

clean:
	rm grammar.tab.c grammar.tab.h lex.yy.c out

files:
	./out files_for_test/derevo.c
	./out files_for_test/example.c
	./out files_for_test/Kursach_optimiz.c
	./out files_for_test/perevod.c
	./out files_for_test/polyndrom.c
	./out files_for_test/prime.c
	./out files_for_test/progressiya.c
	./out files_for_test/Source-1.cpp
	./out files_for_test/derevo.cpp
	./out files_for_test/357.cpp

test:
	./out tests/test1.c
	echo 
	./out tests/tricky_enum1.c
	echo
	./out tests/tricky_comments1.c 
	echo
	./out tests/numbers_binary_hex.c
	echo
	./out tests/combined_tricks.c
	echo
	./out tests/test2.c
	echo
	./out tests/test_pointer.c