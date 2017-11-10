/* query_sysenv_logs.h
 *
 * <copyright>
 * Copyright (C) 2017 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * </endcopyright>
 */

/** \f
 *  Query configuration files, logs, and output of logging commands.
 */

#ifndef QUERY_SYSENV_LOGS_H_
#define QUERY_SYSENV_LOGS_H_

#include "query_sysenv_base.h"

void probe_logs(Env_Accumulator * accum);
void probe_config_files(Env_Accumulator * accum);

#endif /* QUERY_SYSENV_LOGS_H_ */
