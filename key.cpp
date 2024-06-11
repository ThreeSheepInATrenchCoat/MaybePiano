
#include "key.h"

#include <Arduino.h>

// Constructors/Destructor
Key::Key(const char *Name, int Command, int Pin) :
  m_Command(Command),
  m_Pin(Pin),
  m_PressTime(0)
{
  strcpy(m_Name, Name);
  pinMode(m_Pin, OUTPUT);
  digitalWrite(m_Pin, LOW);
}

Key::~Key()
{
  digitalWrite(m_Pin, LOW);
}

void Key::Press(void)
{
  m_PressTime = millis();
  digitalWrite(m_Pin, HIGH);
}

void Key::DePress(void)
{
  digitalWrite(m_Pin, LOW);
  m_PressTime = 0;
}

void Key::Update(void)
{
  if (m_PressTime != 0)
  {
    uint32_t CurrentTime = millis();
    if ((int32_t)(CurrentTime - m_PressTime) >= s_ReleaseTimeMs)
    {
      DePress();
    }
  }
}

const char * Key::GetName(void)
{
  return m_Name;
}

bool Key::CommandMatch(int Command)
{
  return Command == m_Command;
}