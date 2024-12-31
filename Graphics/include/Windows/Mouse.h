#pragma once
#include "Utility/Vec2.h"
#include <queue>
#include <optional>

//////////////////////////////////////////////////////////////////
// @brief An interface with the mouse for a given window
class Mouse
{
    // Friends with Window to allow interfacing with Windows events
    friend class Window;

public:
    //////////////////////////////////////////////////////////////////
    // @brief Simple struct storing the pressed state of the left,
    //      right, and middle mouse buttons
    struct State
    {
        uint8_t left : 1;
        uint8_t right : 1;
        uint8_t middle : 1;
    };

    //////////////////////////////////////////////////////////////////
    // @brief Singular mouse event encoding type of event, click
    //      status, and position at time of event
    class Event
    {
    public:
        //////////////////////////////////////////////////////////////////
        // @brief Enum for mouse event types
        enum class Type {
            LEFT_PRESS, LEFT_RELEASE, RIGHT_PRESS, RIGHT_RELEASE, MIDDLE_PRESS, MIDDLE_RELEASE, 
            WHEEL_UP, WHEEL_DOWN, MOVE, INVALID
        };

    public:
        //////////////////////////////////////////////////////////////////
        // @brief Construct an invalid mouse event
        Event() noexcept;

        //////////////////////////////////////////////////////////////////
        // @brief Constructs a mouse event of specified type and state
        //
        // @param type: mouse event type
        // @param parent: mouse class this event occurs on, used to record
        //      the state at the time of the event
        Event( 
            Type         type, 
            const Mouse& parent ) noexcept;


        //////////////////////////////////////////////////////////////////
        // @brief Returns true if the event was valid
        bool IsValid() const noexcept;

        //////////////////////////////////////////////////////////////////
        // @brief Returns the type enum of this event
        Type GetType() const noexcept;

        //////////////////////////////////////////////////////////////////
        // @brief Returns the position of the mouse at the time of event
        Vec2<int> GetPos() const noexcept;

        //////////////////////////////////////////////////////////////////
        // @brief Returns the pressed state of the LMB, RMB, and MMB at
        //      the time of event
        State GetState() const noexcept;

    private:
        Type type;
        State state;
        Vec2<int> pos;
    };

public:
    //////////////////////////////////////////////////////////////////
    // @brief Class does not require instantiation
    Mouse() = default;

    //////////////////////////////////////////////////////////////////
    // @brief Copy constructor is deleted, one mouse per window
    Mouse( const Mouse& ) = delete;

    //////////////////////////////////////////////////////////////////
    // @brief Assignment operator is deleted, one mouse per window
    Mouse& operator=( const Mouse& ) = delete;


    //////////////////////////////////////////////////////////////////
    // @brief Pops and returns the first mouse event from the queue,
    //      returns nothing if it is empty
    std::optional<Event> Pop() noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Returns true if the mouse event queue is empty
    bool IsEmpty() const noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Clears the mouse event queue
    void Clear() noexcept;


    //////////////////////////////////////////////////////////////////
    // @brief Returns the current position of the mouse as a Vec2
    Vec2<int> GetPos() const noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Returns the pressed state of the left, right, and
    //      middle mouse buttons
    State GetState() const noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Returns true if any mouse buttons are pressed
    bool AnyPressed() const noexcept;

private:
    //////////////////////////////////////////////////////////////////
    // @brief Adds a mouse moved event to queue and updates position
    //
    // @param x: x coordinate of the mouse
    // @param y: y coordinate of the mouse
    void OnMouseMove( int x, int y ) noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Adds a LMB pressed event to the queue
    void OnLeftPress() noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Adds a LMB released event to the queue
    void OnLeftRelease() noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Adds a RMB pressed event to the queue
    void OnRightPress() noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Adds a RMB released event to the queue
    void OnRightRelease() noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Adds a MMB pressed event to the queue
    void OnMiddlePress() noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Adds a MMB released event to the queue
    void OnMiddleRelease() noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Accumulates wheel delta and adds events when needed
    //
    // @param stepDelta: a scaled version of the delta where 1 is a
    //      single step, i.e. stepDelta = delta / DELTA_INCREMENT
    void OnWheelDelta( float stepDelta ) noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Adds a wheel up event to the queue
    void OnWheelUp() noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Adds a wheel down event to the queue
    void OnWheelDown() noexcept;


    //////////////////////////////////////////////////////////////////
    // @brief Clears the current state of all buttons
    void ClearState() noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Pops from the queue to get below maximum size
    void TrimBuffer() noexcept;

private:
    static constexpr unsigned int bufferSize = 16u;
    State state{ false, false, false };
    Vec2<int> pos{ 0, 0 };
    float wheelDeltaCarry = 0.0f;
    std::queue<Event> buffer;
};