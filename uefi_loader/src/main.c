
#include "include.h"

EFI_SYSTEM_TABLE *gST;

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable){
    EFI_STATUS status;
    EFI_INPUT_KEY Key;

    /* Store the system table for future use in other functions */
    gST = SystemTable;


    gST->ConOut->ClearScreen(gST->ConOut);

    /* Say hi */
    status = gST->ConOut->OutputString(gST->ConOut, L"Hello World\n\r");
    if (EFI_ERROR(status)){
      return status;
    }

    status = loadOS(ImageHandle, gST->BootServices);

    return status;
}
