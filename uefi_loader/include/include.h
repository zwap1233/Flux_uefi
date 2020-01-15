
#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <efi.h>
#include <efilib.h>

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);

EFI_STATUS loadOS(EFI_HANDLE ImageHandle, EFI_BOOT_SERVICES *gBS);

#endif
