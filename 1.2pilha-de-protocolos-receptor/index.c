#include <stdio.h>
#include <stdlib.h>

typedef struct byte
{
  char data[8];
  short size;
} Byte;

int receive_low_level_layer(Byte low__level_layer_data[80])
{
  int i, j, k;
  char input[720];
  scanf("%[^\n]", input);
  i = 0;
  j = 0;
  k = 0;
  while (input[i] != '\0')
  {
    if (input[i] == ' ')
    {
      low__level_layer_data[j].size = k;
      j++;
      k = 0;
    }
    else
    {
      low__level_layer_data[j].data[k] = input[i];
      k++;
    }
    i++;
  }
  low__level_layer_data[j].size = k;
  return j + 1;
}

int main(void)
{
  int size, i, j;
  Byte low__level_layer_data[80];

  for (i = 0; i < 80; i++)
  {
    low__level_layer_data[i].size = 0;
    for (j = 0; j < 8; j++)
    {
      low__level_layer_data[i].data[j] = '\0';
    }
  }

  size = receive_low_level_layer(low__level_layer_data);

  for (i = 0; i < size; i++)
  {
    for (j = 0; j < low__level_layer_data[i].size; j++)
    {
      printf("%c", low__level_layer_data[i].data[j]);
    }

    printf(" ");
  }

  return 0;
}