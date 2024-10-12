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

typedef struct FourBFiveB
{
  short binary[10];
} FourBFiveB;

short binaryToDecimal(short *binary, short start, short end)
{
  short i, decimal;
  decimal = 0;
  for (i = start; i < end; i++)
  {
    decimal = decimal << 1 | binary[i];
  }
  return decimal;
}

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

void receive4b5bSignal(char *input, short signal_length, FourBFiveB *fourBFiveB)
{
  short i, j, range;
  range = signal_length / 10;
  for (i = 0; i < range; i++)
  {
    for (j = 0; j < 10; j++)
    {
      fourBFiveB[i].binary[j] = input[(i * 10) + j] == 'A' ? 1 : 0;
    }
  }
}

void decode4b5b(FourBFiveB *fourBFiveB, short size, OriginalByte *bytes)
{
  short i;
  short map[31][4] = {
      {0, 0, 0, 0},

      {0, 0, 0, 0},

      {0, 0, 0, 0},

      {0, 0, 0, 0},

      {0, 0, 0, 0},
      {0, 0, 0, 0},

      {0, 0, 0, 0},

      {0, 0, 0, 0},

      {0, 0, 0, 0},

      {0, 0, 0, 1},
      {0, 1, 0, 0},
      {0, 1, 0, 1},
      {0, 1, 1, 0},
      {0, 0, 0, 0},

      {0, 1, 1, 0},
      {0, 1, 1, 1},
      {0, 0, 0, 0},

      {0, 0, 0, 0},

      {1, 0, 0, 0},
      {1, 0, 0, 1},
      {0, 0, 1, 0},
      {0, 0, 1, 1},
      {1, 0, 1, 0},
      {1, 0, 1, 1},
      {0, 0, 0, 0},

      {0, 0, 0, 0},

      {1, 1, 0, 0},
      {1, 1, 0, 1},
      {1, 1, 1, 0},
      {1, 1, 1, 1},
      {0, 0, 0, 0},
  };

  for (i = 0; i < size; i++)
  {
    short j;
    short head = 0;
    short tail = 0;
    short data = 0;
    head = binaryToDecimal(fourBFiveB[i].binary, 0, 5);
    tail = binaryToDecimal(fourBFiveB[i].binary, 5, 10);
    for (j = 0; j < 4; j++)
    {
      bytes[i].binary[j] = map[head][j];
    }

    for (j = 4; j < 8; j++)
    {
      bytes[i].binary[j] = map[tail][j - 4];
    }

    data = binaryToDecimal(bytes[i].binary, 0, 8);
    bytes[i].data = data;
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
    FourBFiveB fourBFiveB[80];

    OriginalByte bytes[80];

    short i, j;
    scanf("%hd %[^\n]", &signal_length, input);
    receive4b5bSignal(input, signal_length, fourBFiveB);
    decode4b5b(fourBFiveB, signal_length / 10, bytes);

    for (i = 0; i < signal_length / 10; i++)
    {
      printf("%c", bytes[i].data);
    }
  }

  return 0;
}