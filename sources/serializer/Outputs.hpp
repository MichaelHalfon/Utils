#ifndef MUTILS_OUTPUTS_HPP
#define MUTILS_OUTPUTS_HPP

namespace mutils
{
    namespace out
    {
        struct DefaultOutput
        {
            template<typename Derivate, typename Member>
            static void    serializeMember(std::ostream &os, const Derivate &obj, const Member &memberInfo, bool)
            {
                auto memberPtr = std::get<1>(memberInfo);
                os << obj.*memberPtr << std::endl;
            }
        };

        struct DebugOutput
        {
            template<typename Derivate, typename Member>
            static void    serializeMember(std::ostream &os, const Derivate &obj, const Member &memberInfo, bool lastMember)
            {
                std::string memberName = std::get<0>(memberInfo);
                auto memberPtr = std::get<1>(memberInfo);

                if (DebugOutput::firstTime)
                {
                    std::cout << obj.*memberPtr << std::endl << "{" << std::endl;
                    DebugOutput::firstTime = false;
                }
                else
                    std::cout << memberName << ": " << obj.*memberPtr << std::endl;
                if (lastMember)
                    std::cout << "}" << std::endl;
                os << obj.*memberPtr << std::endl;
            }
            static bool firstTime;
        };


        struct JSONOutput
        {
            template<typename Derivate, typename Member>
            static void    serializeMember(std::ostream &os, const Derivate &obj, const Member &memberInfo, bool lastMember)
            {
                std::string memberName = std::get<0>(memberInfo);
                auto memberPtr = std::get<1>(memberInfo);

                if (JSONOutput::firstTime)
                {
                    os << "{" << std::endl;
                    JSONOutput::firstTime = false;
                }
                else
                    os << "\"" << memberName << "\"" << ":";
                JSONOutput::memberHandler(os, obj.*memberPtr);
                if (lastMember)
                    os << "}" << std::endl;
            }

            template<typename MemberType>
            static void    memberHandler(std::ostream &os, const MemberType &member)
            {
                os << member << std::endl;
            }

            static bool firstTime;
        };

        bool DebugOutput::firstTime = true;
        bool JSONOutput::firstTime = true;

        template <>
        void    JSONOutput::memberHandler<std::string>(std::ostream &os, std::string const &member)
        {
            os << "\"" << member << "\"" << std::endl;
        }

//        template <typename Derivate>
//        inline void    JSONOutput::memberHandler<Derivate>(std::ostream &os, Derivate const &member)
//        {
//            os << "\"" << member << "\"" << std::endl;
//        }
    }

    namespace in
    {
        struct DefaultInput
        {
            template<typename Derivate, typename Member>
            static void    deserializeMember(std::istream &is, Derivate &obj, Member &memberInfo)
            {
                auto memberPtr = std::get<1>(memberInfo);

                is >> obj.*memberPtr;
            }
        };
    }
}

#endif //MUTILS_OUTPUTS_HPP
