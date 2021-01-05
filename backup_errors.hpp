#ifndef BACKUP_ERROR_HPP
#define BACKUP_ERROR_HPP

class exception_class
{
    public:
        int error_numb;
        std::string backup_error() throw()
        {
            switch(error_numb)
            {
                case 1: return "Please enter an input file or input file path"; break;
                
                case 2: return "Input file does not exist.\r\nPlease enter an input .txt or prexisting backup program .JSON."; break;
                
                default: return "I'm lost!"; break;
            }
        }
};

#endif