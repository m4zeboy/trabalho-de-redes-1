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

void receiveManchesterSignal(char *input, short signal_length, Manchester *manchester)
{
  short i, j, range;
  range = signal_length / 16;
  for (i = 0; i < range; i++)
  {
    for (j = 0; j < 16; j++)
    {
      if (input[(i * 16) + j] == 'B')
      {
        manchester[i].binary[j] = 0;
      }
      else
      {
        manchester[i].binary[j] = 1;
      }
    }
  }
}

void decodeManchester(Manchester *manchester, short size, OriginalByte *bytes)
{
  short i, j, data;
  data = 0;
  for (i = 0; i < size; i++)
  {
    for (j = 0; j < 8; j++)
    {
      /* 01 = 1*/
      if (manchester[i].binary[j * 2] == 0)
      {
        bytes[i].binary[j] = 1;
        data = data << 1 | 1;
      }
      else
      {
        /* 10 = 0 */
        bytes[i].binary[j] = 0;
        data = data << 1 | 0;
      }
      bytes[i].data = data;
    }
  }
}

int main(void)
{
  char encoder[12], input[1281];
  short signal_length;

  scanf("%s ", encoder);

  if (strcmp(encoder, "NRZ") == 0)
  {
    short i;
    OriginalByte bytes[80];
    scanf("%hd %[^\n]", &signal_length, input);
    decodeNRZ(input, signal_length, bytes);
    for (i = 0; i < signal_length / 8; i++)
    {

      printf("%c", bytes[i].data);
    }
  }
  else if (strcmp(encoder, "Manchester") == 0)
  {
    short i;
    char k;
    Manchester manchester[80];
    OriginalByte bytes[80];
    scanf("%c %hd %[^\n]", &k, &signal_length, input);
    receiveManchesterSignal(input, signal_length, manchester);
    decodeManchester(manchester, signal_length / 16, bytes);
    for (i = 0; i < signal_length / 16; i++)
    {
      printf("%c", bytes[i].data);
    }
  }
  else if (strcmp(encoder, "4b5b") == 0)
  {
  }

  return 0;
}