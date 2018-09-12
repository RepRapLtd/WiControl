/*
 * Command.h
 *
 *  Created on: 27 Oct 2013
 *      Author: ensab
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#define COMMAND_LENGTH 100

class CommandBuffer
{
  public:
    CommandBuffer();
    void Init();
    bool Put(char c);
    void Fill(char* s);
    bool Seen(char c);
    float GetFValue();
    int GetIValue();
    long GetLValue();
    char* GetString();
    char* Buffer();

  private:

    char commandBuffer[COMMAND_LENGTH];
    int commandPointer;
    int readPointer;
    bool inComment;
};


// Get an Int after a G Code letter

inline int CommandBuffer::GetIValue()
{
  return (int)GetLValue();
}

inline char* CommandBuffer::Buffer()
{
  return commandBuffer;
}





#endif /* COMMAND_H_ */

