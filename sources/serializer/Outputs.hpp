#ifndef MUTILS_OUTPUTS_HPP
#define MUTILS_OUTPUTS_HPP

#include <regex>

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
                if (JSONOutput::firstTime == false)
                    os << "]";
                else
                    os << std::endl << std::string(indentation, '\t') << "]";
                JSONOutput::inVector = false;
                JSONOutput::firstTime = false;
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
            static bool        verifyRegexOccurrence(std::string str, std::regex reg)
            {
                auto word_begin = std::sregex_iterator(str.begin(), str.end(), reg);
                auto word_end = std::sregex_iterator();

                if (std::distance(word_begin, word_end) >= 1)
                {
                    std::cout << str << std::endl;
                    return (true);
                }
                return (false);
            }

            template<typename Derivate, typename Member>
            static void    deserializeMember(std::istream &is, Derivate &obj, Member &memberInfo)
            {
                auto        memberName = std::get<0>(memberInfo);
                auto        memberPtr = std::get<1>(memberInfo);
                std::regex  regStd("^\t{1,}\"{1}[a-zA-Z]+\"{1}:{1}.+$");
//                std::regex  regVecClass("^\t{1,}\\{+\"{1}[a-zA-Z]+\":{1}.+\\}{1},?.?$");

                (void)obj;
                for (char strLine[500]; is.getline(strLine, 500); is.eof())
                {
                    std::string     line(strLine);

                    if (verifyRegexOccurrence(line, regStd))
                    {
                        std::cout << "Filling : " << memberName << std::endl;
                        in::DefaultInput::memberHandler(is, obj.*memberPtr);
                        break ;
                    }
                }
//
//                is >> obj.*memberPtr;
            }

            template<typename MemberType>
            static void    memberHandler(std::istream &is, MemberType &member)
            {
                is >> member;
                std::cout << "Its something else !" << std::endl;
            }

            template<typename MemberType>
            static void    memberHandler(std::istream &, std::vector<MemberType> &)
            {
                std::cout << "it's a Vector !" << std::endl;
            }
        };

        template <>
        void    DefaultInput::memberHandler<std::string>(std::istream &is, std::string &member)
        {
            is >> member;
            std::cout << "it's a string !" << std::endl;
        }
    }
}

#endif //MUTILS_OUTPUTS_HPP
