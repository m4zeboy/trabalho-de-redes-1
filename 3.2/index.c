#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 50000
#define FLAG "01111110"
#define ADDRESS_SIZE 8
#define CONTROL_SIZE 8
#define PROTOCOL_SIZE 16
#define CHECKSUM_SIZE 16
#define MAX_FRAME_SIZE 1518 * 8

void readInput(char *stream)
{
  fgets(stream, MAX_SIZE, stdin);
}

char *getContentBetween(char *start, char *end)
{
  size_t size;
  char *result;
  if (start == NULL || end == NULL || end < start)
  {
    return NULL;
  }

  size = end - start;
  result = (char *)malloc(size + 1);

  if (result == NULL)
  {
    return NULL;
  }

  memcpy(result, start, size);
  result[size] = '\0';

  return result;
}

int extractFrames(char *input, char *frames[100])
{
  char *start, *current, *end, *possibleFrame;
  short frameIndex = 0;
  current = input;
  while (current)
  {
    start = strstr(current, FLAG);
    current = current + 8;
    end = strstr(current, FLAG);
    possibleFrame = getContentBetween(current, end);
    current = end;
    if (possibleFrame && strlen(possibleFrame) > 0)
    {
      frames[frameIndex] = possibleFrame;
      frameIndex++;
    }
  }

  return frameIndex;
}

int main(void)
{
  char bit_stream[MAX_SIZE], *start, *frames[100];
  short frameCount, f;
  /*
  01111110
  11111111
  00000011
  11000000 00100001
  01010010 01100101 01100100 01100101 01110011
  00010111 00001111
  01111110
  */
  readInput(bit_stream);
  start = strstr(bit_stream, FLAG);
  frameCount = extractFrames(bit_stream, frames);
  printf("f: %hd\n", frameCount);
  for (f = 0; f < frameCount; f++)
  {
    printf("%s\n", frames[f]);
  }
  return 0;
}