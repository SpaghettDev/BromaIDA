#ifndef __HELPER_HPP__
#define __HELPER_HPP__

#include "broma.hpp"
#include "ast.hpp"

/* Used as a primary helpers around Field's variant and other Types since variants
are not supported by cython yet as far as I am aware... */

/* These help to provide compatability to python */

inline broma::InlineField *Field_GetAs_InlineField(broma::Field *f)
{
    broma::InlineField *x;
    if (x = std::get_if<broma::InlineField>(&f->inner))
    {
        return x;
    };
    return nullptr;
}

inline broma::OutOfLineField *Field_GetAs_OutOfLineField(broma::Field *f)
{
    broma::OutOfLineField *x;
    if (x = std::get_if<broma::OutOfLineField>(&f->inner))
    {
        return x;
    };
    return nullptr;
}

inline broma::FunctionBindField *Field_GetAs_FunctionBindField(broma::Field *f)
{
    broma::FunctionBindField *x;
    if (x = std::get_if<broma::FunctionBindField>(&f->inner))
    {
        return x;
    };
    return nullptr;
}

inline broma::PadField *Field_GetAs_PadField(broma::Field *f)
{
    broma::PadField *x;
    if (x = std::get_if<broma::PadField>(&f->inner))
    {
        return x;
    };
    return nullptr;
}

inline broma::MemberField *Field_GetAs_MemberField(broma::Field *f)
{
    broma::MemberField *x;
    if (x = std::get_if<broma::MemberField>(&f->inner))
    {
        return x;
    };
    return nullptr;
}

inline bool MemberFunctionProtoEquals(broma::MemberFunctionProto const &a, broma::MemberFunctionProto const &b)
{
    return a == b;
};

inline bool FunctionProtoEquals(broma::FunctionProto const &a, broma::FunctionProto const &b)
{
    return a == b;
};

inline bool TypeEquals(broma::Type const &a, broma::Type const &b)
{
    return a == b;
}

inline bool ClassEqualsTo(broma::Class const &a, broma::Class const &b)
{
    return a.name == b.name;
}
inline bool ClassEqualsToName(broma::Class const &a, std::string const &b)
{
    return a.name == b;
}

inline broma::MemberFunctionProto *FieldGetFn(broma::Field *field)
{
    return field->get_fn();
}

broma::Class *RootgetClassFromStr(broma::Root *root, std::string const &name)
{
    return root->operator[](name);
}

#endif // __HELPER_HPP__