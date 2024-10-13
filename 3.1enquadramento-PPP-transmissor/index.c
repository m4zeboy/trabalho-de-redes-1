#include <stdio.h>

typedef struct frame
{
  short *flag_start;
  short *address;
  short *control;
  short *protocol;
  short *payload;
  short *checksum;
  short *flag_end;
} Frame;

short mapHexaToDecimal(char n)
{
  switch (n)
  {
  case '0':
    return 0;
    break;
  case '1':
    return 1;
    break;
  case '2':
    return 2;
    break;
  case '3':
    return 3;
    break;
  case '4':
    return 4;
    break;
  case '5':
    return 5;
    break;
  case '6':
    return 6;
    break;
  case '7':
    return 7;
    break;
  case '8':
    return 8;
    break;
  case '9':
    return 9;
    break;
  case 'A':
    return 10;
    break;
  case 'B':
    return 11;
    break;
  case 'C':
    return 12;
    break;
  case 'D':
    return 13;
    break;
  case 'E':
    return 14;
    break;
  case 'F':
    return 15;
    break;
  default:
    return -1;
    break;
  }
}

int convertHexaToDecimal(char *n)
{
  int i;
  int result = 0;
  int pow = 1;
  for (i = 3; i >= 0; i--)
  {
    printf("%hd\n", mapHexaToDecimal(n[i]));
    result += mapHexaToDecimal(n[i]) * pow;
    pow = pow << 4;
  }
  return result;
}

/* Reversed Order */
void convertDecimalToBinary(int n, short *output)
{
  short x, i;
  i = 0;
  while (n > 0)
  {
    x = n % 2;
    n = n / 2;
    output[i] = x;
    i++;
  }
}

const short flag[8] = {
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    0};

const short address[8] = {
    1, 1, 1, 1, 1, 1, 1, 1};

const short control[8] = {
    0, 0, 0, 0, 0, 0, 1, 1};

void enframe(Frame *frame)
{
  frame->flag_start = flag;
  frame->address = address;
  frame->control = control;
  frame->flag_end = flag;
}

int main(void)
{
  short i, protocol[16];
  char p[5], m[15001];
  Frame frame;

  scanf("%s %[^\n]", p, m);

  printf("%d\n", convertHexaToDecimal(p));
  convertDecimalToBinary(convertHexaToDecimal(p), protocol);

  for (i = 1; i >= 0; i--)
  {
    short j;
    for (j = 7; j >= 0; j--)
    {
      printf("%d", protocol[(i * 8) + j]);
    }
    printf(" ");
  }
  return 0;
}