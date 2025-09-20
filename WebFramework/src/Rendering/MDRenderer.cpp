#include "MDRenderer.h"

#include <cmark-gfm.h>

#include "WebFrameworkCoreConstants.h"

namespace framework
{
	std::string MDRenderer::render(std::string_view source) const
	{
		std::string result;
		
		if (char* temp = cmark_markdown_to_html(source.data(), source.size(), CMARK_OPT_UNSAFE))
		{
			result = temp;

			free(temp);
		}

		return result;
	}

	std::string_view MDRenderer::getExtension() const
	{
		return markdownExtension;
	}
}
