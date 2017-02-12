all: run

run:schadal1.c
	gcc schadal1.c -o schadal1
	./schadal1
clean:
	rm -f schadal1 .*~
