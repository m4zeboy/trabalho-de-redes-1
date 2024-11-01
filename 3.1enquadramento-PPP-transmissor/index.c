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

void copyShortArray(short *destiny, short *source, short size)
{
  memcpy(destiny, source, size * sizeof(short));
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

short sum(short a, short b, short carryIn, short *carryOut)
{
  if (a == 0 && b == 0 && carryIn == 0)
  {
    *carryOut = 0;
    return 0;
  }
  else if (a == 1 && b == 0 && carryIn == 0)
  {
    *carryOut = 0;
    return 1;
  }
  else if (a == 0 && b == 1 && carryIn == 0)
  {
    *carryOut = 0;
    return 1;
  }
  else if (a == 1 && b == 1 && carryIn == 0)
  {
    *carryOut = 1;
    return 0;
  }
  else if (a == 0 && b == 0 && carryIn == 1)
  {
    *carryOut = 0;
    return 1;
  }
  else if (a == 1 && b == 0 && carryIn == 1)
  {
    *carryOut = 1;
    return 0;
  }
  else if (a == 0 && b == 1 && carryIn == 1)
  {
    *carryOut = 1;
    return 0;
  }
  else
  {
    *carryOut = 1;
    return 1;
  }
}

void sum16bits(short *a, short *b, short *result)
{
  short i, carry;
  short s[16] = {0};
  carry = 0;
  for (i = 15; i >= 0; i--)
  {
    s[i] = sum(a[i], b[i], carry, &carry);
  }
  if (carry == 1)
  {
    short one[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    for (i = 15; i >= 0; i--)
    {
      carry = 0;
      s[i] = sum(s[i], one[i], carry, &carry);
    }
  }
  memcpy(result, s, 16 * sizeof(short));
}

/* return the size of buffer in bits */
short enframeBuffer(short *buffer, short *protocol, OriginalByte *payload, int size)
{
  int n, i;
  short control[8] = {0, 0, 0, 0, 0, 0, 1, 1};

  n = 0;
  /* address */
  for (i = 0; i < 8; i++)
  {
    buffer[i] = 1;
  }

  /* control */
  for (i = 8; i <= 16; i++)
  {
    buffer[i] = control[i - 8];
  }

  /* protocol */
  n = 15;
  for (i = 16; i < 32; i++)
  {
    buffer[i] = protocol[n];
    n--;
  }

  /* complete with zero if payload size is odd */
  if (size % 2 != 0)
  {
    int i;
    for (i = 0; i < 8; i++)
    {
      payload[size].binary[i] = 0;
    }
    size++;
  }

  /* payload */
  n = 0;
  i = 32;
  for (n = 0; n < size; n = n + 2)
  {
    short j;
    /* first byte */
    for (j = 0; j < 8; j++)
    {
      buffer[i] = payload[n].binary[j];
      i++;
    }
    /* last byte */
    for (j = 0; j < 8; j++)
    {
      buffer[i] = payload[n + 1].binary[j];
      i++;
    }
  }

  return i;
}

void calculateChecksum(short *checksum, short *protocol, OriginalByte *payload, int size)
{
  short buffer[1500 + 32], i, n;
  i = enframeBuffer(buffer, protocol, payload, size);
  for (n = 0; n < i / 16; n++)
  {
    /* row */
    short current[16], offset;
    offset = n * 16;
    copyShortArray(current, buffer + offset, 16);
    sum16bits(checksum, current, checksum);
  }
  for (n = 0; n < 16; n++)
  {
    checksum[n] = checksum[n] == 0 ? 1 : 0;
  }
}

int main(void)
{
  short i, j, protocol[16], size, stuffedSize;
  char p[5], m[1501];
  OriginalByte payload[1500];

  Frame frame;
  short checksum[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  short flag[] = {0, 1, 1, 1, 1, 1, 1, 0};
  short address[] = {1, 1, 1, 1, 1, 1, 1, 1};
  short control[] = {0, 0, 0, 0, 0, 0, 1, 1};
  scanf("%s %[^\n]", p, m);

  size = strlen(m);

  convertDecimalToBinary(convertHexaToDecimal(p), protocol);

  convertStringToBinary(m, payload);

  stuffedSize = byteStuffing(payload, size, frame.payload);

  calculateChecksum(checksum, protocol, payload, size);

  for (i = 0; i < 8; i++)
  {
    printf("%hd", flag[i]);
  }
  printf(" ");

  for (i = 0; i < 8; i++)
  {

    printf("%hd", address[i]);
  }
  printf(" ");

  for (i = 0; i < 8; i++)
  {

    printf("%hd", control[i]);
  }
  printf(" ");

  for (i = 1; i >= 0; i--)
  {
    for (j = 7; j >= 0; j--)
    {
      printf("%hd", protocol[(i * 8) + j]);
    }
    printf(" ");
  }

  for (i = 0; i < stuffedSize; i++)
  {
    for (j = 0; j < 8; j++)
    {
      printf("%hd", frame.payload[i].binary[j]);
    }
    printf(" ");
  }

  for (i = 0; i < 2; i++)
  {
    for (j = 0; j < 8; j++)
    {
      printf("%hd", checksum[(i * 8) + j]);
    }
    printf(" ");
  }

  for (i = 0; i < 8; i++)
  {
    printf("%hd", flag[i]);
  }

  return 0;
}
