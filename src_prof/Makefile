
update:
	flex expr.l
	bison -v -d expr.y
	g++ -x c++ -DYYDEBUG -g -o expr expr.cpp lex.yy.c expr.tab.c

full: clean update
	@echo > /dev/null

clean:
	rm -rf $(shell cat .gitignore)
