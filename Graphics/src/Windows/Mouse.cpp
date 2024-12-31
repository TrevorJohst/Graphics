#include "Windows/Mouse.h"

/* ======================================================================================================= */
/*                           [PUBLIC] Mouse::Event                                                         */
/* ======================================================================================================= */

//////////////////////////////////////////////////////////////////
// [PUBLIC] Construct an invalid mouse event
Mouse::Event::Event() noexcept : type( Type::INVALID ), state{ false, false, false }, pos( 0, 0 ) {}

//////////////////////////////////////////////////////////////////
// [PUBLIC] Constructs a mouse event of specified type and state
Mouse::Event::Event( 
    Type         type, 
    const Mouse& parent ) noexcept 
    : 
    type( type ), 
    state( parent.state ), 
    pos( parent.pos ) 
{}

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns true if the event was valid
bool Mouse::Event::IsValid() const noexcept { return type != Type::INVALID; }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns the type enum of this event
Mouse::Event::Type Mouse::Event::GetType() const noexcept { return type; }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns the position of the mouse at the time of event
Vec2<int> Mouse::Event::GetPos() const noexcept { return pos; }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns the pressed state of the LMB, RMB, and MMB at
//      the time of event
Mouse::State Mouse::Event::GetState() const noexcept { return state; }

/* ======================================================================================================= */
/*                           [PUBLIC] Mouse                                                                */
/* ======================================================================================================= */

//////////////////////////////////////////////////////////////////
// [PUBLIC] Pops and returns the first mouse event from the queue,
//      returns nothing if it is empty
std::optional<Mouse::Event> Mouse::Pop() noexcept
{
    // Return nothing if queue is empty
    if ( buffer.empty() )
        return std::nullopt;

    // Remove and return the front element
    Mouse::Event e = buffer.front();
    buffer.pop();
    return e;
}

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns true if the mouse event queue is empty
bool Mouse::IsEmpty() const noexcept { return buffer.empty(); }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Clears the mouse event queue
void Mouse::Clear() noexcept { buffer = std::queue<Event>(); }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns the current position of the mouse as a Vec2
Vec2<int> Mouse::GetPos() const noexcept { return pos; }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns the pressed state of the left, right, and
//      middle mouse buttons
Mouse::State Mouse::GetState() const noexcept { return state; }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns true if any mouse buttons are pressed
bool Mouse::AnyPressed() const noexcept { return state.left || state.right || state.middle; }

//////////////////////////////////////////////////////////////////
// [PRIVATE] Adds a mouse move event to queue and updates position
void Mouse::OnMouseMove( int x, int y ) noexcept
{
    // Update mouse position, push into the queue, and trim back to max size
    pos = { x, y };
    buffer.emplace( Event::Type::MOVE, *this );
    TrimBuffer();
}

//////////////////////////////////////////////////////////////////
// [PRIVATE] Adds a LMB pressed event to the queue
void Mouse::OnLeftPress() noexcept
{
    // Update mouse state, push into the queue, and trim back to max size
    state.left = true;
    buffer.emplace( Event::Type::LEFT_PRESS, *this );
    TrimBuffer();
}

//////////////////////////////////////////////////////////////////
// [PRIVATE] Adds a LMB released event to the queue
void Mouse::OnLeftRelease() noexcept
{
    // Update mouse state, push into the queue, and trim back to max size
    state.left = false;
    buffer.emplace( Event::Type::LEFT_RELEASE, *this );
    TrimBuffer();
}

//////////////////////////////////////////////////////////////////
// [PRIVATE] Adds a RMB pressed event to the queue
void Mouse::OnRightPress() noexcept
{
    // Update mouse state, push into the queue, and trim back to max size
    state.right = true;
    buffer.emplace( Event::Type::RIGHT_PRESS, *this );
    TrimBuffer();
}

//////////////////////////////////////////////////////////////////
// [PRIVATE] Adds a RMB released event to the queue
void Mouse::OnRightRelease() noexcept
{
    // Update mouse state, push into the queue, and trim back to max size
    state.right = false;
    buffer.emplace( Event::Type::RIGHT_RELEASE, *this );
    TrimBuffer();
}

//////////////////////////////////////////////////////////////////
// [PRIVATE] Adds a MMB pressed event to the queue
void Mouse::OnMiddlePress() noexcept
{
    // Update mouse state, push into the queue, and trim back to max size
    state.middle = true;
    buffer.emplace( Event::Type::MIDDLE_PRESS, *this );
    TrimBuffer();
}

//////////////////////////////////////////////////////////////////
// [PRIVATE] Adds a MMB released event to the queue
void Mouse::OnMiddleRelease() noexcept
{
    // Update mouse state, push into the queue, and trim back to max size
    state.middle = false;
    buffer.emplace( Event::Type::MIDDLE_RELEASE, *this );
    TrimBuffer();
}

//////////////////////////////////////////////////////////////////
// [PRIVATE] Accumulates wheel delta and adds events when needed
void Mouse::OnWheelDelta( float stepDelta ) noexcept
{
    // Accumulate the wheel delta
    wheelDeltaCarry += stepDelta;

    // Buffer wheel events until delta is below one step
    while ( wheelDeltaCarry >= 1.0f )
    {
        wheelDeltaCarry -= 1.0f;
        OnWheelUp();
    }
    while ( wheelDeltaCarry <= -1.0f )
    {
        wheelDeltaCarry += 1.0f;
        OnWheelDown();
    }
}

//////////////////////////////////////////////////////////////////
// [PRIVATE] Adds a wheel up event to the queue
void Mouse::OnWheelUp() noexcept
{
    // Push into the queue and trim back to max size
    buffer.emplace( Event::Type::WHEEL_UP, *this );
    TrimBuffer();
}

//////////////////////////////////////////////////////////////////
// [PRIVATE] Adds a wheel down event to the queue
void Mouse::OnWheelDown() noexcept
{
    // Push into the queue and trim back to max size
    buffer.emplace( Event::Type::WHEEL_DOWN, *this );
    TrimBuffer();
}

//////////////////////////////////////////////////////////////////
// [PRIVATE] Clears the current state of all buttons
void Mouse::ClearState() noexcept { state = { false, false, false }; }

//////////////////////////////////////////////////////////////////
// [PRIVATE] Pops from the queue to get below maximum size
void Mouse::TrimBuffer() noexcept
{
    // Pop until buffer is valid size again
    while ( buffer.size() > bufferSize )
        buffer.pop();
}
