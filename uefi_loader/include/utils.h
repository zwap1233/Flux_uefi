
#ifndef __UTILS_H__
#define __UTILS_H__

#include <efi.h>
#include <efilib.h>

EFI_STATUS init_print(EFI_SIMPLE_TEXT_OUT_PROTOCOL *protocol);

EFI_STATUS printk(const CHAR16* format, ...);
EFI_STATUS sprintk(CHAR16 *buffer, int max_len, const CHAR16 *format, ...);
EFI_STATUS vsprintk(CHAR16 *buffer, int max_len, const CHAR16 *format, va_list a_list);

#endif
