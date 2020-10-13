#ifndef __MAIN_H__
#define __MAIN_H__

#include <efi.h>
#include <efilib.h>

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable);

EFI_STATUS printImageBase(EFI_HANDLE ImageHandle);

void testPrintFunction();

#endif