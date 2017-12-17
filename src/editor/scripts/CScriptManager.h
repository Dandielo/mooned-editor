#pragma once
#include <type_traits>
#include "angelscript/as_interpreter.h"

#include "CScriptDatabase.h"
#include <cassert>

namespace Scripts
{
    using AngelScript::AsInterpreter;

	struct SMetadata
	{
		static constexpr int ClassType	= 0x0001;
		static constexpr int MethodType = 0x0002;

		// The metadata type
		SMetadata(int type) : type(type) { }
		const int type;
	};

	struct SClassMetadata : SMetadata
	{
		SClassMetadata() : SMetadata(ClassType) { }
		std::unordered_map<std::string, std::string> Data;
	};

	class CScriptManager final
	{
	public:
		using ptr = std::shared_ptr<CScriptManager>;
		CScriptManager();
        ~CScriptManager() = default;

        void Initialize(std::string init_file);

        AsInterpreter::ptr interpreter() const { return _interpreter; }

        asITypeInfo* GetTypeByName(const std::string& name) const;
        std::string GetTypeAttr(asITypeInfo* type, std::string attribute) const;
        std::string GetTypeAttr(asITypeInfo* type, std::string attribute, std::string default) const;
        std::string GetPropertyAttr(asITypeInfo* type, int index, std::string attribute) const;
        std::string GetPropertyAttr(asITypeInfo* type, int index, std::string attribute, std::string default) const;

		std::vector<asITypeInfo*> QueryTypes(std::string query) const;

        template<class T, int FlagValue = 0>
        T* CreateObject(const std::string& name, bool check = true) const
        {
            asIScriptEngine* engine = *_interpreter;
            auto typeinfo = _interpreter->GetDefaultModule()->GetTypeInfoByName(name.c_str());

            if (nullptr == typeinfo)
            {
                assert(nullptr == typeinfo && !check);
                return nullptr;
            }

            auto cstr = typeinfo->GetFactoryByIndex(0);
            T* native_obj = nullptr;

            if (cstr)
            {
                auto ctx = _interpreter->GetContext();
                int flag = FlagValue;
                ctx->SetUserData(&flag, 0x01);
                ctx->Prepare(cstr);
                ctx->Execute();
                ctx->SetUserData(nullptr, 0x01);

                auto obj = reinterpret_cast<asIScriptObject*>(ctx->GetReturnAddress());
                if (obj) native_obj = *reinterpret_cast<T**>(obj->GetAddressOfProperty(0));
            }

            return native_obj;
        }

	protected:
		void ProcessMetadata(std::string metadata, std::unordered_map<std::string, std::string>& data) const;

    private:
        CScriptDatabase _database;
		AsInterpreter::ptr _interpreter;
	};
}

