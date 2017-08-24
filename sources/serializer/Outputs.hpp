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

                if (!JSONOutput::inVector)
                    os << std::string(indentation, '\t');
                if (JSONOutput::inVector && JSONOutput::firstTime)
                {
                    os << std::endl << std::string(indentation, '\t') << "{";
                }
                else if (JSONOutput::firstTime)
                {
                    mutils::out::JSONOutput::indentation++;
                    JSONOutput::memberHandler(os, obj.*memberPtr);
                    os << ":" << " {" << std::endl;
                }
                else
                {
                    os << "\"" << memberName << "\":";
                    JSONOutput::memberHandler(os, obj.*memberPtr);
                }
                if (!lastMember && !JSONOutput::firstTime)
                {
                    if (!JSONOutput::inVector)
                        os << "," << std::endl;
                    else
                        os << ", ";
                }
                JSONOutput::firstTime = false;
            }

            template<typename MemberType>
            static void    memberHandler(std::ostream &os, const MemberType &member)
            {
                os << member;
            }

            template<typename MemberType>
            static void    memberHandler(std::ostream &os, const std::vector<MemberType> &member)
            {
                JSONOutput::inVector = true;
                os << "[";
                for (auto it = member.begin(); it != member.end(); it++)
                {
                    memberHandler(os, (*it));
                    if (it + 1 != member.end())
                        os << ", ";
                }
                JSONOutput::inVector = false;
                JSONOutput::firstTime = false;
                os << "]";
            }


            static bool         firstTime;
            static bool         inVector;
            static unsigned int indentation;
        };

        bool DebugOutput::firstTime = true;
        bool JSONOutput::firstTime = true;
        bool JSONOutput::inVector = false;
        unsigned int JSONOutput::indentation = 0;

        template <>
        void    JSONOutput::memberHandler<std::string>(std::ostream &os, std::string const &member)
        {
            os << "\"" << member << "\"";
        }

        template <>
        void    JSONOutput::memberHandler<bool>(std::ostream &os, bool const &member)
        {
            os << ((member)? "true" : "false");
        }

    }

    namespace in
    {
        struct DefaultInput
        {
            template<typename Derivate, typename Member>
            static void    deserializeMember(std::istream &, Derivate &, Member &)
            {
//                auto memberPtr = std::get<1>(memberInfo);
//
//                is >> obj.*memberPtr;
            }
        };
    }
}

#endif //MUTILS_OUTPUTS_HPP
