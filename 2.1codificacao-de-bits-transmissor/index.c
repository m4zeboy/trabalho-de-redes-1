#include <stdio.h>
#include <string.h>

typedef struct byte
{
  short data[8];
  short size;
} Byte;

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
short int_to_binary(int input, short *output)
{
  int i = 0;
  while (input > 0)
  {
    output[i] = input % 2;
    input = input / 2;
    i++;
  }

  return i - 1;
}

/* return the size of message (bits) */
void low_level_layer(int *input, int input_size, Byte output[80])
{
  int i;
  for (i = 0; i < input_size; i++)
  {
    output[i].size = int_to_binary(input[i], output[i].data);
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
      if (bytes[i].data[j] == 0)
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

int main(void)
{
  /*
    NRZ
    MANCHESTER
    4b/5b
  */
  char encoder[11], message[81];
  int data[80], size;
  Byte bytes[80];

  initializeBytes(bytes);

  scanf("%s ", encoder);

  high_level_layer(message);
  size = mid_level_layer(message, data);
  low_level_layer(data, size, bytes);
  if (strcmp(encoder, "NRZ") == 0)
  {
    emitNRZSignal(bytes, size);
  }

  return 0;
}