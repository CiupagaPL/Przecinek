NAME = Przecinek.o
INC = Include/*
LIB = Lib/*
SRC = *.c
CSFML = -lcsfml-audio -lcsfml-graphics -lcsfml-system -lcsfml-window

all:
	g++ $(SRC) -o$(NAME) -I${INC} -L${LIB} ${CSFML}

run:
	./$(NAME)

clean:
	rm -r *.o
