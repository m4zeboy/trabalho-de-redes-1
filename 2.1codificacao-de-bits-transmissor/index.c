#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct byte
{
  short data[8];
  short size;
} Byte;

typedef struct splittedByte
{
  short head[4];
  short decimalHead;
  short tail[4];
  short decimalTail;
} SplittedByte;

typedef struct byte4b5b
{
  short head[5];
  short tail[5];
} Byte4b5b;

int power(int x, int n)
{
  return x << n;
}

void high_level_layer(char *input)
{
  scanf("%[^\n]", input);
}

int mid_level_layer(char *input, int *output)
{
  int i, size;
  i = 0;
  size = 0;
  while (input[i] != '\0')
  {
    size++;
    output[i] = input[i];
    i++;
  }

  return size;
}

/* return the size of array */
void int_to_binary(unsigned int input, short *output)
{
  int i = 0;
  while (input > 0)
  {
    output[i] = input % 2;
    input = input / 2;
    i++;
  }
}

/* return the size of message (bits) */
void low_level_layer(int *input, int input_size, Byte output[80])
{
  int i;
  for (i = 0; i < input_size; i++)
  {
    int_to_binary(input[i], output[i].data);
    output[i].size = 8;
  }
}

void initializeBytes(Byte bytes[80])
{
  int i, j;
  for (i = 0; i < 80; i++)
  {
    for (j = 0; j < 8; j++)
    {
      bytes[i].data[j] = 0;
    }
  }
}

void emitNRZSignal(Byte bytes[80], int size)
{
  int i, j;
  printf("NRZ %d ", size * 8);

  for (i = 0; i < size; i++)
  {
    for (j = 7; j >= 0; j--)
    {
      printf("%c", bytes[i].data[j] == 0 ? 'B' : 'A');
    }
  }
}

void emitManchesterSignal(Byte bytes[80], int size, char k)
{
  int i, j;

  printf("Manchester %c %d ", k, size * 16);

  for (i = 0; i < size; i++)
  {
    for (j = 7; j >= 0; j--)
    {
      printf("%s", bytes[i].data[j] == 0 ? "AB" : "BA");
    }
  }
}

void splitByte(Byte input, SplittedByte *output)
{
  int i;
  output->decimalHead = 0;
  for (i = 7; i >= 4; i--)
  {
    output->head[i - 4] = input.data[i];
    output->decimalHead = (output->decimalHead << 1) | input.data[i];
  }

  output->decimalTail = 0;
  for (i = 3; i >= 0; i--)
  {
    output->tail[i] = input.data[i];
    output->decimalTail = (output->decimalTail << 1) | input.data[i];
  }
}

const short lookup4b5b[16][5] = {
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

void convert4bTo5b(short decimalData, short output[5])
{
  int i;
  for (i = 0; i < 5; i++)
  {
    output[i] = lookup4b5b[decimalData][i];
  }
}

int main(void)
{
  char encoder[11], message[81];
  int data[80], size;
  Byte bytes[80];
  SplittedByte splitedBytes[80];
  Byte4b5b data4b5b[80];
  initializeBytes(bytes);

  scanf("%s ", encoder);

  if (strcmp(encoder, "NRZ") == 0)
  {
    high_level_layer(message);
    size = mid_level_layer(message, data);
    low_level_layer(data, size, bytes);
    emitNRZSignal(bytes, size);
  }
  else if (strcmp(encoder, "Manchester") == 0)
  {
    char k;
    scanf(" %c ", &k);
    high_level_layer(message);
    size = mid_level_layer(message, data);
    low_level_layer(data, size, bytes);
    emitManchesterSignal(bytes, size, k);
  }
  else if (strcmp(encoder, "4b5b") == 0)
  {
    int i, j;

    high_level_layer(message);
    size = mid_level_layer(message, data);
    low_level_layer(data, size, bytes);
    for (i = 0; i < size; i++)
    {
      splitByte(bytes[i], &(splitedBytes[i]));
    }

    for (i = 0; i < size; i++)
    {
      convert4bTo5b(splitedBytes[i].decimalHead, data4b5b[i].head);
      convert4bTo5b(splitedBytes[i].decimalTail, data4b5b[i].tail);
    }
    printf("4b5b %d ", (5 * size * 2));
    for (i = 0; i < size; i++)
    {
      for (j = 0; j < 5; j++)
      {

        if (data4b5b[i].head[j] == 0)
        {
          printf("B");
        }
        else
        {
          printf("A");
        }
      }
      for (j = 0; j < 5; j++)
      {
        if (data4b5b[i].tail[j] == 0)
        {
          printf("B");
        }
        else
        {
          printf("A");
        }
      }
    }
  }

  return 0;
}