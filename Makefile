CFILES = $(wildcard kernel/*.c)
OFILES = $(patsubst kernel/%.c, build/%.o, $(CFILES))

GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
GCCPATH = ../../gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin

all: build_dir kernel8.img

build_dir:
	mkdir -p build

build/boot.o: boot/boot.S | build_dir
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c boot/boot.S -o build/boot.o

build/%.o: kernel/%.c | build_dir
	$(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: build/boot.o $(OFILES)
	$(GCCPATH)/aarch64-none-elf-ld -nostdlib build/boot.o $(OFILES) -T link.ld -o build/kernel8.elf
	$(GCCPATH)/aarch64-none-elf-objcopy -O binary build/kernel8.elf build/kernel8.img

clean:
	/bin/rm -rf build > /dev/null 2> /dev/null || true

run:
	qemu-system-aarch64 -M raspi3b -kernel build/kernel8.img -d in_asm
