#pragma once
#include <memory>
#include <stack>
#include <angelscript.h>
#include <scriptbuilder.h>

namespace Scripts
{
	namespace AngelScript
	{
		class AsScriptModule
		{
		public:
			using ptr = std::shared_ptr<AsScriptModule>;

			AsScriptModule(asIScriptEngine* engine, std::string name);
			~AsScriptModule() = default;

			asIScriptFunction* GetFunctionByName(const std::string& name) const;
			asITypeInfo* GetTypeInfoByName(const std::string& name) const;

			// Object types
			asITypeInfo* GetObjectTypeByIndex(int index) const;
			asITypeInfo* GetObjectTypeByIndex(int index, std::string& metadata) const;
			int GetObjectTypeCount() const;

            // Metadata values
            std::string GetMetadataForType(asITypeInfo* typeinfo) const;
            std::string GetMetadataForMethod(asITypeInfo* typeinfo, asIScriptFunction* function) const;
            std::string GetMetadataForProperty(asITypeInfo* typeinfo, int index) const;

			// Script handling
			void AddSectionFromFile(const std::string& file);
			void Build();

        public:
            std::string current_include_directory() const;
            void push_include_directory(std::string);
            void pop_include_directory();

        private:
            std::stack<std::string> m_IncludeStack;

		private:
			std::unique_ptr<CScriptBuilder> m_Builder;
			asIScriptModule* m_Module;
		};
	}
}

