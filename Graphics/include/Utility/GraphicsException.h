#pragma once
#include <exception>
#include <string>

//////////////////////////////////////////////////////////////////
// @brief Custom exception types with additional debug information
class GraphicsException : public std::exception
{
public:
    //////////////////////////////////////////////////////////////////
    // @brief Constructs a generic GraphicsException
    //
    // @param line, file: where the exception is thrown from
    GraphicsException( int line, const char* file ) noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Human readable error string recovered from exception
    const char* what() const noexcept override;

    //////////////////////////////////////////////////////////////////
    // @brief Returns the type of the exception, must be implemented
    //      by child exception
    virtual const char* GetType() const noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Returns the line that the exception was thrown on
    int GetLine() const noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Returns the file that the exception was thrown from
    const std::string& GetFile() const noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Returns a formatted string of the line and file origin
    std::string GetOriginString() const noexcept;

private:
    int line;
    std::string file;

protected:
    mutable std::string whatBuffer;
};