#pragma once

namespace editor::script
{

//! Defines a strong type for AngelScripts type ids.
using typeid_t = struct { int value; };

//! Defines an invalid type id value.
extern const typeid_t invalid_type_id;

//! Defines comparison methods for the typeid value.
bool operator==(typeid_t lhs, typeid_t rhs) noexcept;
bool operator!=(typeid_t lhs, typeid_t rhs) noexcept;

//! Constructor data type declaration
//! \note The template allows later specialization for the specific types.
template<typename>
struct ConstructorData;

}
