
CC:=x86_64-w64-mingw32-gcc

WARNINGS:=  -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-declarations \
            -Wredundant-decls -Winline -Wno-long-long \
            -Wuninitialized #-Wconversion

FLAGS:=-O0 -g $(WARNINGS)

CFLAGS:=-ffreestanding
LDFLAGS:=-nostdlib -Wl,-dll -shared -Wl,--subsystem,10

LIBS:=-lgcc

SRC_DIR:=src
INCLUDE_DIR:=include

GNU_EFI_INC:=/home/wouter/workspace/gnu-efi-3.0.11/inc
GNU_EFI_INC_X86_64:=$(GNU_EFI_INC)/x86_64
GNU_EFI_INC_PROT:=$(GNU_EFI_INC)/protocol

INCLUDES:= -I$(INCLUDE_DIR) -I$(GNU_EFI_INC) -I$(GNU_EFI_INC_X86_64) -I$(GNU_EFI_INC_PROT)

OS_EFI?=BOOTX64.efi
ENTRY_POINT:=efi_main

C_OBJS:=$(shell find ./$(SRC_DIR) -name *.c -print)
OBJS:=$(C_OBJS:.c=.o)

.PHONY: all install clean

all: $(OS_EFI)

$(OS_EFI): $(OBJS)
	$(CC) -o $@ $(FLAGS) $(LDFLAGS) -e $(ENTRY_POINT) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) -MD -c $< -o $@ $(INCLUDES) $(FLAGS) $(CFLAGS)

install: $(OS_EFI)

clean:
	rm -f $(OS_EFI)
	rm -f $(OBJS)
	rm -f $(OBJS:.o=.d)

-include $(OBJS:.o=.d)
