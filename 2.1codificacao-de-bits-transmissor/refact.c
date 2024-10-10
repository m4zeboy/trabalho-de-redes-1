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

static const short lookup4b5b[16][5] = {
    {1, 1, 1, 1, 0},
    {0, 1, 0, 0, 1},
    {1, 0, 1, 0, 0},
    {1, 0, 1, 0, 1},
    {0, 1, 0, 1, 0},
    {0, 1, 0, 1, 1},
    {0, 1, 1, 1, 0},
    {0, 1, 1, 1, 1},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 1},
    {1, 0, 1, 1, 0},
    {1, 0, 1, 1, 1},
    {1, 1, 0, 1, 0},
    {1, 1, 0, 1, 1},
    {1, 1, 1, 0, 0},
    {1, 1, 1, 0, 1}};

void encode4b5b(short *input, short *output)
{

  short i, head, tail;
  head = 0;
  tail = 0;

  for (i = 0; i < 4; i++)
  {
    head = head << 1 | input[i];
    tail = tail << 1 | input[i + 4];
  }
  for (i = 0; i < 5; i++)
  {
    output[i] = lookup4b5b[head][i];
    output[i + 5] = lookup4b5b[tail][i];
  }
}

void emitFourBFiveBSignal(short *binary)
{
  int i;
  for (i = 0; i < 10; i++)
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
    int i;
    char k;
    Manchester manchesterBytes[80];
    scanf(" %c ", &k);
    scanf("%[^\n]", input);
    size = strlen(input);
    printf("Manchester %c %d ", k, size * 16);

    for (i = 0; i < size; i++)
    {
      charToBinary(input[i], bytes[i].binary);
      encodeManchester(bytes[i].binary, manchesterBytes[i].binary);
      emitManchesterSignal(manchesterBytes[i].binary);
    }
  }
  else if (strcmp(encoder, "4b5b") == 0)
  {
    int i, size;
    FourBFiveB fourBFiveB[80];
    scanf("%[^\n]", input);
    size = strlen(input);
    printf("4b5b %d ", size * 10);
    for (i = 0; i < size; i++)
    {
      charToBinary(input[i], bytes[i].binary);
      encode4b5b(bytes[i].binary, fourBFiveB[i].binary);
      emitFourBFiveBSignal(fourBFiveB[i].binary);
    }
  }

  return 0;
}