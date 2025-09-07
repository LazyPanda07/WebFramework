#include "MDRenderer.h"

#include <cmark-gfm.h>

#include "WebFrameworkCoreConstants.h"

using namespace std;

namespace framework
{
	string MDRenderer::render(string_view source) const
	{
		string result;
		
		if (char* temp = cmark_markdown_to_html(source.data(), source.size(), CMARK_OPT_UNSAFE))
		{
			result = temp;

			free(temp);
		}

		return result;
	}

	string_view MDRenderer::getExtension() const
	{
		return markdownExtension;
	}
}
