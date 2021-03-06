
#include "utils.h"

typedef struct {
  int mode;           //0: normal chars, 1: specifiying flags, 2: width, 3: precision

  BOOLEAN sign;       //TRUE: always use sign regardles wether positive or negative number
  BOOLEAN padding;    //TRUE: pad with 0's instead of spaces
  BOOLEAN hex_prefix; //TRUE: prefix hex numbers with 0x

  BOOLEAN width_specified; //Is the width specified or should it be ignored.
  int width;               //width of specifier;

  //BOOLEAN precision_specified = FALSE;
  //int precision = 0;
} print_state;

EFI_STATUS insertInteger(print_state *ps, CHAR16 *buffer, int *index, int base, int number);
EFI_STATUS insertUnisgnedInteger(print_state *ps, CHAR16 *buffer, int *index, int base, unsigned int number);
EFI_STATUS insertUnisgnedLongInteger(print_state *ps, CHAR16 *buffer, int *index, int base, unsigned long long int number);

EFI_SIMPLE_TEXT_OUT_PROTOCOL *ConOut;

/**
 * @brief Setup printing
 * 
 * @param protocol The EFI_SIMPLE_TEXT_OUT_PROTOCOL
 * @return EFI_STATUS The resulting status from the setup
 */
EFI_STATUS init_print(EFI_SIMPLE_TEXT_OUT_PROTOCOL *protocol){
  EFI_STATUS status = EFI_SUCCESS;
  ConOut = protocol;

  status = ConOut->ClearScreen(ConOut);
  if(EFI_ERROR(status)){
    return status;
  }
  
  status = ConOut->OutputString(ConOut, L"Flux Operating System UEFI Bootloader\n\r");
  if (EFI_ERROR(status)){
    return status;
  }

  return status;
}

/**
 * @brief Like printf
 * 
 * @param format 
 * @param ... 
 * @return EFI_STATUS 
 */
EFI_STATUS printk(const CHAR16 *format, ...){
  va_list a_list;
  va_start(a_list, format);

  CHAR16 buffer[2048];
  EFI_STATUS status = vsprintk(buffer, 2048, format, a_list);

  ConOut->OutputString(ConOut, buffer);

  va_end(a_list);
  return status;
}

/**
 * @brief like sprintf
 * 
 * @param buffer 
 * @param max_len 
 * @param format 
 * @param ... 
 * @return EFI_STATUS 
 */
EFI_STATUS sprintk(CHAR16 *buffer, int max_len, const CHAR16 *format, ...){
  va_list a_list;
  va_start(a_list, format);

  EFI_STATUS status = vsprintk(buffer, max_len, format, a_list);

  va_end(a_list);
  return status;
}

/**
 * @brief Like vsprintf
 * 
 * @param buffer 
 * @param max_len 
 * @param format 
 * @param a_list 
 * @return EFI_STATUS 
 */
EFI_STATUS vsprintk(CHAR16 *buffer, int max_len, const CHAR16 *format, va_list a_list){
  print_state ps = {0, FALSE, FALSE, FALSE, FALSE, 0};

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
        ps.mode = 1;
      }

      if(c == '\0') {
        done = 1;
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

      //type specifiers
        // signed decimal integer
      case 'd':
      case 'i':
        insertInteger(&ps, buffer, &b_ind, 10, va_arg(a_list, int));
        ps.mode = 0;
        break;

        //unsigned decimal integer
      case 'u':
        insertUnisgnedInteger(&ps, buffer, &b_ind, 10, va_arg(a_list, unsigned int));
        ps.mode = 0;
        break;

        //unsigned octal integer
      case 'o':
        insertUnisgnedInteger(&ps, buffer, &b_ind, 8, va_arg(a_list, unsigned int));
        ps.mode = 0;
        break;

        //unsigned hex integer
      case 'x':
      case 'X':
        if(ps.hex_prefix){
          buffer[b_ind] = '0';
          buffer[b_ind+1] = 'x';
          b_ind += 2;
        }

        insertUnisgnedInteger(&ps, buffer, &b_ind, 16, va_arg(a_list, unsigned int));
        ps.mode = 0;
        break;

        //decimal floating point
      case 'f':
      case 'F':
        return EFI_UNSUPPORTED;
        break;

        //character
      case 'c': ;
        CHAR16 ch = (CHAR16) va_arg(a_list, unsigned int);
        buffer[b_ind] = ch;
        ++b_ind;
        ps.mode = 0;
        break;

        //string
      case 's': ;
        CHAR16 *str = (CHAR16 *) va_arg(a_list, unsigned int *);
        int i = 0;
        for(i = 0; str[i] != '\0'; ++i, ++b_ind){
          buffer[b_ind] = str[i];
        }
        ps.mode = 0;
        break;
      
      case 'S': ;
        char *str8 = (char *) va_arg(a_list, unsigned int *);
        int i8 = 0;
        for(i8 = 0; str8[i8] != '\0'; ++i8, ++b_ind){
          buffer[b_ind] = str8[i8];
        }
        ps.mode = 0;
        break;

        //pointer address
      case 'p':
        if(ps.hex_prefix){
          buffer[b_ind] = '0';
          buffer[b_ind+1] = 'x';
          b_ind += 2;
        }

        insertUnisgnedLongInteger(&ps, buffer, &b_ind, 16, (unsigned long long int) va_arg(a_list, unsigned long long int *));
        ps.mode = 0;
        break;
      }
    }
  }

  buffer[b_ind] = '\0';

  return EFI_SUCCESS;
}

