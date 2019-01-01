#include <scripts/angelscript_new/base.h>
#include <angelscript.h>

namespace editor::script
{

// For now the 'void' type is used as an 'invalid' type, as it does not store any data with it.
const typeid_t invalid_type_id = { asTYPEID_VOID };

// Check for equality.
bool editor::script::operator==(typeid_t lhs, typeid_t rhs) noexcept
{
    return lhs.value == rhs.value;
}

// Check for in-equality.
bool editor::script::operator!=(typeid_t lhs, typeid_t rhs) noexcept
{
    return !(lhs == rhs);
}

} // namespace editor::script
