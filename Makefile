## "Group Project"
## Compiler for Linux

NAME = out/GroupProject
INC = include/*
LIB = lib/linux/*
SRC = src/*
OUT = out/*
CSFML = -lcsfml-audio -lcsfml-graphics -lcsfml-system -lcsfml-window

all:
	g++ $(SRC) -o$(NAME) -I${INC} -L${LIB} ${CSFML}

clean:
	rm -r $(OUT)
