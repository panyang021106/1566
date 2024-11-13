LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL -lc

all:  mouse

mouse: mouse_lecture.c initShader.o myLib.o
	gcc -o mouse mouse_lecture.c initShader.o myLib.o $(LIBS)

# triangle: triangle.c initShader.o myLib.o
# 	gcc -o triangle triangle.c initShader.o myLib.o $(LIBS)

# test: test.c myLib.o
# 	gcc -o driver test.c myLib.o -lm

myLib.o: myLib.c myLib.h
	gcc -fno-stack-protector -c myLib.c

initShader.o: initShader.c initShader.h
	gcc -c initShader.c

clean:
	rm -f *.o  myLib.o
