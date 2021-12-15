#pragma once

#include <fstream>
#include <iostream>

class Exception {
   protected:
    std::string m_error;

   public:
    Exception(std::string error) : m_error(error) {}

    const char *what() const noexcept { return m_error.c_str(); }
};
