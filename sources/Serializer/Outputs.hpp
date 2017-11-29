#ifndef MUTILS_OUTPUTS_HPP
#define MUTILS_OUTPUTS_HPP

# include <regex>
# include <iostream>
# include <vector>
# include <list>
# include <unordered_map>
# include <map>
# include <array>
# include <chrono>
# include <thread>


namespace mutils {

    namespace out {
        struct TextOutput {
            template<typename Derivate, typename Member>
            static void serializeMember(std::ostream &os, const Derivate &obj, const Member &memberPtr) {
                os << obj.*memberPtr << std::endl;
            }
        };

        struct BinaryOutput {

            template<typename Derivate, typename Member>
            static void serializeMember(std::ostream &os, const Derivate &obj, const Member &memberPtr) {
                writeMember(os, obj.*memberPtr);
            }


            template<typename T>
            static void writeMember(std::ostream &os, const T &member) {
                if constexpr (std::is_pointer<T>::value) {
                        writeMember(os, *member);
                }
                else if (std::is_scalar<T>::value) {
                    os.write(reinterpret_cast<const char *>(&member), sizeof(member));
                }
                else
                    os << member;
            }

            template<typename T>
            static void writeMember(std::ostream &os, const std::shared_ptr<T> &member) {
                writeMember(os, *member);
            }

            template<typename T>
            static void writeMember(std::ostream &os, const std::unique_ptr<T> &member) {
                writeMember(os, *member);
            }

            template<typename T>
            static void writeMember(std::ostream &os, const std::vector<T> &member) {

                writeMember(os, member.size());

                for (auto &elem : member) {
                    writeMember(os, elem);
                }
            }

            template<typename T>
            static void writeMember(std::ostream &os, const std::list<T> &member) {

                writeMember(os, member.size());
                for (auto &elem : member) {
                    writeMember(os, elem);
                }
            }
            template<typename T, typename U>
            static void writeMember(std::ostream &os, const std::unordered_map<T, U> &member) {

                writeMember(os, member.size());

                for (auto &elem : member) {
                    writeMember(os, elem.first);
                    writeMember(os, elem.second);
                }
            }

            template<typename T, typename U>
            static void writeMember(std::ostream &os, const std::map<T, U> &member) {

                writeMember(os, member.size());

                for (auto &elem : member) {
                    writeMember(os, elem.first);
                    writeMember(os, elem.second);
                }
            }

            template<typename T, std::size_t size>
            static void writeMember(std::ostream &os, const std::array<T, size> &member) {

                for (auto &elem : member) {
                    writeMember(os, elem);
                }
            }

        };
        template<>
        void BinaryOutput::writeMember<std::string>(std::ostream &os, const std::string &member) {
            os << member << std::endl;
        }
    }

    namespace in {
        struct TextInput {
            template<typename Derivate, typename Member>
            static void deserializeMember(std::istream &is, const Derivate &obj, const Member &memberPtr) {
                is >> obj.*memberPtr;
            }
        };

        struct BinaryInput {
            template<typename Derivate, typename Member>
            static void deserializeMember(std::istream &is, Derivate &obj, Member &memberPtr) {
                BinaryInput::readMember(is, obj.*memberPtr);
            }

            template<typename T>
            static void readMember(std::istream &is, T &member) {
                if constexpr (std::is_pointer<T>::value) {
                        member = new typename std::remove_pointer<T>::type;
                        readMember(is, *member);
                }
                else if (std::is_scalar<T>::value) {
                    is.read(reinterpret_cast<char *>(&member), sizeof(member));
                }
                else
                    is >> member;
            }

            template<typename T>
            static void readMember(std::istream &is, std::shared_ptr<T> &member) {
                T val;

                readMember(is, val);
                member = std::make_shared<T>(val);
            }

            template<typename T>
            static void readMember(std::istream &is, std::unique_ptr<T> &member) {
                T val;

                readMember(is, val);
                member = std::make_unique<T>(val);
            }

            template<typename T>
            static void readMember(std::istream &is, std::vector<T> &member) {
                unsigned long size = 0;

                readMember(is, size);
                for (unsigned int i = 0; i < size; i++) {
                    T elem;

                    readMember(is, elem);
                    member.emplace_back(elem);
                }
            }

            template<typename T>
            static void readMember(std::istream &is, std::list<T> &member) {
                unsigned long size = 0;

                readMember(is, size);
                for (unsigned int i = 0; i < size; i++) {
                    T elem;

                    readMember(is, elem);
                    member.emplace_back(elem);
                }
            }

            template<typename T, typename U>
            static void readMember(std::istream &is, std::unordered_map<T, U> &member) {
                unsigned long size;

                readMember(is, size);
                for (unsigned int i = 0; i < size; i++) {
                    T key;
                    U val;

                    readMember(is, key);
                    readMember(is, val);
                    member[key] = val;
                }
            }

            template<typename T, typename U>
            static void readMember(std::istream &is, std::map<T, U> &member) {
                unsigned long size;

                readMember(is, size);
                for (unsigned int i = 0; i < size; i++) {
                    T key;
                    U val;

                    readMember(is, key);
                    readMember(is, val);
                    member[key] = val;
                }
            }
            template<typename T, std::size_t size>
            static void readMember(std::istream &is, std::array<T, size> &member) {

                for (unsigned int i = 0; i < size; i++) {
                   T elem;

                    readMember(is, elem);
                    member[i] = elem;
                }
            }
        };
        template<>
        void BinaryInput::readMember<std::string>(std::istream &is, std::string &member) {
            std::getline(is, member);
        }
    }
}


#endif //MUTILS_OUTPUTS_HPP
