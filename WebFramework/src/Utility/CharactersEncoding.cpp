#include "StringConversion.h"

#include "ThreadPool.h"
#include "Strings.h"

using namespace std;

class PercentEncodingSymbols
{
public:
	static constexpr array<pair<char, string_view>, 21> data =
	{
		pair{ ' ', "%20" },
		pair{ '!', "%21" },
		pair{ '\"', "%22" },
		pair{ '#', "%23" },
		pair{ '$', "%24" },
		pair{ '%', "%25" },
		pair{ '&', "%26" },
		pair{ '\'', "%27" },
		pair{ '(', "%28" },
		pair{ ')', "%29" },
		pair{ '*', "%2A" },
		pair{ '+', "%2B" },
		pair{ ',', "%2C" },
		pair{ '/', "%2F" },
		pair{ ':', "%3A" },
		pair{ ';', "%3B" },
		pair{ '=', "%3D" },
		pair{ '?', "%3F" },
		pair{ '@', "%40" },
		pair{ '[', "%5B" },
		pair{ ']', "%5D" }
	};

public:
	static constexpr size_t codeSize = 3;

public:
	using iterator = array<pair<char, string_view>, 21>::const_iterator;

public:
	PercentEncodingSymbols() = default;

	iterator operator [] (char symbol) const;

	iterator operator [] (string_view code) const;

	iterator end() const;

	~PercentEncodingSymbols() = default;
};

namespace framework
{
	namespace utility
	{
		string encodeString(const string& source)
		{
			string result;
			PercentEncodingSymbols encoding;
			PercentEncodingSymbols::iterator end = encoding.end();

			result.reserve(source.size());

			for (char character : source)
			{
				if (auto it = encoding[character]; it != end)
				{
					result += it->second;
				}
				else
				{
					result += character;
				}
			}

			return result;
		}

		string decodeString(const string& source)
		{
			string result = source;
			PercentEncodingSymbols decoding;
			PercentEncodingSymbols::iterator end = decoding.end();
			array<pair<string_view, string>, PercentEncodingSymbols::data.size()> replaces{};
			auto fillReplaces = [&result, &decoding, &replaces](size_t index, string_view code)
			{
				if (result.find(code) != string::npos)
				{
					replaces[index] = pair{ code, decoding[code]->first };
				}
			};

			{
				threading::ThreadPool threadPool;

				for (size_t i = 0; i < PercentEncodingSymbols::data.size(); i++)
				{
					threadPool.addTask(bind(fillReplaces, i, PercentEncodingSymbols::data[i].second));
				}
			}

			for (const auto& [encodedSymbol, symbol] : replaces)
			{
				if (encodedSymbol.size())
				{
					::utility::strings::replaceAllByRef(result, encodedSymbol, symbol);
				}
			}
			
			return result;
		}
	}
}

PercentEncodingSymbols::iterator PercentEncodingSymbols::operator [] (char symbol) const
{
	return ranges::find_if(PercentEncodingSymbols::data, [symbol](const pair<char, string_view>& value) { return value.first == symbol; });
}

PercentEncodingSymbols::iterator PercentEncodingSymbols::operator [] (string_view code) const
{
	return ranges::find_if(PercentEncodingSymbols::data, [code](const pair<char, string_view>& value) { return value.second == code; });
}

PercentEncodingSymbols::iterator PercentEncodingSymbols::end() const
{
	return PercentEncodingSymbols::data.end();
}
