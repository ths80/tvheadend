/*
 *  tvheadend, ExtJS based interface
 *  Copyright (C) 2008, 2010 Andreas Öman
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <arpa/inet.h>

#include "htsmsg.h"
#include "htsmsg_json.h"

#include "tvheadend.h"
#include "http.h"
#include "webui.h"
#include "access.h"
#include "dtable.h"
#include "channels.h"
#include "psi.h"

#include "dvr/dvr.h"
#include "serviceprobe.h"
#include "xmltv.h"
#include "epg.h"
#include "iptv_input.h"

extern const char *htsversion;
extern const char *htsversion_full;

static void
extjs_load(htsbuf_queue_t *hq, const char *script)
{
  htsbuf_qprintf(hq,
		 "<script type=\"text/javascript\" "
		 "src=\"%s\">"
		 "</script>\n", script);
		 
}

/**
 *
 */
static void
extjs_exec(htsbuf_queue_t *hq, const char *fmt, ...)
{
  va_list ap;

  htsbuf_qprintf(hq, "<script type=\"text/javascript\">\r\n");

  va_start(ap, fmt);
  htsbuf_vqprintf(hq, fmt, ap);
  va_end(ap);

  htsbuf_qprintf(hq, "\r\n</script>\r\n");
}


/**
 *
 */
static int
webuiloader(http_connection_t *hc, const char *remain, void *opaque)
{
  htsbuf_queue_t *hq = &hc->hc_reply;

#define EXTJSPATH "static/extjs-3.3.1"

  htsbuf_qprintf(hq, "<html>\n"
		 "<script type=\"text/javascript\" src=\""EXTJSPATH"/adapter/ext/ext-base-debug.js\"></script>\n"
		 "<script type=\"text/javascript\" src=\""EXTJSPATH"/ext-all-debug.js\"></script>\n"
		 "<link rel=\"stylesheet\" type=\"text/css\" href=\""EXTJSPATH"/resources/css/ext-all.css\">\n"
		 "<link rel=\"stylesheet\" type=\"text/css\" href=\""EXTJSPATH"/ux/css/GroupTab.css\">\n"
);

  extjs_exec(hq, "Ext.BLANK_IMAGE_URL = "
	     "'"EXTJSPATH"/resources/images/default/s.gif';");

#if 0
  htsbuf_qprintf(hq, 
		 "<script type='text/javascript' "
		 "src='http://getfirebug.com/releases/lite/1.2/firebug-lite-compressed.js'></script>");
#endif

  /**
   * Create a namespace for our app
   */
  extjs_exec(hq, "Ext.namespace('tvh');");

  /**
   * UX
   */
  extjs_load(hq, EXTJSPATH"/ux/GroupTabPanel.js");
  extjs_load(hq, EXTJSPATH"/ux/GroupTab.js");

  /**
   * Finally, the app itself
   */
  extjs_load(hq, "static/tvheadend/tvheadend.js");
  



  htsbuf_qprintf(hq,
		 "<style type=\"text/css\">\n"
		 "html, body {\n"
		 "\tfont:normal 12px verdana;\n"
		 "\tmargin:0;\n"
		 "\tpadding:0;\n"
		 "\tborder:0 none;\n"
		 "\toverflow:hidden;\n"
		 "\theight:100%;\n"
		 "}\n"
		 "#systemlog {\n"
		 "\tfont:normal 12px courier; font-weight: bold;\n"
		 "}\n"
		 "p {\n"
		 "\tmargin:5px;\n"
		 "}\n"
		 "</style>\n"
		 "<title>HTS Tvheadend %s</title>\n"
		 "</head>\n"
		 "<body>\n"
		 "<div id=\"systemlog\"></div>\n"
		 "</body></html>\n",
		 htsversion);
  http_output_html(hc);
  return 0;
}



/**
 * WEB user interface
 */
void
extjs_start(void)
{
  http_path_add("/webui",       NULL, webuiloader,       ACCESS_WEB_INTERFACE);
}
