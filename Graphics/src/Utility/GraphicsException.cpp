#include "Utility/GraphicsException.h"
#include <sstream>

// =============================
// GraphicsException definitions
// =============================

//////////////////////////////////////////////////////////////////
// Constructs a generic GraphicsException
GraphicsException::GraphicsException( 
    int         line, 
    const char* file ) noexcept
    :
    line( line ),
    file( file )
{}

//////////////////////////////////////////////////////////////////
// Human readable error string recovered from exception
const char* GraphicsException::what() const noexcept
{
    // Format the error string and store in buffer
    std::ostringstream oss;
    oss << GetType() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();

    // Return pointer to persistent buffer string
    return whatBuffer.c_str();
}

//////////////////////////////////////////////////////////////////
// Returns the type of the exception, must be implemented
//      by child exception
const char* GraphicsException::GetType() const noexcept { return "Generic Graphics Exception"; }

//////////////////////////////////////////////////////////////////
// Returns the line that the exception was thrown on
int GraphicsException::GetLine() const noexcept { return line; }

//////////////////////////////////////////////////////////////////
// Returns the file that the exception was thrown from
const std::string& GraphicsException::GetFile() const noexcept { return file; }

//////////////////////////////////////////////////////////////////
// Returns a formatted string of the line and file origin
std::string GraphicsException::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "[File] " << file << std::endl
        << "[Line] " << line;
    return oss.str();
}
