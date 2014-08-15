/*
   Widgets for the Midnight Commander

   Copyright (C) 1994-2014
   Free Software Foundation, Inc.

   Authors: Andrey Bachmaga

   This file is part of the Midnight Commander.

   The Midnight Commander is free software: you can redistribute it
   and/or modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the License,
   or (at your option) any later version.

   The Midnight Commander is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/** \file speedchart.c
 *  \brief Source: WSpeedChart widget (progress indicator with speed diagram on it)
 */

#include <config.h>

#include <stdlib.h>
#include <string.h>

#include "lib/global.h"

#include "lib/tty/tty.h"
#include "lib/tty/color.h"
#include "lib/skin.h"
#include "lib/widget.h"

/*** global variables ****************************************************************************/

/*** file scope macro definitions ****************************************************************/

/*** file scope type declarations ****************************************************************/

/*** file scope variables ************************************************************************/
char *s1;
/*** file scope functions ************************************************************************/

static cb_ret_t
speedchart_callback (Widget * w, Widget * sender, widget_msg_t msg, int parm, void *data)
{
    WSpeedChart *g = SPEEDCHART (w);
    WDialog *h = w->owner;

    switch (msg)
    {
    case MSG_INIT:
        return MSG_HANDLED;

        /* We don't want to get the focus */
    case MSG_FOCUS:
        return MSG_NOT_HANDLED;

    case MSG_DRAW:
        widget_move (w, 0, 0);
        if (!g->shown)
        {
            tty_setcolor (h->color[DLG_COLOR_NORMAL]);
            tty_printf ("%*s", w->cols, "");
        }
        else
        {
            int gauge_len;
            int percentage, columns;
            long total = g->max;
            long done = g->current;

            if (total <= 0 || done < 0)
            {
                done = 0;
                total = 100;
            }
            if (done > total)
                done = total;
            while (total > 65535)
            {
                total /= 256;
                done /= 256;
            }

            gauge_len = w->cols;

            percentage = done / total;
            columns = gauge_len * done / total; // (2 * gauge_len * done / total + 1) / 2;

            // drawing lines from down to up
            tty_setcolor (GAUGE_COLOR);
            //tty_printf ("%*s", (int) columns, "");
            s1 = (char *)malloc(w->cols + 1);
            for (int r = g->rows; r > 0; r--) {
                for (int i = 0; i < columns; i++)
                    s1[i] = 'X';
                s1[columns] = 0;
                tty_print_string(s1);
            }

            free(s1);

            tty_setcolor (h->color[DLG_COLOR_NORMAL]);
            tty_printf ("%*s", gauge_len - columns, "");
        }
        return MSG_HANDLED;

    default:
        return widget_default_callback (w, sender, msg, parm, data);
    }
}

/* --------------------------------------------------------------------------------------------- */
/*** public functions ****************************************************************************/
/* --------------------------------------------------------------------------------------------- */

WSpeedChart *
speedchart_new (int y, int x, int cols, gboolean shown, int max, int current)
{
    WSpeedChart *g;
    Widget *w;

    g = g_new (WSpeedChart, 1);
    w = WIDGET (g);
    widget_init (w, y, x, 5, cols, speedchart_callback, NULL);
    widget_want_cursor (w, FALSE);
    widget_want_hotkey (w, FALSE);

    g->shown = shown;
    if (max == 0)
        max = 1;                /* I do not like division by zero :) */
    g->max = max;
    g->current = current;

    return g;
}

/* --------------------------------------------------------------------------------------------- */

void
speedchart_set_value (WSpeedChart * g, int max, int current)
{
    if (g->current == current && g->max == max)
        return;                 /* Do not flicker */

    if (max == 0)
        max = 1;                /* I do not like division by zero :) */
    g->current = current;
    g->max = max;
    widget_redraw (WIDGET (g));
}

/* --------------------------------------------------------------------------------------------- */

void
speedchart_show (WSpeedChart * g, gboolean shown)
{
    if (g->shown != shown)
    {
        g->shown = shown;
        widget_redraw (WIDGET (g));
    }
}

/* --------------------------------------------------------------------------------------------- */
