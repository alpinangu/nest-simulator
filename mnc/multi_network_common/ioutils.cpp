#include <cstdio>

#include <multi_network_common/ioutils.h>

namespace nest 
{

namespace IOUtils 
{

void
write (std::ostringstream& out, std::string s)
{
    std::istringstream value (s);
    while (true)
    {
        int c;
        switch (c = value.get ())
        {
            case '\\':
            case ':':
                out << '\\';
            default:
                out << (char) c;
                continue;
            case EOF:
                break;
        }
        break;
    }
}

std::string
read (std::istringstream& in, int delim)
{
    std::ostringstream value;
    while (true)
    {
        int c = in.peek ();
        switch (c)
        {
            case '\\':
                in.get ();
                value << (char) in.get ();
                continue;
            default:
                if (c == delim)
                break;
                value << (char) in.get ();
                continue;
            case EOF:
                break;
        }
        break;
    }
    return value.str ();
}

std::string
read (std::istringstream& in)
{
    return read (in, ':');
}

}
  
}
