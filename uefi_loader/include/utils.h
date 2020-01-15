
#ifndef __UTILS_H__
#define __UTILS_H__

#include <efi.h>
#include <efilib.h>

EFI_STATUS printk(EFI_SIMPLE_TEXT_OUT_PROTOCOL *CONTEXT, const CHAR16* format, ...);

#endif
