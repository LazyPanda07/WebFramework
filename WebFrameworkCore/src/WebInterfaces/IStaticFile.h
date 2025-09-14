#pragma once

#include "IFile.h"

#include <algorithm>

#include "IHTTPResponse.h"
#include "Rendering/IStaticFileRenderer.h"

namespace framework::interfaces
{
	struct InsensitiveStringViewHash
	{
		inline size_t operator () (const std::string_view& value) const noexcept
		{
			std::string tem;

			tem.reserve(value.size());

			std::for_each(value.begin(), value.end(), [&tem](char c) { tem += tolower(c); });

			return std::hash<std::string>()(tem);
		}
	};

	struct InsensitiveStringViewEqual
	{
		inline bool operator () (const std::string_view& left, const std::string_view& right) const noexcept
		{
			return std::equal
			(
				left.begin(), left.end(),
				right.begin(), right.end(),
				[](char first, char second) { return tolower(first) == tolower(second); }
			);
		}
	};

	class IStaticFile : virtual public IFile
	{
	public:
		/**
		* @param fileName Optional parameter for specifying name of file in Content-Disposition HTTP header
		*/
		virtual void sendStaticFile(std::string_view filePath, IHTTPResponse& response, bool isBinary, std::string_view fileName) = 0;

		virtual const std::unordered_map<std::string_view, std::unique_ptr<IStaticFileRenderer>, InsensitiveStringViewHash, InsensitiveStringViewEqual>& getStaticRenderers() const = 0;

		virtual ~IStaticFile() = default;
	};
}
