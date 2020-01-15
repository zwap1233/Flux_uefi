
#include "include.h"

#define NR_HANDLES 20

EFI_HANDLE handles[NR_HANDLES];
EFI_GUID fs_prot = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs_intf;

EFI_STATUS loadOS(EFI_HANDLE ImageHandle, EFI_BOOT_SERVICES *BootServices){

  UINTN handles_size = sizeof(handles)*NR_HANDLES;

  EFI_STATUS status = BootServices->LocateHandle(ByProtocol, &fs_prot, NULL, &handles_size, handles);
  if(EFI_ERROR(status)){
    return status;
  }

  status = BootServices->OpenProtocol(handles[0], &fs_prot, (void **) &fs_intf, ImageHandle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
  if(EFI_ERROR(status)){
    return status;
  }

  return status;

  //find handle using LocateHandle()
  //open protocol
  //close protocol
}
