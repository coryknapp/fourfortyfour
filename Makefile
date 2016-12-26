#
# Makefile
# cknapp, 2016-05-23 13:10
#

all:
	clang++ -std=c++14 -g\
		-ferror-limit=1000\
		-o main\
		Analyzer.cpp\
		Tokenizer.cpp\
		Validator.cpp\
		Node.cpp\
		main.cpp


# vim:ft=make
#

