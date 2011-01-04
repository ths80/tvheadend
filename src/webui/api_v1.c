/*
 *  Tvheadend HTTP API v1
 *  Copyright (C) 2010 Andreas Öman
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
#include "channels.h"


static int
send_json(http_connection_t *hc, htsmsg_t *out)
{
  htsbuf_queue_t *hq = &hc->hc_reply;

  htsmsg_json_serialize(out, hq, 0);
  htsmsg_destroy(out);
  http_output_content(hc, "text/x-json; charset=UTF-8");
  return 0;
}


/**
 *
 */
static int
api_channels(http_connection_t *hc, const char *remain, void *opaque)
{
  htsmsg_t *reply, *list;

  switch(hc->hc_cmd) {
  case HTTP_CMD_GET:
    reply = htsmsg_create_map();
    htsmsg_add_u32(reply, "success", 1);
    htsmsg_add_str(reply, "message", "Loaded channels");
    
    list = htsmsg_create_list();

    htsmsg_add_msg(reply, "data", list);

    break;

  default:
    return HTTP_METHOD_NOT_ALLOWED;

  }

  return send_json(hc, reply);

}


/**
 * WEB user interface
 */
void
api_v1_init(void)
{
  http_path_add("/api/v1/channels", 
		NULL, api_channels, ACCESS_WEB_INTERFACE);
}
