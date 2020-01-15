
#include "utils.h"

typedef struct {
  int mode = 0;               //0: normal chars, 1: specifiying flags, 2: width, 3: precision

  BOOLEAN sign = FALSE;       //TRUE: always use sign regardles wether positive or negative number
  BOOLEAN padding = FALSE;    //TRUE: pad with 0's instead of spaces
  BOOLEAN hex_prefix = FALSE; //TRUE: prefix hex numbers with 0x

  BOOLEAN width_specified = FALSE; //Is the width specified or should it be ignored.
  int width = 0;                   //width of specifier;

  //BOOLEAN precision_specified = FALSE;
  //int precision = 0;
} print_state;

EFI_STATUS printk(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *CONTEXT, const CHAR16 *format, ...){
  va_list a_list;
  va_start(a_list, format);
}

EFI_STATUS sprintk(CHAR16 *buffer, int max_len, const CHAR16 *format, ...){
  va_list a_list;
  va_start(a_list, format);

  print_state ps;

  int f_ind = 0;
  int b_ind = 0;

  int done = 0;
  while(!done && b_ind < max_len){
    CHAR16 c = format[f_ind];
    ++f_ind;

    if(ps.mode == 0){
      if(c != '%'){
        buffer[b_ind] = c;
        ++b_ind;
      } else {
        ps.mode = TRUE;
      }

      if(c == '\0') {
        break;
      }
    } else {
      switch(c){
      case '%':
        buffer[b_ind] = '%';
        ++b_ind;
        break;

        //flags
      case '+':
        ps.sign = TRUE;
        break;
      case '#':
        ps.hex_prefix = TRUE;
        break;
      case '0':
        if(ps.mode == 1){
          ps.padding = TRUE;
        } else if(ps.mode == 2){
          ps.width = ps.width*10;
        }
        break;

        //width specifiers
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        ps.mode = 2;
        ps.width_specified = TRUE;

        ps.width = ps.width * 10 + (c - '0');
        break;
        //TODO: implement precision
        //TODO: implement length modifiers

      case 'd':
        break;
      case 'd':
        break;
      }
    }
  }
}
