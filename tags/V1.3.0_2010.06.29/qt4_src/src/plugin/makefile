#PC
CC=gcc 
#CC=/opt/gnuarm-3.4.3/bin/arm-elf-gcc

# embed dev
#CC=/opt/gnuarm-3.4.3/bin/arm-elf-gcc 

CFLAGS = -I/usr/loacl/include -I../../include -g -fvisibility=hidden
# No linux System  use -D__NOLINUX__
#CC=/opt/gnuarm-3.4.3/bin/arm-elf-gcc -D__NOLINUX__

so: clean npunix.o plugin.o
	$(CC) $(CFLAGS)  -shared    -o mdolphin_c_plugin.so plugin.o npunix.o   
plugin.o: 
	$(CC) $(CFLAGS)  -c plugin.c
npunix.o: 
	$(CC) $(CFLAGS)  -c npunix.c
clean:
	rm -f *.o *.so sdk 

