
ARCH?=x86_64

CC:=$(ARCH)-elf-gcc

SRC_DIR:=src
ARCH_DIR:=arch
INCLUDE_DIR:=include

export TARGET_DIR=$(ARCH_DIR)/$(ARCH)

SYSROOT?=$(shell pwd)


include $(TARGET_DIR)/make.config

WARNINGS:=  -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-declarations \
            -Wredundant-decls -Winline -Wno-long-long \
            -Wuninitialized #-Wconversion

FLAGS:=-O0 -g -ffreestanding --sysroot=$(SYSROOT) -isystem=$(INCLUDE_DIR) $(WARNINGS) $(KERNEL_ARCH_FLAGS)
CFLAGS:= $(KERNEL_ARCH_CFLAGS)
LDFLAGS:= $(KERNEL_ARCH_LDFLAGS)

LIBS:= -nostdlib -lgcc $(KERNEL_ARCH_LIBS)

INCLUDES:= -I$(INCLUDE_DIR)

BINARY_FILE:=flux.bin
LINKER?=linker.ld

C_OBJS:=$(shell find ./$(SRC_DIR) -name *.c -print)
KERNEL_OBJS:=$(C_OBJS:.c=.o) $(KERNEL_ARCH_OBJS)

PREFIX_ENTRIES?=
SUFFIX_ENTRIES?=

OBJS=\
$(PREFIX_ENTRIES) \
$(KERNEL_OBJS) \
$(SUFFIX_ENTRIES) \

LINK_LIST=\
$(LDFLAGS) \
$(PREFIX_ENTRIES) \
$(KERNEL_OBJS) \
$(LIBS) \
$(SUFFIX_ENTRIES) \

.PHONY: all install clean

all: $(OBJS) $(TARGET_DIR)/$(LINKER)

$(BINARY_FILE): $(OBJS) $(TARGET_DIR)/$(LINKER)
	$(CC) -T $(TARGET_DIR)/$(LINKER) -o $@ $(FLAGS) $(LINK_LIST)

$(TARGET_DIR)/crtbegin.o $(TARGET_DIR)/crtend.o:
	@OBJ=`$(CC) $(FLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

%.o: %.c
	$(CC) -MD -c $< -o $@ $(FLAGS) $(CFLAGS)

install: $(BINARY_FILE)

clean:
	@rm -f $(OBJS)
	@rm -f $(OBJS:.o=.d)

-include $(OBJS:.o=.d)
