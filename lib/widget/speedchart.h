
/** \file speedchart.h
 *  \brief Header: WSpeedChart widget
 */

#ifndef MC__WIDGET_SPEEDCHART_H
#define MC__WIDGET_SPEEDCHART_H

/*** typedefs(not structures) and defined constants **********************************************/

#define SPEEDCHART(x) ((WSpeedChart *)(x))

/*** enums ***************************************************************************************/

/*** structures declarations (and typedefs of structures)*****************************************/

typedef struct WSpeedChart
{
    Widget widget;
    gboolean shown;
    int max;
    int current;
    long *speeds;
    gboolean from_left_to_right;
} WSpeedChart;

/*** global variables defined in .c file *********************************************************/

/*** declarations of public functions ************************************************************/

WSpeedChart *speedchart_new (int y, int x, int cols, gboolean shown, int max, int current);
void speedchart_set_value (WSpeedChart * g, int max, int current);
void speedchart_show (WSpeedChart * g, gboolean shown);

/*** inline functions ****************************************************************************/

#endif /* MC__WIDGET_SPEEDCHART_H */