/**
 * @brief Write a integer to buffer starting at index. Max number length is 256
 * 
 * @param ps The print_state struct of the print job
 * @param buffer A pointer to the CHAR16 buffer where the integer should be written to.
 * @param index The starting point in the buffer, it should be safe to write to this index. Its updated after number is written.
 * @param base The base of the number to be writen (i.e. 10 for decimal, 16 for hex, etc.) Max supported is 16
 * @param number The number to be written
 * @return EFI_STATUS 
 */
EFI_STATUS insertInteger(print_state *ps, CHAR16 *buffer, int *index, int base, int number){
  if(ps->sign || (number < 0)){
    buffer[*index] = (number < 0) ? '-' : '+';
    ++(*index);
  }

  return insertUnisgnedInteger(ps, buffer, index, base, (unsigned int) (number < 0 ? -1*number : number));
}

/**
 * @brief Write a unsigned integer to buffer starting at index. Max number length is 256
 * 
 * @param ps The print_state struct of the print job
 * @param buffer A pointer to the CHAR16 buffer where the unsigned integer should be written to.
 * @param index The starting point in the buffer, it should be safe to write to this index. Its updated after number is written.
 * @param base The base of the number to be writen (i.e. 10 for decimal, 16 for hex, etc.) Max supported is 16
 * @param number The number to be written
 * @return EFI_STATUS 
 */
EFI_STATUS insertUnisgnedInteger(print_state *ps, CHAR16 *buffer, int *index, int base, unsigned int number){
  return insertUnisgnedLongInteger(ps, buffer, index, base, (unsigned long long int) number);
}

/**
 * @brief Write a unsigned long integer to buffer starting at index. Max number length is 256
 * 
 * @param ps The print_state struct of the print job
 * @param buffer A pointer to the CHAR16 buffer where the unsigned long integer should be written to.
 * @param index The starting point in the buffer, it should be safe to write to this index. Its updated after number is written.
 * @param base The base of the number to be writen (i.e. 10 for decimal, 16 for hex, etc.) Max supported is 16
 * @param number The number to be written
 * @return EFI_STATUS 
 */
EFI_STATUS insertUnisgnedLongInteger(print_state *ps, CHAR16 *buffer, int *index, int base, unsigned long long int number){
  unsigned long long int num = number;

  CHAR16 buf[256];
  int ind = 0;

  if(base > 16){
    return EFI_UNSUPPORTED;
  }

  while ((int) (num / base) != 0){
    unsigned long long int next_num = num % base;

    if(next_num < 10){
      buf[ind] = next_num + '0';
    } else if (next_num >= 10 && next_num < 16){
      buf[ind] = next_num - 10 + 'a';
    } else {
      return EFI_UNSUPPORTED;
    }

    ++ind;
    num = num/base;
  }

  if(num < 10){
    buf[ind] = num + '0';
  } else if (num >= 10 && num < 16){
    buf[ind] = num - 10 + 'a';
  } else {
    return EFI_UNSUPPORTED;
  }

  if(ps->width_specified && ps->width > ind+1){
    int i = 0;
    for(i = 0; i < (ps->width - (ind+1)); ++i){
      buffer[*index] = ps->padding ? '0' : ' ';
      ++(*index);
    }
  }

  for(ind = ind; ind >= 0; --ind, ++(*index)){
    buffer[*index] = buf[ind];
  }

  return EFI_SUCCESS;
}
