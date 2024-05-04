#pragma once

#include <string>
#include <vector>
#include <variant>
#include <map>
#include <algorithm>
#include <iostream>

namespace broma {
	/// TODO: (Calloc) Make a pull request with None Translated to NONE for python support...
	/// @brief The platform currently being processed in a bind statement.
	enum class Platform {
		None = 0,
		Mac = 1,
		Windows = 2,
		Android = 4,
		iOS = 8,
		Android32 = 16,
		Android64 = 32,
	};

	inline Platform operator|(Platform a, Platform b) {
		return static_cast<Platform>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline Platform& operator|=(Platform& a, Platform b) {
		a = a | b;
		return a;
	}

	inline Platform operator&(Platform a, Platform b) {
		return static_cast<Platform>(static_cast<int>(a) & static_cast<int>(b));
	}

	inline Platform& operator&=(Platform& a, Platform b) {
		a = a & b;
		return a;
	}

	/// @brief Binding offsets for each platform.
	struct PlatformNumber {
		ptrdiff_t mac = -1;
		ptrdiff_t ios = -1;
		ptrdiff_t win = -1;
		ptrdiff_t android32 = -1;
		ptrdiff_t android64 = -1;
	};

	/// @brief A C++ type declaration.
	struct Type {
		bool is_struct = false;
		std::string name;

		bool operator==(Type const& t) const {
			return name == t.name;
		}
	};

	/// @brief The signature of a free function.
	struct FunctionProto {
		Type ret; ///< The return type of the function.
		std::vector<std::pair<Type, std::string>> args; ///< All arguments, represented by their type and their name.
		std::string docs; ///< Any docstring pulled from a `[[docs(...)]]` attribute.
		std::string name; ///< The function's name.

		inline bool operator==(FunctionProto const& f) const {
			if (name != f.name || args.size() != f.args.size()) {
				return false;
			}

			for (size_t i = 0; i < args.size(); ++i) {
				if (!(args[i].first == f.args[i].first)) {
					return false;
				}
			}
			
			return true;
		}
	};

	/// @brief A member function's type.
	enum class FunctionType {
		Normal,
		Ctor, ///< A constructor.
		Dtor ///< A destructor.
	};

	/// @brief The signature of a member function.
	struct MemberFunctionProto : FunctionProto {
		FunctionType type = FunctionType::Normal;
		bool is_const = false;
		bool is_virtual = false;
		bool is_callback = false; ///< Whether or not this function is a callback.
								  ///< In Geode, this determines the function's calling convention (optcall or thiscall).
		bool is_static = false;

		inline bool operator==(MemberFunctionProto const& f) const {
			if (!FunctionProto::operator==(f))
				return false;

			if (is_const != f.is_const)
				return false;

			return true;
		}
	};

	/// @brief A class's member variables.
	struct MemberField {
		std::string name; ///< The name of the field.
		Type type; ///< The type of the field.
		size_t count = 0; ///< The number of elements in the field when it's an array (pretty much unused since we use std::array).
	};

	/// @brief Any class padding.
	struct PadField {
		PlatformNumber amount; ///< The amount of padding, separated per platform.
	};

	/// @brief A function that is bound to an offset.
	struct FunctionBindField {
		MemberFunctionProto prototype;
		PlatformNumber binds; ///< The offsets, separated per platform.
	};

	/// @brief An inline function body that should go in a source file (.cpp).
	struct OutOfLineField {
		MemberFunctionProto prototype;
		std::string inner; ///< The inline body of the function as a raw string.
	};

	/// @brief A inline function body that should go in a header file (.hpp).
	struct InlineField {
		std::string inner; ///< The inline body of the function as a raw string.
	};

	/// @brief A class field.
	struct Field {
		size_t field_id; ///< The index of the field. This starts from 0 and counts up across all classes.
		std::string parent; ///< The name of the parent class.
		std::variant<InlineField, OutOfLineField, FunctionBindField, PadField, MemberField> inner;
		Platform links; ///< All the platforms that link the field
		Platform missing; ///< All the platforms that are missing the field

		/// @brief Cast the field into a variant type. This is useful to extract data from the field.
		template <typename T>
		T* get_as() {
			return std::get_if<T>(&inner);
		}

		/// @brief Cast the field into a variant type. This is useful to extract data from the field.
		template <typename T>
		T const* get_as() const {
			return std::get_if<T>(&inner);
		}

		/// @brief Convenience function to get the function prototype of the field, if the field is a function of some sort.
		inline MemberFunctionProto* get_fn() {
			if (auto fn = get_as<OutOfLineField>()) {
				return &fn->prototype;
			} else if (auto fn = get_as<FunctionBindField>()) {
				return &fn->prototype;
			} else return nullptr;
		}
	};

	/// @brief A top-level class declaration.
	struct Class {
		std::string name; ///< The name of the class.
		std::vector<std::string> superclasses; ///< Parent classes that the current class inherits.
		std::vector<std::string> depends; ///< Classes the current class depends on.
										  ///< This includes parent classes, and any classes declared in a `[[depends(...)]]` attribute.
		std::vector<Field> fields; ///< All the fields parsed in the class.

		Platform links; ///< All the platforms that link the class
		Platform missing; ///< All the platforms that are missing the class

		inline bool operator==(Class const& c) const {
			return name == c.name;
		}
		inline bool operator==(std::string const& n) const {
			return name == n;
		}
	};

	/// @brief A top-level free function binding.
	struct Function {
		FunctionProto prototype; ///< The free function's signature.
		PlatformNumber binds; ///< The offsets of free function, separated per platform.
		Platform links; ///< All the platforms that link the function
		Platform missing; ///< All the platforms that are missing the function
	};

	/// @brief Broma's root grammar (the root AST).
	///
	/// See the user's guide for an example on how to traverse this AST.
	struct Root {
		std::vector<Class> classes;
		std::vector<Function> functions;

		inline Class* operator[](std::string const& name) {
			auto it = std::find_if(classes.begin(), classes.end(), [name](Class& cls) {
					return cls.name == name;
			});

			if (it == classes.end())
				return nullptr;

			return &*it;
		}
	};
} // namespace broma
