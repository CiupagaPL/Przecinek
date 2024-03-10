NAME = Out/Przecinek
INC = Include/*
LIB = Lib/*
SRC = Src/*
OUT = Out/*
CSFML = -lcsfml-audio -lcsfml-graphics -lcsfml-system -lcsfml-window

all:
	g++ $(SRC) -o$(NAME) -I${INC} -L${LIB} ${CSFML}

clean:
	rm -r $(OUT)
