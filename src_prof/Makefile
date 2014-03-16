
.PHONY: update full clean

CXXFLAGS= -x c++ -DYYDEBUG -g

update: expr testexpr
	@echo > /dev/null

full: clean update
	@echo > /dev/null

clean:
	rm -rf $(shell cat .gitignore)

expr: expr.cpp expr.l expr.y expr.h
	flex expr.l
	bison -v -d expr.y
	g++ $(CXXFLAGS) -o $@ $(filter %.cpp,$^) lex.yy.c expr.tab.c

testexpr: testexpr.cpp expr.cpp expr.h
	g++ $(CXXFLAGS) -o $@ $(filter %.cpp,$^)
