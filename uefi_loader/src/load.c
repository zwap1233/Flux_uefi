
#include "load.h"
#include "utils.h"

EFI_GUID gEfiSimpleFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

/**
 * @brief Load the kernal into memory.
 * 
 * @param ImageHandle The EFI_HANDLE
 * @param BootServices The EFI_BOOT_SERVICES
 * @return EFI_STATUS 
 */
EFI_STATUS loadOS(EFI_HANDLE ImageHandle, EFI_HANDLE DeviceHandle, EFI_BOOT_SERVICES *BootServices){
  EFI_STATUS status = EFI_SUCCESS;

  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
  EFI_FILE_PROTOCOL *Root;


  status = BootServices->OpenProtocol(DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void **) &FileSystem, ImageHandle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
  if(EFI_ERROR(status)){
    return status;
  }

  status = FileSystem->OpenVolume(FileSystem, &Root);
  if(EFI_ERROR(status)){
    return status;
  }

  /*status = Root->Open(Root, &hello, L"\\hello.txt", EFI_FILE_MODE_READ, 0);
  if(EFI_ERROR(status)){
    return status;
  }
  printk(L"Status: %d\n\r", status);

  char buffer[20];
  int buffersize = 20;
  status = hello->Read(hello, &buffersize, buffer);
  if(EFI_ERROR(status)){
    return status;
  }*/

  return status;

  //allocate memory
  //close protocol
}

EFI_STATUS allocateMemory(EFI_HANDLE ImageHandle, EFI_BOOT_SERVICES *BootServices){
  return 0;
}