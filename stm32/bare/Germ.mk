include $(STM32)/Settings.mk


AOPS = --warn --fatal-warnings 
#COPS =  -nostdlib -nostartfiles -ffreestanding -I$(CRUNKY)



$(IMG) : $(HEX)

LIB_CRUNKY = libcrunky.o

$(LIB_CRUNKY) : $(CRUNKY_OBJS)

all : $(IMG)

clean :
	rm -f *.o *.bin *.hex *.elf *.list *.img *.a $(BUILT)


%.o : %.c
	$(CC) $(COPS) -c $^ -o $@

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

LINKER = $(STM32)/linker.ld


#XCEPT = -L /usr/lib/arm-none-eabi/newlib -lsupc++
NEWLIB = -L/usr/lib/arm-none-eabi/newlib/hard/ -lc_nano

$(ELF) : $(LINKER) $(OBJS)  $(LIBUSPI) $(CRUNKY)/vectors.o
	$(LD) $(CRUNKY)/vectors.o   $(OBJS) -T $(LINKER)  -L$(CRUNKY) -lcrunky $(NEWLIB) -o $@
	$(OBJDUMP) -D $@ > $(KERNEL).list

$(IMG) : $(ELF)
	$(OBJCOPY) $(ELF) -O binary $@

$(HEX) : $(ELF)
	$(OBJCOPY) $^ -O ihex $(HEX)

#flash : install

#install:
#	cp $(IMG) /media/pi/50BD-6FEC
