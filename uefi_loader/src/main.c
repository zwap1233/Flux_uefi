
#include "include.h"
#include "utils.h"

//#define __TEST
#define __TEST_INT
#define __TEST_OTHER
#define __TEST_HEX
#define __TEST_POINTER
#define __TEST_STR

void testPrintFunction();

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

  init_print(gST->ConOut);

#ifdef __TEST
  testPrintFunction();
#endif

  status = loadOS(ImageHandle, gST->BootServices);
  printk(L"Status loadOS:%d", status);

  while(1){;}

  return status;
}

void testPrintFunction(){
  printk(L"Hello this is a test\n\r");

#ifdef __TEST_INT
  printk(L"Int:%d,33\n\r", 33);
  printk(L"Int:%d,-54\n\r", -54);
  printk(L"Int:%+d,+12\n\r", 12);
  printk(L"Int:%3d, 16\n\r", 16);
  printk(L"Int:%3d,- 16\n\r", -16);
  printk(L"Int:%03d,008\n\r", 8);
  printk(L"Int:%010d,0000001402\n\r", 1402);
  printk(L"Int:%05d,5640894\n\r", 5640894);
#endif

#ifdef __TEST_OTHER
  printk(L"uint:%u,28\n\r", 28);

  printk(L"oct:%o,23\n\r", 19);
  printk(L"oct:%03o,023\n\r", 19);
#endif

#ifdef __TEST_HEX
  printk(L"hex:%x,3a\n\r", 58);
  printk(L"hex:%03x,020\n\r", 32);
  printk(L"hex:%#x,0x4c\n\r", 76);
  printk(L"hex:%#3x,0x 4c\n\r", 76);
  printk(L"hex:%#03x,0x04c\n\r", 76);
#endif

#ifdef __TEST_POINTER
  int *pa = (int *) 0x0000000000000200;
  int *pb = (int *) 0x00000000003f0001;
  int *pc = (int *) 0x0c0f3000a0b60500;
  printk(L"pointer:%#016p\n\r", pa);
  printk(L"pointer:%#0p\n\r", pa);
  printk(L"pointer:%#016p\n\r", pb);
  printk(L"pointer:%#0p\n\r", pb);
  printk(L"pointer:%#016p\n\r", pc);
  printk(L"pointer:%#0p\n\r", pc);
#endif

#ifdef __TEST_STR
  CHAR16 ch = 'd';
  printk(L"char:%c,l\n\r", 'l');
  printk(L"char:%c,d\n\r", ch);

  printk(L"char:%s,Hello World\n\r", L"Hello World");
#endif

  printk(L"Mix:%c hi %d,a hi 120\n\r", 'a', 120);
}

