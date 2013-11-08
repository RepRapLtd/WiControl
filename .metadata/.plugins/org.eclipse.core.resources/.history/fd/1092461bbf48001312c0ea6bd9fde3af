/*
 * Command.cpp
 *
 *  Created on: 27 Oct 2013
 *      Author: ensab
 */

#include "Control.h"

CommandBuffer::CommandBuffer()
{
  Init();
}

void CommandBuffer::Init()
{
  commandPointer = 0;
  readPointer = -1;
  inComment = false;
}

bool CommandBuffer::Put(char c)
{
  bool result = false;
  commandBuffer[commandPointer] = c;

  if(c == ';')
    inComment = true;

  if(c == '\n' || !c)
  {
    commandBuffer[commandPointer] = 0;
    Init();
    result = true;
  } else
  {
    if(!inComment)
      commandPointer++;
  }

  if(commandPointer >= COMMAND_LENGTH)
  {
    Serial.print("Command buffer length overflow.\n");
    commandPointer = 0;
    commandBuffer[0] = 0;
  }

  return result;
}

// Is 'c' in the G Code string?
// Leave the pointer there for a subsequent read.

bool CommandBuffer::Seen(char c)
{
  readPointer = 0;
  while(commandBuffer[readPointer])
  {
    if(commandBuffer[readPointer] == c)
      return true;
    readPointer++;
  }
  readPointer = -1;
  return false;
}

// Get a float after a G Code letter

float CommandBuffer::GetFValue()
{
  if(readPointer < 0)
  {
     Serial.print("Command: Attempt to read a GCode float before a search.\n");
     return 0.0;
  }
  float result = (float)strtod(&commandBuffer[readPointer + 1], 0);
  readPointer = -1;
  return result;
}

// This returns a pointer to the byte in the buffer where a
// string starts.  It assumes that a search has been done for 'S', so readPointer will
// point to the S.

char* CommandBuffer::GetString()
{
  if(readPointer < 0)
  {
	    Serial.print("Command: Attempt to read a string before a search.\n");
	    return commandBuffer;
  }
  char* result = &commandBuffer[readPointer + 1];
  readPointer = -1;
  return result;
}


// Get an long after a G Code letter

long CommandBuffer::GetLValue()
{
  if(readPointer < 0)
  {
    Serial.print("Command: Attempt to read a command long before a search.\n");
    return 0;
  }
  long result = strtol(&commandBuffer[readPointer + 1], 0, 0);
  readPointer = -1;
  return result;
}




