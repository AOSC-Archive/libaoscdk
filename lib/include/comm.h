#ifndef AOSC_DK_COMM_H
#define AOSC_DK_COMM_H

#include <glib.h>

int dk_comm_notify(const char *method, GVariant *params);
int dk_comm_call(const char *method, GVariant *params);

int dk_comm_respond(const char *method, const int id, GVariant *params);

#endif
