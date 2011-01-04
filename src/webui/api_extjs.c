/*
 *  Tvheadend HTTP EXTJS specific API
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
#include "device.h"

static void
devicetree_add_list(htsmsg_t *reply, struct device_list *list)
{
  device_t *d;
  LIST_FOREACH(d, list, d_link) {
    htsmsg_t *m = htsmsg_create_map();
    htsmsg_add_u32(m, "id", d->d_id);
    htsmsg_add_str(m, "text", d->d_displayname);
    htsmsg_add_u32(m, "leaf", 1);
    htsmsg_add_msg(reply, NULL, m);
  }
}


/**
 *
 */
static int
api_devicetree(http_connection_t *hc, const char *remain, void *opaque)
{
  htsmsg_t *reply;
  const char *node = http_arg_get(&hc->hc_req_args, "node");
  if(node == NULL)
    return HTTP_STATUS_BAD_REQUEST;

  reply = htsmsg_create_list();

  if(!strcmp(node, "root")) {
    devicetree_add_list(reply, &conf_devices);
    devicetree_add_list(reply, &running_devices);

  }
  
  return http_send_json(hc, reply);
}


/**
 * WEB user interface
 */
void
api_extjs_init(void)
{
  http_path_add("/api/extjs/devicetree", 
		NULL, api_devicetree, ACCESS_WEB_INTERFACE);
}
