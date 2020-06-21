.phony: all

all: test

test: path_test.c
	@ccsm --output-metrics=HIS_PATH --output-format=csv path_test.c 2> /dev/null | grep expected

ast: path_test.c
	 clang -Xclang -ast-dump -fsyntax-only path_test.c
