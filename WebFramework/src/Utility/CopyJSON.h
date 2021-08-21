#pragma once

#include "headers.h"

namespace framework
{
	namespace utility
	{
		/// @brief Copy top level JSON object. Supported simple array(null, int, string, double)
		/// @param json JSON to copy
		/// @return Copied JSON
		inline json::utility::objectSmartPointer<json::utility::jsonObject> copyJSON(const json::utility::objectSmartPointer<json::utility::jsonObject>& json)
		{
			using json::utility::objectSmartPointer;
			using json::utility::jsonObject;

			objectSmartPointer<jsonObject> result = json::utility::make_object<jsonObject>();

			for (const auto& i : json->data)
			{
				if (i.second.index() == static_cast<size_t>(json::utility::variantTypeEnum::jJSONArray))
				{
					const std::vector<objectSmartPointer<jsonObject>>& currentArray = std::get<std::vector<objectSmartPointer<jsonObject>>>(i.second);
					std::vector<objectSmartPointer<jsonObject>> data;

					for (const auto& j : currentArray)
					{
						switch (static_cast<json::utility::variantTypeEnum>(j->data.front().second.index()))
						{
						case json::utility::variantTypeEnum::jNull:
							json::utility::appendArray(std::get<nullptr_t>(j->data.front().second), data);

							break;

						case json::utility::variantTypeEnum::jString:
							json::utility::appendArray(std::get<std::string>(j->data.front().second), data);

							break;

						case json::utility::variantTypeEnum::jBool:
							json::utility::appendArray(std::get<bool>(j->data.front().second), data);

							break;

						case json::utility::variantTypeEnum::jInt64_t:
							json::utility::appendArray(std::get<int64_t>(j->data.front().second), data);

							break;

						case json::utility::variantTypeEnum::jUInt64_t:
							json::utility::appendArray(std::get<uint64_t>(j->data.front().second), data);

							break;

						case json::utility::variantTypeEnum::jDouble:
							json::utility::appendArray(std::get<double>(j->data.front().second), data);

							break;

						default:
							break;
						}
					}

					result->data.push_back({ i.first, move(data) });
				}
				else
				{
					switch (static_cast<json::utility::variantTypeEnum>(i.second.index()))
					{
					case json::utility::variantTypeEnum::jNull:
						result->data.push_back({ i.first, std::get<nullptr_t>(i.second) });

						break;

					case json::utility::variantTypeEnum::jString:
						result->data.push_back({ i.first, std::get<std::string>(i.second) });

						break;

					case json::utility::variantTypeEnum::jBool:
						result->data.push_back({ i.first, std::get<bool>(i.second) });

						break;

					case json::utility::variantTypeEnum::jInt64_t:
						result->data.push_back({ i.first, std::get<int64_t>(i.second) });

						break;

					case json::utility::variantTypeEnum::jUInt64_t:
						result->data.push_back({ i.first, std::get<uint64_t>(i.second) });

						break;

					case json::utility::variantTypeEnum::jDouble:
						result->data.push_back({ i.first, std::get<double>(i.second) });

						break;

					default:
						break;
					}
				}
			}

			return result;
		}
	}
}
