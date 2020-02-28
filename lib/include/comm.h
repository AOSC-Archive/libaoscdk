/**
 * @file comm.h
 * @author Junde Yhi <lmy441900@aosc.xyz>
 * @copyright (C) 2019-2020 Anthon Open Source Community
 *
 * Definition of the communication infrastructure for libaoscdk.
 */

#ifndef LIBAOSCDK_COMM_H
#define LIBAOSCDK_COMM_H

#include <glib.h>

int dk_comm_notify(const char *method, GVariant *params);

int dk_comm_call(const char *method, GVariant *params);

int dk_comm_respond(const char *method, const int id, GVariant *params);

#endif
