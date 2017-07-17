//
// Created by Michael Boublil on 17/07/2017.
//

#ifndef MUTILS_OUTPUTS_HPP
#define MUTILS_OUTPUTS_HPP

namespace mutils
{
    namespace out
    {
        struct DefaultOutput
        {
            template<typename Member>
            static void    serializeMember(std::ostream &os, const Member &member)
            {
                os << member << std::endl;
            }
        };

        struct DebugOutput
        {
            template<typename Member>
            static void    serializeMember(std::ostream &os, const Member &member)
            {
                std::cout << "Pushing: " << member << " of size: " << sizeof(member) << std::endl;
                os << member << std::endl;
            }
        };
    }
}

#endif //MUTILS_OUTPUTS_HPP
