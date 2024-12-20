LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL -lc

all:  mouse

mouse: mouse_lecture.c initShader.o myLib.o maze.o
	gcc -g -o mouse mouse_lecture.c initShader.o myLib.o maze.o $(LIBS)

# triangle: triangle.c initShader.o myLib.o
# 	gcc -o triangle triangle.c initShader.o myLib.o $(LIBS)

# test: test.c myLib.o
# 	gcc -o driver test.c myLib.o -lm
test_lib: test_lib.o myLib.o
	gcc -o test_lib test_lib.o myLib.o -lm

myLib.o: myLib.c myLib.h
	gcc  -c myLib.c


maze.o: maze.c maze.h
	gcc -c maze.c

initShader.o: initShader.c initShader.h
	gcc -c initShader.c

clean:
	rm -f *.o  myLib.o maze.o initShader.o
