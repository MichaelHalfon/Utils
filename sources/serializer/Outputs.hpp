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
            template<typename Derivate, typename Member>
            static void    serializeMember(std::ostream &os, const Derivate &obj, const Member &memberInfo)
            {
                auto memberPtr = std::get<1>(memberInfo);
                os << obj.*memberPtr << std::endl;
            }
        };

        struct DebugOutput
        {
            template<typename Derivate, typename Member>
            static void    serializeMember(std::ostream &os, const Derivate &obj, const Member &memberInfo)
            {
                std::string memberName = std::get<0>(memberInfo);
                auto memberPtr = std::get<1>(memberInfo);

                std::cout << "Pushing " << memberName << ": " << obj.*memberPtr << " of size: " << sizeof(obj.*memberPtr) << std::endl;
                os << obj.*memberPtr << std::endl;
            }
        };
    }
}

#endif //MUTILS_OUTPUTS_HPP
