
#ifndef __LOAD_H__
#define __LOAD_H__

#include <efi.h>
#include <efilib.h>

EFI_STATUS loadOS(EFI_HANDLE ImageHandle, EFI_HANDLE DeviceHandle, EFI_BOOT_SERVICES *BootServices);

EFI_STATUS allocateMemory(EFI_HANDLE ImageHandle, EFI_BOOT_SERVICES *BootServices);

#endif
