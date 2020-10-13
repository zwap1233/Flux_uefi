
#include "include.h"
#include "utils.h"

#define NR_HANDLES 20

EFI_HANDLE handles[NR_HANDLES];
EFI_GUID fs_prot = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *fs_intf;
EFI_FILE_PROTOCOL *root;
EFI_FILE_PROTOCOL *hello;

/**
 * @brief Load the kernal into memory.
 * 
 * @param ImageHandle The EFI_HANDLE
 * @param BootServices The EFI_BOOT_SERVICES
 * @return EFI_STATUS 
 */
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

  status = fs_intf->OpenVolume(fs_intf, &root);
  if(EFI_ERROR(status)){
    return status;
  }
  printk(L"Status: %d\n\r", status);

  status = root->Open(root, &hello, L"\\hello.txt", EFI_FILE_MODE_READ, NULL);
  if(EFI_ERROR(status)){
    return status;
  }
  printk(L"Status: %d\n\r", status);

  char buffer[20];
  int buffersize = 20;
  status = hello->Read(hello, &buffersize, buffer);
  if(EFI_ERROR(status)){
    return status;
  }
  buffer[20] = '\0';
  printk(L"Status: %d\n\r", status);

  printk(L"Text: %S \n\r", buffer);

  return status;

  //find handle using LocateHandle()
  //open protocol
  //close protocol
}
