#include <stdio.h>
#include <string.h>

typedef struct originalByte
{
  char data;
  short binary[8];
} OriginalByte;

void decodeNRZ(char *input, short signal_length, OriginalByte *bytes)
{
  short i, range;
  range = signal_length / 8;

  for (i = 0; i < range; i++)
  {
    int j;
    bytes[i].data = 0;
    for (j = 0; j < 8; j++)
    {
      bytes[i].binary[j] = input[(i * 8) + j] == 'B' ? 0 : 1;
      bytes[i].data = bytes[i].data << 1 | bytes[i].binary[j];
    }
  }
}

int main(void)
{
  char encoder[12], input[1281];

  scanf("%s ", encoder);

  if (strcmp(encoder, "NRZ") == 0)
  {
    short signal_length;
    short i, j;
    OriginalByte bytes[80];
    scanf("%hd %[^\n]", &signal_length, input);
    decodeNRZ(input, signal_length, bytes);
    for (i = 0; i < signal_length / 8; i++)
    {

      printf("%c ", bytes[i].data);
    }
  }
  else if (strcmp(encoder, "Manchester") == 0)
  {
  }
  else if (strcmp(encoder, "4b5b") == 0)
  {
  }

  return 0;
}