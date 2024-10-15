#include <stdio.h>
#include <string.h>

typedef enum boolean
{
  false,
  true
} boolean;

typedef struct originalByte
{
  char data;
  short binary[8];
} OriginalByte;

typedef struct frame
{
  short flag_start[8];
  short address[8];
  short control[8];
  short protocol[16];
  OriginalByte payload[1518];
  short checksum[16];
  short flag_end[8];
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

void charToBinary(char input, short *output)
{
  int i;
  for (i = 7; i >= 0; i--)
  {
    output[i] = (input >> (7 - i)) & 1;
  }
}

void convertStringToBinary(char *message, OriginalByte *bytes)
{
  short i;
  for (i = 0; message[i] != '\0'; i++)
  {
    bytes[i].data = message[i];
    charToBinary(message[i], bytes[i].binary);
  }
}

boolean byteCompare(short *a, short *b)
{
  short i;
  for (i = 0; i < 8; i++)
  {
    if (a[i] != b[i])
    {
      return false;
    }
  }
  return true;
}

void copyByte(short *destiny, short *source)
{
  memcpy(destiny, source, 8 * sizeof(short));
}

short byteStuffing(OriginalByte *payload, short size, OriginalByte *stuffedPayload)
{
  short flag[8] = {0, 1, 1, 1, 1, 1, 1, 0};

  short escape[8] = {0, 1, 1, 1, 1, 1, 0, 1};

  short i, stuffedSize;
  stuffedSize = 0;

  for (i = 0; i < size; i++)
  {
    if (byteCompare(payload[i].binary, flag) || byteCompare(payload[i].binary, escape))
    {
      copyByte(stuffedPayload[stuffedSize++].binary, escape);
    }
    copyByte(stuffedPayload[stuffedSize++].binary, payload[i].binary);
  }
  return stuffedSize;
}

const short address[8] = {
    1, 1, 1, 1, 1, 1, 1, 1};

const short control[8] = {
    0, 0, 0, 0, 0, 0, 1, 1};

int main(void)
{
  short i, j, protocol[16], size, stuffedSize;
  char p[5], m[15001];
  OriginalByte payload[1500];

  Frame frame;

  scanf("%s %[^\n]", p, m);

  size = strlen(m);

  convertDecimalToBinary(convertHexaToDecimal(p), protocol);
  convertStringToBinary(m, payload);
  stuffedSize = byteStuffing(payload, size, frame.payload);

  for (i = 0; i < stuffedSize; i++)
  {
    for (j = 0; j < 8; j++)
    {
      printf("%hd", frame.payload[i].binary[j]);
    }
    printf(" ");
  }

  return 0;
}