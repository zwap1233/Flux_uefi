
MODULES:=uefi_loader

OVMF_DIR:=OVMF_dir
OVMF_FD:=OVMF.fd

export OS_EFI:=flux.efi
export OS_IMG:=flux.img
export OS_ISO:=flux.iso

#export SYSROOT:=$(shell pwd)/sysroot
#export INCLUDEDIR:=/include

.PHONY: all install iso qemu clean $(MODULES)

all: $(MODULES)

$(MODULES):
	@$(MAKE) -C $@ $(MAKECMDGOALS)

install: $(OS_IMG)

$(OS_IMG): $(MODULES)
	dd if=/dev/zero of=$@ bs=1k count=1440; \
	mformat -i $@ -f 1440 ::; \
	mmd -i $@ ::/EFI; \
	mmd -i $@ ::/EFI/BOOT; \
	mcopy -i $@ uefi_loader/$(OS_EFI) ::/EFI/BOOT/BOOTX64.EFI

$(OS_ISO): $(OS_IMG)
	mkdir -p iso
	cp $(OS_IMG) iso
	xorriso -as mkisofs -R -f -e $(OS_IMG) -no-emul-boot -o $@ iso

iso: MAKECMDGOALS=all
iso: $(OS_ISO)

qemu: MAKECMDGOALS=all
qemu: $(OS_IMG)
	qemu-system-x86_64 -L $(OVMF_DIR)/ -bios $(OVMF_FD) -usb -usbdevice disk::$(OS_IMG) -monitor stdio

debug: MAKECMDGOALS=all
debug: $(OS_IMG)
	qemu-system-x86_64 -s -S -L $(OVMF_DIR)/ -bios $(OVMF_FD) -usb -usbdevice disk::$(OS_IMG) -monitor stdio

clean: $(MODULES)
	rm -rf iso
	rm -rf $(SYSROOT)
	rm -f $(OS_IMG)
	rm -f $(OS_ISO)
