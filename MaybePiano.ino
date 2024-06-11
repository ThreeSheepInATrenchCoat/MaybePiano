/**
 * @file  
 * @brief Toggles FETs based on received serial comms
 * @note  Add more keys by adding them to the vector in setup()
 */

#include <Arduino.h>

#include "vector.h"

#include "key.h"

#define COMMAND_PACKET_SIZE_BYTES   (3)

// Command values
const int CommandNoteOn = 144;

const int CommandNoteC4 = 60;
const int PinNoteC4 = 5;

const int CommandNoteC4s = 61;
const int PinNoteC4s = 4;

const int CommandNoteD4 = 62;
const int PinNoteD4 = 3;

const int CommandNoteD4s = 63;
const int PinNoteD4s = 2;


// Allocate keys on heap so we can call parameterised constructors
// Given this memory is never freed we can do this without risking fragmentation
not_std::vector<Key*> Keys;

/**
 * @brief   Actions a full 3-byte command packet
 * @param   pRxBuffer   Pointer to receive buffer
 * @retval  true        Packet contents valid, key triggered
 * @retval  false       Packet contents invalid, key not triggered
 */
bool PressKey(uint8_t *pRxBuffer)
{
  // Check command byte is correct and velocity > 0
  bool IsValid = (pRxBuffer[0] == CommandNoteOn);
  if (IsValid && pRxBuffer[2])
  {
    IsValid = false;

    // Find corresponding key and press
    for (size_t Idx = 0; Idx < Keys.size(); Idx++)
    {
      if (Keys.at(Idx)->CommandMatch(pRxBuffer[1]))
      {
        // Key found, start sequence
        Keys.at(Idx)->Press();
        IsValid = true;
        Serial.print("Key press ");
        Serial.println(Keys.at(Idx)->GetName());
        break;
      }
    }
  }

  return IsValid;
}

/**
 * @brief   Blinks the built-in LED at a 1Hz rate
 * @note    If using pin 13 for anything else, disable this function!
 */
void BlinkLED(void)
{
  static uint32_t BlinkTime = 0;
  static bool flip = false;

  uint32_t CurrentTime = millis();

  if ((int32_t)(CurrentTime - BlinkTime) >= 500)
  {
    digitalWrite(LED_BUILTIN, flip);
    flip = !flip;
    BlinkTime = CurrentTime;
  }
}

void setup(void) 
{
  Serial.begin(115200);
  Serial.println("Start");

  pinMode(LED_BUILTIN, OUTPUT);

  // Add any new notes here in this format
  // Parameters are:
  //  -Key note name as a C-style string
  //  -Serial command value to trigger note
  //  -Arduino pin number
  Keys.push_back(new Key("C4", CommandNoteC4, PinNoteC4));
  Keys.push_back(new Key("C4s", CommandNoteC4s, PinNoteC4s));
  Keys.push_back(new Key("D4", CommandNoteD4, PinNoteD4));
  Keys.push_back(new Key("D4s", CommandNoteD4s, PinNoteD4s));
}

void loop(void) 
{

  // Keep doing this - tells us if the program has crashed
  BlinkLED();

  // Serial receive buffer
  // Byte 0 is the command
  // Byte 1 is the note
  // Byte 2 is the velocity
  uint8_t RxBuffer[COMMAND_PACKET_SIZE_BYTES];
  uint8_t RxBufferPos = 0;

  // Read any bytes from serial stream into local buffer, but do not overflow
  while (Serial.available() && (RxBufferPos < COMMAND_PACKET_SIZE_BYTES))
  {
    RxBuffer[RxBufferPos++] = Serial.read();
  }

  // Check if full command packet received
  if (RxBufferPos >= COMMAND_PACKET_SIZE_BYTES)
  {
    if (!PressKey(RxBuffer))
    {  
      // There may have been some desync here
      // So sanity check the buffer position and reset if necessary
      RxBuffer[0] = CommandNoteOn;
      if (RxBuffer[1] == CommandNoteOn)
      {
        RxBuffer[1] = RxBuffer[2];
        RxBufferPos = 2;
      }
      else if (RxBuffer[2] == CommandNoteOn)
      {
        RxBufferPos = 1;
      }
    }
    RxBufferPos = 0;
  }

  // Update all keys
  for (size_t Idx = 0; Idx < Keys.size(); Idx++)
  {
    Keys.at(Idx)->Update();
  }
}
