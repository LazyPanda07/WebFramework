#include "CopyJSON.h"

using namespace std;

namespace framework
{
	namespace utility
	{
		WEB_FRAMEWORK_API_FUNCTION json::utility::objectSmartPointer<json::utility::jsonObject> copyJSON(const json::utility::objectSmartPointer<json::utility::jsonObject>& json)
		{
			using json::utility::objectSmartPointer;
			using json::utility::jsonObject;

			objectSmartPointer<jsonObject> result = json::utility::make_object<jsonObject>();

			for (const auto& i : json->data)
			{
				if (i.second.index() == static_cast<size_t>(json::utility::variantTypeEnum::jJSONArray))
				{
					const vector<objectSmartPointer<jsonObject>>& currentArray = get<vector<objectSmartPointer<jsonObject>>>(i.second);
					vector<objectSmartPointer<jsonObject>> data;

					for (const auto& j : currentArray)
					{
						switch (static_cast<json::utility::variantTypeEnum>(j->data.front().second.index()))
						{
						case json::utility::variantTypeEnum::jNull:
							json::utility::appendArray(get<nullptr_t>(j->data.front().second), data);

							break;

						case json::utility::variantTypeEnum::jString:
							json::utility::appendArray(get<string>(j->data.front().second), data);

							break;

						case json::utility::variantTypeEnum::jBool:
							json::utility::appendArray(get<bool>(j->data.front().second), data);

							break;

						case json::utility::variantTypeEnum::jInt64_t:
							json::utility::appendArray(get<int64_t>(j->data.front().second), data);

							break;

						case json::utility::variantTypeEnum::jUInt64_t:
							json::utility::appendArray(get<uint64_t>(j->data.front().second), data);

							break;

						case json::utility::variantTypeEnum::jDouble:
							json::utility::appendArray(get<double>(j->data.front().second), data);

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
						result->data.push_back({ i.first, get<nullptr_t>(i.second) });

						break;

					case json::utility::variantTypeEnum::jString:
						result->data.push_back({ i.first, get<string>(i.second) });

						break;

					case json::utility::variantTypeEnum::jBool:
						result->data.push_back({ i.first, get<bool>(i.second) });

						break;

					case json::utility::variantTypeEnum::jInt64_t:
						result->data.push_back({ i.first, get<int64_t>(i.second) });

						break;

					case json::utility::variantTypeEnum::jUInt64_t:
						result->data.push_back({ i.first, get<uint64_t>(i.second) });

						break;

					case json::utility::variantTypeEnum::jDouble:
						result->data.push_back({ i.first, get<double>(i.second) });

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
