#include "el_link.h"
#include "document.h"
#include "document_container.h"
#include <cstring>

litehtml::el_link::el_link(const std::shared_ptr<document>& doc) : litehtml::html_tag(doc) {}

void litehtml::el_link::parse_attributes()
{
	document::ptr doc = get_document();

	const char* rel = get_attr("rel");
	if (rel && !strcmp(rel, "stylesheet"))
	{
		const char* media = get_attr("media");
		const char* href = get_attr("href");
		if (href && href[0])
		{
			string media_copy;
			if (media && media[0])
			{
				media_copy = media;
			}
			auto wait_for_import_completion = doc->container()->import_css(
				href, "", [&, media_copy, doc](const string& css_text, const string& new_baseurl) {
					printf("Adding stylesheet, media: %s\n", media_copy.c_str());
					if (!css_text.empty())
					{
						doc->add_stylesheet(css_text.c_str(), new_baseurl.c_str(), media_copy.c_str());
					} else
					{
						doc->container()->link(doc, shared_from_this());
					}
				});
			doc->add_pending_stylesheet(wait_for_import_completion);
		}
	} else
	{
		doc->container()->link(doc, shared_from_this());
	}
}
