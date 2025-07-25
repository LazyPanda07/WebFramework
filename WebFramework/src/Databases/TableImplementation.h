#pragma once

#include "DatabaseInterfaces/ITable.h"

#include "Framework/WebFrameworkPlatform.h"

#include "Table.h"

namespace framework
{
	class WEB_FRAMEWORK_API TableImplementation : public interfaces::ITable
	{
	private:
		database::Table* table;

	public:
		TableImplementation(database::Table* table);

		interfaces::ISQLResult* execute(const char* query, const interfaces::ISQLValue** values, size_t size) override;

		void deleteResult(interfaces::ISQLResult* ptr) const override;

		const char* getTableName() const override;

		~TableImplementation() = default;
	};
}
