#include "win_app.hpp"

bool checkadmin()
{
    return (IsUserAnAdmin() == NULL) ? false : true;
}