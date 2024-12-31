#include "Windows/Keyboard.h"

/* ======================================================================================================= */
/*                           [PUBLIC] Keyboard::Event                                                      */
/* ======================================================================================================= */

//////////////////////////////////////////////////////////////////
// [PUBLIC] Construct an invalid key event
Keyboard::Event::Event() noexcept : type( Type::INVALID ), code( 0u ) {}

//////////////////////////////////////////////////////////////////
// [PUBLIC] Constructs a key event of specified type and keycode
Keyboard::Event::Event( Type type, unsigned char keycode ) noexcept : type( type ), code( keycode ) {}

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns true if the event represents a press
bool Keyboard::Event::IsPress() const noexcept { return type == Type::PRESS; }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns true if the event represents a release
bool Keyboard::Event::IsRelease() const noexcept { return type == Type::RELEASE; }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns true if the event was valid (press or release)
bool Keyboard::Event::IsValid() const noexcept { return type != Type::INVALID; }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns the keycode corresponding to this event
unsigned char Keyboard::Event::GetCode() const noexcept { return code; }

/* ======================================================================================================= */
/*                           [PUBLIC] Keyboard                                                             */
/* ======================================================================================================= */

//////////////////////////////////////////////////////////////////
// [PUBLIC] Pops and returns a the first key event from the queue,
//          returns nothing if the queue is empty
std::optional<Keyboard::Event> Keyboard::PopKey() noexcept
{
    // Return nothing if queue is empty
    if ( keyBuffer.empty() )
        return std::nullopt;

    // Remove and return the front element
    Keyboard::Event e = keyBuffer.front();
    keyBuffer.pop();
    return e;
}

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns true if the key queue is empty
bool Keyboard::KeyIsEmpty() const noexcept { return keyBuffer.empty(); }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Clears the key queue
void Keyboard::ClearKey() noexcept { keyBuffer = std::queue<Event>(); }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Pops and returns the first char from the queue, 
//          returns nothing if it is empty
std::optional<char> Keyboard::PopChar() noexcept
{
    // Return nothing if queue is empty
    if ( charBuffer.empty() )
        return std::nullopt;

    // Remove and return the front element
    char charCode = charBuffer.front();
    charBuffer.pop();
    return charCode;
}

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns true if the char queue is empty
bool Keyboard::CharIsEmpty() const noexcept { return charBuffer.empty(); }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Clears the char queue
void Keyboard::ClearChar() noexcept { charBuffer = std::queue<char>(); }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns true if the queried keycode is currently pressed
bool Keyboard::KeyIsPressed( unsigned char keycode ) const noexcept { return keyStates[keycode]; }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Clears both the key and char queues
void Keyboard::Clear() noexcept
{
    ClearKey();
    ClearChar();
}

//////////////////////////////////////////////////////////////////
// [PUBLIC] Enables autorepeat so holding a key will add multiple
//      events to the key queue
void Keyboard::EnableAutorepeat() noexcept { autorepeatEnabled = true; }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Disables autorepeat so holding a key wont add multiple
//      events to the key queue
void Keyboard::DisableAutorepeat() noexcept { autorepeatEnabled = false; }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns true if autorepeat is currently enabled
bool Keyboard::AutorepeatIsEnabled() const noexcept { return autorepeatEnabled; }

//////////////////////////////////////////////////////////////////
// [PRIVATE] Adds a key pressed event to the queue
void Keyboard::OnKeyPressed( unsigned char keycode ) noexcept
{
    // Set the key's state to true, push it into the queue, and trim back to max size
    keyStates[keycode] = true;
    keyBuffer.emplace( Event::Type::PRESS, keycode );
    TrimBuffer( keyBuffer );
}

//////////////////////////////////////////////////////////////////
// [PRIVATE] Adds a key released event to the queue
void Keyboard::OnKeyReleased( unsigned char keycode ) noexcept
{
    // Set the key's state to false, push it into the queue, and trim back to max size
    keyStates[keycode] = false;
    keyBuffer.emplace( Event::Type::RELEASE, keycode );
    TrimBuffer( keyBuffer );
}

//////////////////////////////////////////////////////////////////
// [PRIVATE] Adds a char to the queue
void Keyboard::OnChar( char character ) noexcept
{
    // Push the char into the buffer and trim back to max size
    charBuffer.push( character );
    TrimBuffer( charBuffer );
}

//////////////////////////////////////////////////////////////////
// [PRIVATE] Clears the current state of all keys
void Keyboard::ClearState() noexcept { keyStates.reset(); }

//////////////////////////////////////////////////////////////////
// [PRIVATE] Pops from the queue to get below maximum size
template<typename T>
void Keyboard::TrimBuffer( std::queue<T>& buffer ) noexcept 
{ 
    // Pop until buffer is valid size again
    while ( buffer.size() > bufferSize ) 
        buffer.pop(); 
}