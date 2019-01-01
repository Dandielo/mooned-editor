#pragma once
#include <scripts/angelscript_new/base.h>

#include <angelscript.h>
#include <functional>
#include <vector>
#include <memory>

namespace editor::script
{

//! The Method class declaration.
class Method;

//! The Object class declaration.
class Object;

//! The Object class declaration.
class ScriptObject;

//! The Method class declaration.
class Function;

//! A single script context object.
class ScriptContext
{
public:
    ScriptContext(ConstructorData<ScriptContext>) noexcept;
    ~ScriptContext() noexcept;

    //! Sets the given method to be called.
    void prepare(const Function& func) noexcept;

    //! Sets the given function to be called.
    void prepare(const Method& method, const ScriptObject& object) noexcept;

    //! Sets a userdata value for the whole live time of this object.
    void set_userdata(const void* data, uintptr_t type = 0) noexcept;

    //! \returns The userdata associated with the given type value.
    auto userdata(uintptr_t type = 0) const noexcept -> const void*;

    //! Calls the prepared function with the given arguments.
    template<typename... Args>
    void call(Args&&... args) noexcept;

    //! \returns The native script object.
    auto native() noexcept -> asIScriptContext* { return _script_context.get(); }

    //! \returns The native script object.
    auto native() const noexcept -> const asIScriptContext* { return _script_context.get(); }

    //! \returns A script context to be used for a function call.
    template<class T>
    static auto from_any(T& object) noexcept -> ScriptContext
    {
        return from_engine(object.native()->GetEngine());
    }

protected:
    static auto from_engine(asIScriptEngine* engine) noexcept -> ScriptContext;

protected:
    //! Sets a BYTE argument at the given index.
    void set_argument(asUINT index, asBYTE arg) noexcept;

    //! Sets a WORD argument at the given index.
    void set_argument(asUINT index, asWORD arg) noexcept;

    //! Sets a DWORD argument at the given index.
    void set_argument(asUINT index, asDWORD arg) noexcept;

    //! Sets a QWORD argument at the given index.
    void set_argument(asUINT index, asQWORD arg) noexcept;

    //! Sets a float argument at the given index.
    void set_argument(asUINT index, float arg) noexcept;

    //! Sets a double argument at the given index.
    void set_argument(asUINT index, double arg) noexcept;

    //! Sets a object argument at the given index.
    void set_argument(asUINT index, Object& object) noexcept;

    //! Sets a object argument at the given index.
    void set_argument(asUINT index, ScriptObject& object) noexcept;

    //! Sets a address argument at the given index.
    void set_argument(asUINT index, void* address) noexcept;

    //! Sets a address argument at the given index.
    void set_argument(asUINT index, const char* string) noexcept;

    //! Sets a address argument at the given index.
    template<typename T>
    void set_argument(asUINT index, T* address) noexcept;

    //! Sets a address argument at the given index.
    template<typename T>
    void set_argument(asUINT index, T& address) noexcept;

    //! Sets a address argument at the given index.
    template<typename T>
    void set_argument(asUINT index, const T& address) noexcept;

    //! Executes the prepare context.
    void execute() noexcept;

private:
    std::unique_ptr<asIScriptContext, void(*)(asIScriptContext*) noexcept> _script_context;

    std::function<void()> _cleanup;
};

template<typename T>
void editor::script::ScriptContext::set_argument(asUINT index, T* address) noexcept
{
    set_argument(index, reinterpret_cast<void*>(address));
}

template<typename T>
void editor::script::ScriptContext::set_argument(asUINT index, T& address) noexcept
{
    set_argument(index, reinterpret_cast<void*>(&address));
}

template<typename T>
void editor::script::ScriptContext::set_argument(asUINT index, const T& address) noexcept
{
    set_argument(index, const_cast<T*>(&address));
}

namespace detail
{

// Helper type
struct SetArgsHelper { template<class... Args> SetArgsHelper(Args&&...) { } };

} // namespace detail

template<typename... Args>
void editor::script::ScriptContext::call(Args&&... args) noexcept
{
    asUINT index = 0;
    detail::SetArgsHelper{ (set_argument(index++, args), 0)... };

    execute();
}

} // namespace editor::script

namespace Scripts
{
    namespace AngelScript
    {
		class AsContextPool;
		class CContextDeleter;

		using script_context_ptr = std::unique_ptr<asIScriptContext, CContextDeleter>;

		class CContextDeleter
		{
		public:
			CContextDeleter(AsContextPool* pool);
			CContextDeleter(const CContextDeleter& other);
			CContextDeleter(CContextDeleter&& other);
			~CContextDeleter() = default;

			void operator()(asIScriptContext* context) const;

		private:
			AsContextPool* m_Pool;
		};

		// The context pool
		class AsContextPool
		{
		public:
			using ptr = std::unique_ptr<AsContextPool>;
			using context_vector = std::vector<asIScriptContext*>;
			using return_func_t = void(*)(asIScriptEngine*, asIScriptContext*, AsContextPool*);
			using request_func_t = asIScriptContext*(*)(asIScriptEngine*, AsContextPool*);

			AsContextPool(asIScriptEngine* engine);
			~AsContextPool();

			script_context_ptr LocalContext();
			asIScriptContext* RequestContext();
			void ReturnContext(asIScriptContext* context);

		private:
			asIScriptEngine* m_ScriptEngine;
			context_vector m_ContextList;

			request_func_t m_RequestFunc;
			return_func_t m_ReturnFunc;
		};
	}
}
