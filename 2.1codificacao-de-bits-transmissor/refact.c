#include <stdio.h>
#include <string.h>

typedef struct originalByte
{
  char data;
  short binary[8];
} OriginalByte;

typedef struct manchester
{
  short binary[16];
} Manchester;

void charToBinary(char input, short *output)
{
  int i;
  for (i = 7; i >= 0; i--)
  {
    output[i] = (input >> (7 - i)) & 1;
  }
}

void emitNRZSignal(int size, OriginalByte *bytes, char *input)
{
  int i, j;
  printf("NRZ %d ", size * 8);
  for (i = 0; i < size; i++)
  {
    bytes[i].data = input[i];
    charToBinary(input[i], bytes[i].binary);
  }
  for (i = 0; i < size; i++)
  {
    for (j = 0; j < 8; j++)
    {
      printf("%c", bytes[i].binary[j] == 0 ? 'B' : 'A');
    }
  }
}

void encodeManchester(short *binary, short *manchester)
{
  int i;
  for (i = 0; i < 8; i++)
  {
    if (binary[i] == 0)
    {
      /* 1 0 */
      manchester[i * 2] = 1;
      manchester[i * 2 + 1] = 0;
    }
    else
    {
      /* 0 1 */
      manchester[i * 2] = 0;
      manchester[i * 2 + 1] = 1;
    }
  }
}

void emitManchesterSignal(short *binary)
{
  int i;
  for (i = 0; i < 16; i++)
  {
    if (binary[i] == 1)
    {
      printf("%c", 'A');
    }
    else
    {
      printf("%c", 'B');
    }
  }
}

int main(void)
{
  char encoder[12], input[81];
  OriginalByte bytes[80];
  Manchester manchesterBytes[80];

  int size;
  scanf("%s ", encoder);
  if (strcmp(encoder, "NRZ") == 0)
  {
    scanf("%[^\n]", input);
    size = strlen(input);
    emitNRZSignal(size, bytes, input);
  }
  else if (strcmp(encoder, "Manchester") == 0)
  {
    int i, j;
    scanf("%[^\n]", input);
    size = strlen(input);
    for (i = 0; i < size; i++)
    {
      charToBinary(input[i], bytes[i].binary);
      encodeManchester(bytes[i].binary, manchesterBytes[i].binary);
      emitManchesterSignal(manchesterBytes[i].binary);
    }
  }

  return 0;
}