#pragma once

#include "WebFrameworkCoreConstants.h"

namespace framework::interfaces
{
	class WEB_FRAMEWORK_CORE_API ISQLValue
	{
	public:
		enum class SQLValueType : uint8_t
		{
			intType,
			doubleType,
			stringType,
			boolType,
			nullptrType,
			blobType
		};

	public:
		virtual void setInt(int64_t value) const = 0;

		virtual void setDouble(double value) const = 0;

		virtual void setString(const char* value) const = 0;

		virtual void setBool(bool value) const = 0;

		virtual void setNull() const = 0;

		virtual void setBlob(const uint8_t* value, size_t size) const = 0;

		virtual int64_t getInt() const = 0;

		virtual double getDouble() const = 0;

		virtual const char* getString() const = 0;

		virtual bool getBool() const = 0;

		virtual void* getNull() const = 0;

		virtual const uint8_t* getBlob(size_t* size) const = 0;

		virtual SQLValueType getType() const = 0;

		virtual ~ISQLValue() = default;
	};
}
