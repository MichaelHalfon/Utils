#ifndef MUTILS_SERIALIZABLE_HPP
#define MUTILS_SERIALIZABLE_HPP

#include <iostream>
#include <sstream>
#include <memory>
#include <tuple>
#include "Outputs.hpp"

namespace mutils
{
    template<typename Derivate>
    struct IBaseImpl
    {
        virtual void serialize(std::ostream &, const Derivate &) = 0;
        virtual void deserialize(std::istream &, Derivate &) = 0;
    };

    template<typename OutputPolicy, typename Derivate, typename ...MemberPointers>
    void    printAttrs(std::ostream &, const Derivate &, std::index_sequence<>, std::tuple<MemberPointers...> &)
    {}

    template<typename OutputPolicy, typename Derivate, size_t I, typename ...MemberPointers>
    void    printAttrs(std::ostream &os, const Derivate &obj, std::index_sequence<I>, std::tuple<MemberPointers...> &tuple)
    {
        auto tupleElem = std::get<I>(tuple);

        OutputPolicy::serializeMember(os, obj, tupleElem, true);
    }

    template<typename OutputPolicy, typename Derivate, size_t I, size_t Second, size_t ...Rest, typename ...MemberPointers>
    void    printAttrs(std::ostream &os, const Derivate &obj, std::index_sequence<I, Second, Rest...>, std::tuple<MemberPointers...> &tuple)
    {
        auto tupleElem = std::get<I>(tuple);

        OutputPolicy::serializeMember(os, obj, tupleElem, false);
        printAttrs<OutputPolicy>(os, obj, std::index_sequence<Second, Rest...>(), tuple);
    }

    template<typename Derivate, typename ...MemberPointers>
    void    receiveAttrs(std::istream &, Derivate &, std::index_sequence<>, std::tuple<MemberPointers...> &)
    {}

    template<typename Derivate, size_t I, size_t ...Rest, typename ...MemberPointers>
    void    receiveAttrs(std::istream &is, Derivate &obj, std::index_sequence<I, Rest...>, std::tuple<MemberPointers...> &tuple)
    {
        auto tupleElem = std::get<I>(tuple);

        mutils::in::DefaultInput::deserializeMember(is, obj, tupleElem);
        receiveAttrs(is, obj, std::index_sequence<Rest...>(), tuple);
    }

    template<typename Derivate, typename OutputPolicy = mutils::out::DefaultOutput>
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
                printAttrs<OutputPolicy>(os, obj, std::index_sequence_for<MemberPointers...>(), this->_tuple);
            }

            void    deserialize(std::istream &is, Derivate &obj)
            {
                receiveAttrs(is, obj, std::index_sequence_for<MemberPointers...>(), this->_tuple);
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
            return (os);
        }

        friend std::istream     &operator>>(std::istream &is, Derivate &obj)
        {
            obj._impl->deserialize(is, obj);
            return (is);
        }

    private:
        std::shared_ptr< IBaseImpl<Derivate> >        _impl;
    };
}

#endif //MUTILS_SERIALIZABLE_HPP
