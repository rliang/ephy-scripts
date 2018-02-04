#include <webkit2/webkit-web-extension.h>
#include <JavaScriptCore/JavaScript.h>

struct script_info {
	JSStringRef source;
	JSStringRef script;
};

static GArray *script_infos;
static WebKitScriptWorld *script_world;

static void add_file(const gchar *dirname, const gchar *filename)
{
	gchar *path = g_build_filename(dirname, filename, NULL);
	gchar *uri = g_filename_to_uri(path, NULL, NULL);
	GMappedFile *file = g_mapped_file_new(path, FALSE, NULL);
	if (file != NULL) {
		struct script_info s = {
			JSStringCreateWithUTF8CString(uri),
			JSStringCreateWithUTF8CString(g_mapped_file_get_contents(file)),
		};
		g_array_append_val(script_infos, s);
		g_mapped_file_unref(file);
	}
	g_free(uri);
	g_free(path);
}

static void add_path(const gchar *path)
{
	gchar *dirname = g_build_filename(path, "epiphany", "userscripts", NULL);
	GDir *dir = g_dir_open(dirname, 0, NULL);
	if (dir != NULL) {
		const gchar *filename;
		while ((filename = g_dir_read_name(dir)) != NULL)
			add_file(dirname, filename);
		g_dir_close(dir);
	}
	g_free(dirname);
}

static void on_document_loaded(WebKitWebPage *page, gpointer user_data)
{
	(void) user_data;
	WebKitFrame *frame = webkit_web_page_get_main_frame(page);
	JSGlobalContextRef ctx = webkit_frame_get_javascript_context_for_script_world(frame, script_world);
	for (guint i = 0; i < script_infos->len; i++) {
		struct script_info *s = &g_array_index(script_infos, struct script_info, i);
		JSEvaluateScript(ctx, s->script, NULL, s->source, 1, NULL);
	}
}

static void on_page_created(WebKitWebExtension *extension, WebKitWebPage *page, gpointer user_data)
{
	(void) extension;
	(void) user_data;
	g_signal_connect(page, "document-loaded", G_CALLBACK(on_document_loaded), NULL);
}

G_MODULE_EXPORT void webkit_web_extension_initialize(WebKitWebExtension *extension)
{
	script_infos = g_array_new(FALSE, FALSE, sizeof(struct script_info));
	script_world = webkit_script_world_new();
	const gchar * const *paths = g_get_system_data_dirs();
	while (*paths != NULL)
		add_path(*(paths++));
	add_path(g_get_user_data_dir());
	g_signal_connect(extension, "page-created", G_CALLBACK(on_page_created), NULL);
}
