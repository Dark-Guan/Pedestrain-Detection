
PROJECT_NAME="Pedstrain Detection"

# CC=gcc
# CC=arm-anykav200-linux-uclibcgnueabi-gcc

all:
	@echo "Project $(PROJECT_NAME) make all"
	gcc -o peopleDetect *.c -lm
debug:
	@echo "Project $(PROJECT_NAME) make all"
	gcc -g -o peopleDetect *.c -lm
ipc:
	@echo "Project $(PROJECT_NAME) make ipc"
	arm-anykav200-linux-uclibcgnueabi-gcc -o peopleDetect *.c -lm
clean:
	@echo "Project $(PROJECT_NAME) make clean"
	rm -rf *.o
	rm -rf peopleDetect
