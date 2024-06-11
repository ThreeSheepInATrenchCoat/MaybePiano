
Disclaimer: Everything in here is meant to be constructive. Please don't take anything badly! We were all here at some point.

Also, do not be put off by hiw much is going on. C++ is an absolutely massive language and I'm yet to meet someone who actually knows all of it. Find what works for you and use that.

These are general comments to do with style and techniques.

# Comments

Try to avoid useless comments. Comments should add context, not tell me what the line is doing. I can see that by reading the code.

Example:
```C++
if (noteByte == 60 && velocityByte > 0)  //if note == C4 and velocity > 0
```

The comment here adds no value. Instead, this would be a bit better:

```C++
// Check if we are interested in command
if (commandByte == noteOn) {

    // Check for valid key press and action
    if (noteByte == 60 && velocityByte > 0) {
        digitalWrite(5, HIGH);                 
    }
    if (noteByte == 61 && velocityByte > 0) {  
        digitalWrite(4, HIGH);           
    }
    if (noteByte == 62 && velocityByte > 0) { 
        digitalWrite(3, HIGH);      
    }
    if (noteByte == 63 && velocityByte > 0) {
        digitalWrite(2, HIGH);       
    }
}
```

Too many comments is not good either! Its hard for me to add meaningful comments here without knowing the context.

Comments should only be added if they add some kind of value taht cannot be otherwise inferred easily.

# Repetition

Factor out common bits if you can. For example:

```C++
// Check if we are interested in command
if ((commandByte == noteOn) && (velocityByte > 0)) {

    // Only action command if 
    if (noteByte == 60) {
        digitalWrite(5, HIGH);                 
    }
    if (noteByte == 61) {  
        digitalWrite(4, HIGH);           
    }
    if (noteByte == 62) { 
        digitalWrite(3, HIGH);      
    }
    if (noteByte == 63) {
        digitalWrite(2, HIGH);       
    }
}
```

There is no value in checking `velocityByte` on every iteration; it adds overhead and worsens readability.

# Magic Numbers

There are some constant values in your code (gewnerally known as magic numbers) that will not change for the lifetime of the program. We can mark these with the `const` or `constexpr` type qualifier.

```C++
const byte noteOn = 144; //note on message
const int hold = 50;  //solenoid sustain time
```

What this does is tell the micro we do not want these values to ever change. If you do attempt to change them, the program will usually crash, which in this case is a good thing! We want programs to obviously fail when something goes wrong.

Furthermore, magic numbers should not exit in their raw form. Either define them as `const` variables, or as macros using `#define`.

```C++
const int noteC4 = 60; 
const int noteC4s = 61; 
const int noteD4 = 62; 
const int noteD4s = 63; 
```

Then the following code becomes:

```C++
// Check if we are interested in command
if ((commandByte == noteOn) && (velocityByte > 0)) {

    // Only action command if 
    if (noteByte == noteC4) {
        digitalWrite(5, HIGH);                 
    }
    if (noteByte == noteC4s) {  
        digitalWrite(4, HIGH);           
    }
    if (noteByte == noteD4) { 
        digitalWrite(3, HIGH);      
    }
    if (noteByte == noteD4s) {
        digitalWrite(2, HIGH);       
    }
}
```

See the benefit? The code here is now much more readable and self explanatory.

# Qualifiers

You asked the question in the thread what do `constexpr` and `static` mean. 

`constexpr` means the value is fixed at compile time and the value can therefore not be changed by the program. It is used to hardcode magic numbers mainly.

`static` has multiple meanings in C++, depending on context. For the context we're using it here, it keeps a variable in memory even after it has gone out of scope.

For example:

```C++
void myFunc() {
    int myVar = 0;
    static int myStaticVar = 0;

    myVar++;
    myStaticVar++;

    Serial.println(myVar);
    Serial.println(myStaticVar);
}

void loop() {
    myFunc();
}
```


What would the output of this be? The answer is that `myVar` will always be 1 while `myStaticVar` will increase by 1 on every loop.

See how that works? Variables generally only exist within their scope, so when `myFunc` is called, a variable called `myVar` is created. When we leave `myFunc`, `myVar` is destoryed. This happens every time.

Therefore, everytime we enter `myFunc`, we create a brand new instance of `myVar`. Hence it will always print 1;

Byt for `myStaticVar`, we're explicitly telling the program to remember its value even when we exit the function. Therefore when we come back the value has been reserved, so it is incrementing on ever loop.

# Blocking

We can categorise code into 2 types, `blocking` and `non-blocking`. 

Blocking means that nothing else can run until that bit of code is complete. This is not ideal and should be avoided.

Non-blocking means that the task will allow the rest of the program to run and not hog resources until it is complete.

Consider the following examples:

```C++
void Blink() {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, High);
    delay(500);
}

void loop() {
    // Blink LED at 1Hz rate
    Blink();
    // Do something else
    SomeTask();
}
```

```C++
void Blink() {
    // Blink LED at 1Hz rate
    int CurrentTime = millis();

    static int ChangeTime = 0;
    static bool LEDState = false;

    if (CurrentTime - ChangeTime >= 500) {
        digitalWrite(LED_BUILTIN, LEDState);
        LEDState = !LEDState;
        ChangeTime = CurrentTime;
    }
}

void loop() {
    // Blink LED at 1Hz rate
    Blink();
    // Do something else
    SomeTask();
}
```

In example 1, nothing can execute until `Blink` has been finished. This is a `blocking` implementation. That means for an entire second, the MCU cannot do anything else. This is an eternity in computer time!

Therefore, example 2 is much better. This is a `non-blocking` implementation. We simply check the LED on every loop and when enough time has passed we toggle the state.

To write `non-blocking` code, we need a way to remember what we were doing when we come back. In example 2 this is done via the 2 variables, `ChangeTime` and `LEDState`. 

For a good general purpose technique, I'd recommend looking up state machines.

# Scalability

As you probably already know, the big problem with this is adding more keys. What happens if you need 100? adding more and more chained `if` statements is not a practical solution.

The obvious solution is object-orientation. I'd recommend reading up on it, `C++` is primarily meant as an OOP (Object-Oriented Programming) language.

Look, I'm not going to sugar coat this; there is an awful lot to take in here and the only way you'll learn it is by constant doing. Start with a C++ tutorial and do any exercises they give you. 
The example I've sent will help, but going off your comments is maybe a bit more advanced than what you will be able to undertsnad at this point.

# Types

You're familiar with the concept of types, such as `int`, `byte`, `char`, `long` etc. `C++` is a strongly-typed language, which means every variable must be declared with its type.

Now there's a bit of a problem here. An `int` in AVR is 2-bytes, meaning an `unsigned int` can hold a maximum value of 65535. On ARM, however, an `int` is 4-bytes wide, meaning a max value of about 4 billion.

As you can tell, this can get confusing. Types like `int`, `byte`, `char` etc. _do not gaurantee size_.

The solution here is the `stdint` library. It introduces types that gaurantee size across all platforms.

```C++
int8_t mySignedChar; // 1 byte, signed
uint8_t myUnsignedChar; // 1 byte, unsigned

int16_t mySignedShort; // 2 bytes, signed
uint16_t myUnsignedShort; // 2 bytes, unsigned

int32_t myUnsignedInt; // 4 bytes, signed
uint32_t myUnsignedInt; // 4 bytes, unsigned

int64_t mySignedLongLong; // 8 bytes, signed
uint64_t myUnsignedLongLong; // 8 bytes, unsigned
```

