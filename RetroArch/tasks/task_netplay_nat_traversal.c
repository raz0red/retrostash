/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2017 - Gregor Richards
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>

#include "tasks_internal.h"

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#ifdef HAVE_NETWORKING
#include <net/net_natt.h>
#include "../network/netplay/netplay.h"

struct nat_traversal_state_data
{
   struct natt_status *nat_traversal_state;
   uint16_t port;
};

static void task_netplay_nat_traversal_handler(retro_task_t *task)
{
   struct nat_traversal_state_data *ntsd =
      (struct nat_traversal_state_data *) task->task_data;

   if (natt_new(ntsd->nat_traversal_state))
      natt_init(ntsd->nat_traversal_state, ntsd->port, SOCKET_PROTOCOL_TCP);

   task_set_progress(task, 100);
   task_set_finished(task, true);
}

static void task_netplay_nat_close_handler(retro_task_t *task)
{
   natt_deinit((struct natt_status *) task->task_data,
      SOCKET_PROTOCOL_TCP);

   task_set_progress(task, 100);
   task_set_finished(task, true);
}

static void netplay_nat_traversal_callback(retro_task_t *task,
      void *task_data,
      void *user_data, const char *error)
{
   free(task_data);

   netplay_driver_ctl(RARCH_NETPLAY_CTL_FINISHED_NAT_TRAVERSAL, NULL);
}

static bool nat_task_finder(retro_task_t *task, void *userdata)
{
   if (!task)
      return false;

   return task->handler == task_netplay_nat_traversal_handler ||
      task->handler == task_netplay_nat_close_handler;
}

static bool nat_task_queued(void *data)
{
   task_finder_data_t find_data = {nat_task_finder, NULL};

   return task_queue_find(&find_data);
}

bool task_push_netplay_nat_traversal(void *nat_traversal_state, uint16_t port)
{
   retro_task_t *task;
   struct nat_traversal_state_data *ntsd;

   /* Do not run more than one NAT task at a time. */
   task_queue_wait(nat_task_queued, NULL);

   task = task_init();
   if (!task)
      return false;

   ntsd = (struct nat_traversal_state_data *) malloc(sizeof(*ntsd));
   if (!ntsd)
   {
      free(task);
      return false;
   }

   ntsd->nat_traversal_state = (struct natt_status *) nat_traversal_state;
   ntsd->port                = port;

   task->handler             = task_netplay_nat_traversal_handler;
   task->callback            = netplay_nat_traversal_callback;
   task->task_data           = ntsd;

   task_queue_push(task);

   return true;
}

bool task_push_netplay_nat_close(void *nat_traversal_state)
{
   retro_task_t *task;

   /* Do not run more than one NAT task at a time. */
   task_queue_wait(nat_task_queued, NULL);

   task = task_init();
   if (!task)
      return false;

   task->handler   = task_netplay_nat_close_handler;
   task->task_data = nat_traversal_state;

   task_queue_push(task);

   return true;
}
#else
bool task_push_netplay_nat_traversal(void *nat_traversal_state, uint16_t port) { return false; }
bool task_push_netplay_nat_close(void *nat_traversal_state) { return false; }
#endif
