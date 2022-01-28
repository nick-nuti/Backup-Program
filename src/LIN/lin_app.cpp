#include "lin_app.hpp"

bool checkadmin()
{
    return (getuid()) ? false : true;
}