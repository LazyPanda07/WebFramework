#pragma once

#include "Databases/Interfaces/ISQLValue.h"

#include "Framework/WebFrameworkPlatform.h"

#include "SQLValue.h"

namespace framework
{
	class WEB_FRAMEWORK_API SQLValueImplementation : interfaces::ISQLValue
	{
	private:
		database::SQLValue value;

	public:
		SQLValueImplementation() = default;

		SQLValueImplementation(database::SQLValue&& value);

		void setInt(int64_t value) override;

		void setDouble(double value) override;

		void setString(const char* value) override;

		void setBool(bool value) override;

		void setNull() override;

		void setBlob(const uint8_t* value, size_t size) override;

		int64_t getInt() const override;

		double getDouble() const override;

		const char* getString() const override;

		bool getBool() const override;

		const uint8_t* getBlob(size_t* size) const override;

		SQLValueType getType() const override;

		~SQLValueImplementation() = default;
	};
}
