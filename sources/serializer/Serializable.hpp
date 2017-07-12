#ifndef MUTILS_SERIALIZABLE_HPP
#define MUTILS_SERIALIZABLE_HPP

#include <iostream>
#include <memory>
#include <tuple>

namespace mutils
{
    template<typename Derivate>
    struct IBaseImpl
    {
        virtual void serialize(std::ostream &, const Derivate &) = 0;
    };

    struct DefaultOutput
    {
        template<typename Member>
        static void    serializeMember(std::ostream &os, const Member &member)
        {
            os << member << std::endl;
        }
    };

    template<typename Derivate, size_t ...I, typename ...MemberPointers>
    void    printAttrs(std::ostream &os, const Derivate &obj, std::index_sequence<>, std::tuple<MemberPointers...> &tuple)
    {}

    template<typename Derivate, size_t I, size_t ...Rest, typename ...MemberPointers>
    void    printAttrs(std::ostream &os, const Derivate &obj, std::index_sequence<I, Rest...>, std::tuple<MemberPointers...> &tuple)
    {
        auto memberPointer = std::get<I>(tuple);

        DefaultOutput::serializeMember(os, obj.*memberPointer);
        printAttrs(os, obj, std::index_sequence<Rest...>(), tuple);
    }

    template<typename Derivate>
    class Serializable
    {
        template<typename ...MemberPointers>
        struct Impl : public IBaseImpl<Derivate>
        {
            Impl(MemberPointers...attributes)
                    : _tuple(attributes...)
            {}

            void    serialize(std::ostream &os, const Derivate &obj)
            {
                printAttrs(os, obj, std::index_sequence_for<MemberPointers...>(), this->_tuple);
            }
            std::tuple<MemberPointers...>       _tuple;
        };

    public:
        template<typename ...MemberPointers>
        Serializable(MemberPointers...args)
                : _impl(new Impl<MemberPointers...>(args...))
        {}

        friend std::ostream     &operator<<(std::ostream &os, Derivate const &obj)
        {
            obj._impl->serialize(os, obj);
        }
    private:
        std::shared_ptr< IBaseImpl<Derivate> >        _impl;
    };

}

#endif //MUTILS_SERIALIZABLE_HPP
