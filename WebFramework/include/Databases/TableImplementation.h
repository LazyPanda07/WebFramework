#pragma once

#include "DatabaseInterfaces/ITable.h"

#include "Framework/WebFrameworkPlatform.h"

#include "Table.h"

namespace framework
{
	class TableImplementation : public interfaces::ITable
	{
	private:
		enum class PreferredPlaceholders : uint8_t
		{
			none,
			sqlite,
			postgres
		};

	private:
		database::Table* table;
		bool adaptSqlPlaceholders;
		PreferredPlaceholders type;

	public:
		TableImplementation(database::Table* table, bool adaptSqlPlaceholders);

		interfaces::ISQLResult* execute(const char* query, const interfaces::ISQLValue** values, size_t size) override;

		void deleteResult(interfaces::ISQLResult* ptr) const override;

		const char* getTableName() const override;

		~TableImplementation() = default;
	};
}
