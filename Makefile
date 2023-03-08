all:
	arm-linux-gcc ./src/*.c -o ./bin/shop -I ./inc/ -L ./lib -lfont
clean:
	rm ./bin/shop