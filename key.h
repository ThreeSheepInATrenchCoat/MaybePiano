
#if !defined(__INC_KEY_H)
#define __INC_KEY_H

#include <stdint.h>

/**
 * @class Key
 * @brief Provides an interface for a single key
 */
class Key
{
public:
  // Constructors/Destructor
  Key(const char *Name, int Command, int Pin);
  ~Key();

  /**
   * @brief Tells driver key was pressed
   */
  void Press(void);

  /**
   * @brief Forces driver to depress key
   */
  void DePress(void);

  /**
   * @brief Updates key instance
   */
  void Update(void);

  /**
   * @brief Gets key name
   * @return Pointer to name C-style string
   */
  const char * GetName(void);

  /**
   * @brief Checks if command number matches
   * @retval true Commands match
   */
  bool CommandMatch(int Command);

private:

  // Amount of time to keep a key on for in milliseconds
  static constexpr int32_t s_ReleaseTimeMs = 50;

  // Key name. Usually the note
  char m_Name[16];

  // Command value
  const int m_Command;

  // Arduino pin reference (Not ATMega328)
  const int m_Pin;
  
  // System time when key press occured
  uint32_t m_PressTime;
};

#endif /* __INC_KEY_H */