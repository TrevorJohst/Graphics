#pragma once
#include <queue>
#include <bitset>
#include <optional>

//////////////////////////////////////////////////////////////////
// @brief An interface with the keyboard for a given window
class Keyboard
{
    // Friends with Window to allow interfacing with Windows events
    friend class Window;

public:
    //////////////////////////////////////////////////////////////////
    // @brief Singular key event encoding type of event (press,
    //      release, invalid) and keycode pressed
    class Event
    {
    public:
        //////////////////////////////////////////////////////////////////
        // @brief Enum for key event types
        enum class Type { PRESS, RELEASE, INVALID };

    public:
        //////////////////////////////////////////////////////////////////
        // @brief Construct an invalid key event
        Event() noexcept;

        //////////////////////////////////////////////////////////////////
        // @brief Constructs a key event of specified type and keycode
        //
        // @param type: keyboard event type
        // @param keycode: Windows keycode of the event 
        Event( 
            Type          type, 
            unsigned char keycode ) noexcept;


        //////////////////////////////////////////////////////////////////
        // @brief Returns true if the event represents a press
        bool IsPress() const noexcept;

        //////////////////////////////////////////////////////////////////
        // @brief Returns true if the event represents a release
        bool IsRelease() const noexcept;

        //////////////////////////////////////////////////////////////////
        // @brief Returns true if the event was valid (press or release)
        bool IsValid() const noexcept;

        //////////////////////////////////////////////////////////////////
        // @brief Returns the keycode corresponding to this event
        unsigned char GetCode() const noexcept;

    private:
        Type type;
        unsigned char code;
    };

public:
    //////////////////////////////////////////////////////////////////
    // @brief Class does not require instantiation
    Keyboard() = default;

    //////////////////////////////////////////////////////////////////
    // @brief Copy constructor is deleted, one keyboard per window
    Keyboard( const Keyboard& ) = delete;

    //////////////////////////////////////////////////////////////////
    // @brief Assignment operator is deleted, one keyboard per window
    Keyboard& operator=( const Keyboard& ) = delete;


    //////////////////////////////////////////////////////////////////
    // @brief Pops and returns the first key from the queue, returns
    //      nothing if it is empty
    std::optional<Event> PopKey() noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Returns true if the key queue is empty
    bool KeyIsEmpty() const noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Clears the key queue
    void ClearKey() noexcept;


    //////////////////////////////////////////////////////////////////
    // @brief Pops and returns the first char from the queue, returns
    //      nothing if it is empty
    std::optional<char> PopChar() noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Returns true if the char queue is empty
    bool CharIsEmpty() const noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Clears the char queue
    void ClearChar() noexcept;


    //////////////////////////////////////////////////////////////////
    // @brief Returns true if the queried keycode is currently pressed
    //
    // @param keycode: Windows keycode to query
    bool KeyIsPressed( unsigned char keycode ) const noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Clears both the key and char queues
    void Clear() noexcept;


    //////////////////////////////////////////////////////////////////
    // @brief Enables autorepeat so holding a key will add multiple
    //      events to the key queue
    void EnableAutorepeat() noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Disables autorepeat so holding a key wont add multiple
    //      events to the key queue
    void DisableAutorepeat() noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Returns true if autorepeat is currently enabled
    bool AutorepeatIsEnabled() const noexcept;

private:
    //////////////////////////////////////////////////////////////////
    // @brief Adds a key pressed event to the queue
    //
    // @param keycode: Windows keycode pressed
    void OnKeyPressed( unsigned char keycode ) noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Adds a key released event to the queue
    //
    // @param keycode: Windows keycode released
    void OnKeyReleased( unsigned char keycode ) noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Adds a char to the queue
    //
    // @param character: character registered as pressed
    void OnChar( char character ) noexcept;


    //////////////////////////////////////////////////////////////////
    // @brief Clears the current state of all keys
    void ClearState() noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Pops from the queue to get below maximum size
    //
    // @param buffer: queue to trim down to max size
    template<typename T>
    static void TrimBuffer( std::queue<T>& buffer ) noexcept;

private:
    static constexpr unsigned int nKeys = 256u;
    static constexpr unsigned int bufferSize = 16u;
    bool autorepeatEnabled = false;
    std::bitset<nKeys> keyStates;
    std::queue<Event> keyBuffer;
    std::queue<char> charBuffer;
};
