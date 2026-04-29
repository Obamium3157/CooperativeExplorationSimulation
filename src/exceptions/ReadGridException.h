#ifndef COOPERATIVEEXPLORATIONSIMULATION_READGRIDEXCEPTION_H
#define COOPERATIVEEXPLORATIONSIMULATION_READGRIDEXCEPTION_H

#include <stdexcept>

class ReadGridException : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

#endif //COOPERATIVEEXPLORATIONSIMULATION_READGRIDEXCEPTION_H
