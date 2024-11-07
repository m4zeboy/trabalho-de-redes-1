#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define ESCAPE 0xFD;
#define FLAG 0xFE;

void readInput(char *protocolString, char *messageString)
{
  scanf("%4s %[^\n]", protocolString, messageString);
}

unsigned char *byteStuffing(char *messageString, unsigned short *_length)
{
  unsigned char *stuffedString;
  unsigned int length, i;
  stuffedString = (unsigned char *)malloc(1504 * sizeof(unsigned char));
  i = 0;
  length = 0;
  if (stuffedString != NULL)
  {
    while (messageString[i] != '\0')
    {
      char currentByte = messageString[i];
      if (currentByte == 0x7D || currentByte == 0x7E)
      {
        stuffedString[length++] = 0x7D;
        stuffedString[length++] = currentByte ^ 0x20;
      }
      else
      {
        stuffedString[length++] = currentByte;
      }
      i++;
    }
    stuffedString[length] = '\0';
    *_length = length;
  }
  return stuffedString;
}

unsigned short calculateChecksum(unsigned char *protocolBytes, char *messageString)
{
  int checksumDataLength, messageLength;
  unsigned char *checksumData;
  unsigned long sum;
  unsigned short checksum;
  sum = 0;
  messageLength = strlen(messageString);
  checksumDataLength = 1 + 1 + 2 + messageLength;
  checksumData = (unsigned char *)malloc(checksumDataLength * sizeof(unsigned char));
  if (checksumData != NULL)
  {
    int i, j;
    i = 0;
    checksumData[i++] = 0xFF;
    checksumData[i++] = 0x03;
    checksumData[i++] = protocolBytes[0];
    checksumData[i++] = protocolBytes[1];

    for (j = 0; j < messageLength; j++)
    {
      checksumData[i++] = messageString[j];
    }

    if (checksumDataLength % 2 != 0)
    {
      checksumData[i++] = 0x00;
      checksumDataLength++;
    }

    for (i = 0; i < checksumDataLength; i += 2)
    {
      unsigned short word = (checksumData[i] << 8) + checksumData[i + 1];
      sum += word;

      if (sum > 0xFFFF)
      {
        sum = (sum & 0xFFFF) + (sum >> 16);
      }
    }

    checksum = ~sum & 0xFFFF;
    return checksum;
  }
  return -1;
}

char *byteToBinary(char byte)
{
  int i;
  char *binaryString;
  binaryString = (char *)malloc(9 * sizeof(char));
  for (i = 7; i >= 0; i--)
  {
    binaryString[7 - i] = ((byte >> i) & 1) ? '1' : '0';
  }
  binaryString[8] = '\0';
  return binaryString;
}

int main()
{
  char protocolString[5], messageString[1501];
  unsigned char protocolBytes[2], *stuffedString, frame[1518];
  unsigned int protocolHexadecimal;
  unsigned short checksum, checksumBytes[2], stuffedLength, frameLength, i;

  readInput(protocolString, messageString);

  sscanf(protocolString, "%x", &protocolHexadecimal);

  protocolBytes[0] = protocolHexadecimal >> 8 & 0xFF;
  protocolBytes[1] = protocolHexadecimal & 0xFF;
  stuffedString = byteStuffing(messageString, &stuffedLength);

  checksum = calculateChecksum(protocolBytes, messageString);
  checksumBytes[0] = (checksum >> 8) & 0xFF;
  checksumBytes[1] = checksum & 0xFF;

  frameLength = 0;
  frame[frameLength++] = 0x7E;
  frame[frameLength++] = 0xFF;
  frame[frameLength++] = 0x03;
  frame[frameLength++] = protocolBytes[0];
  frame[frameLength++] = protocolBytes[1];

  for (i = 0; i < stuffedLength; i++)
  {
    frame[frameLength++] = stuffedString[i];
  }
  frame[frameLength++] = checksumBytes[0];
  frame[frameLength++] = checksumBytes[1];
  frame[frameLength++] = 0x7E;

  for (i = 0; i < frameLength; i++)
  {
    printf("%s ", byteToBinary(frame[i]));
  }

  return 0;
}