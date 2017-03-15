/* adl_shim.c
 *
 * <copyright>
 * Copyright (C) 2014-2017 Sanford Rockowitz <rockowitz@minsoft.com>
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

/** \file
 * Implementation of interface in adl/adl_shim.h for use when building
 * **ddcutil** with ADL support.
 */

/** \cond */
#include <assert.h>
#include <stdlib.h>     // wchar_t, needed by adl_structures.h
#include <stdbool.h>
#include <string.h>
/** \endcond */

#include "public/ddcutil_types.h"

#include "util/edid.h"

#include "base/core.h"
#include "base/displays.h"
#include "base/execution_stats.h"
#include "base/status_code_mgt.h"

#include "adl/adl_impl/adl_intf.h"

#include "adl/adl_shim.h"

// Initialization

/** Wrappers adl_intf function adl_is_available().
 *
 * @return true if ADL interface has been initialized, false if not
 */
bool adlshim_is_available() {
   return adl_is_available();
}


/** Initialize the ADL subsystem. Wrappers function adl_initialize().
 *
 * Must be called before any other function (except adlshim_is_available()):
 *
 * @retval true  success
 * @retval false failure
 */
bool adlshim_initialize() {
   return adl_initialize();
}


void adlshim_release() {
   adl_release();
}


// Report on active displays

Parsed_Edid*
adlshim_get_parsed_edid_by_display_handle(
      Display_Handle * dh)
{
   ASSERT_DISPLAY_IO_MODE(dh, DDC_IO_ADL);
   // assert(dh->io_mode == DDC_IO_ADL);
   return adl_get_parsed_edid_by_adlno(dh->iAdapterIndex, dh->iDisplayIndex);
}


Parsed_Edid*
adlshim_get_parsed_edid_by_display_ref(
      Display_Ref * dref)
{
   // assert(dref->io_mode == DDC_IO_ADL);
   ASSERT_DISPLAY_IO_MODE(dref, DDC_IO_ADL);
   return adl_get_parsed_edid_by_adlno(dref->iAdapterIndex, dref->iDisplayIndex);
}

#ifdef UNUSED
// needed?
void adlshim_show_active_display_by_adlno(int iAdapterIndex, int iDisplayIndex, int depth) {
   return adl_report_active_display_by_adlno(iAdapterIndex, iDisplayIndex, depth);
}
#endif


void adlshim_report_active_display_by_display_ref(Display_Ref * dref, int depth) {
   ASSERT_DISPLAY_IO_MODE(dref, DDC_IO_ADL);
   return adl_report_active_display_by_adlno(dref->iAdapterIndex, dref->iDisplayIndex, depth);
}


// Find and validate display

bool              adlshim_is_valid_display_ref(Display_Ref * dref, bool emit_error_msg) {
   // assert(dref->ddc_io_mode == DDC_IO_ADL);
   ASSERT_DISPLAY_IO_MODE(dref, DDC_IO_ADL);
   return adl_is_valid_adlno(dref->iAdapterIndex, dref->iDisplayIndex, emit_error_msg);
}

Display_Ref * adlshim_find_display_by_mfg_model_sn(const char * mfg_id, const char * model, const char * sn) {
   Display_Ref * dref = NULL;
   ADL_Display_Rec * adl_rec = adl_find_display_by_mfg_model_sn(mfg_id, model, sn);
   if (adl_rec)
      dref = create_adl_display_ref(adl_rec->iAdapterIndex, adl_rec->iDisplayIndex);
   return dref;
}

Display_Ref * adlshim_find_display_by_edid(const Byte * pEdidBytes) {
   Display_Ref * dref = NULL;
   ADL_Display_Rec * adl_rec = adl_find_display_by_edid(pEdidBytes);
   if (adl_rec)
      dref = create_adl_display_ref(adl_rec->iAdapterIndex, adl_rec->iDisplayIndex);
   return dref;
}

Display_Info_List adlshim_get_valid_displays() {
   return adl_get_valid_displays();
}


/** Get video card information for an ADL display.
 *
 * @param  dh  display handle
 * @param  card_info pointer to Video_Card_Info struct to be filled in
 * @return modulated ADL status code
 */
Modulated_Status_ADL
adlshim_get_video_card_info(
      Display_Handle *  dh,
      Video_Card_Info * card_info)
{
   Base_Status_ADL adlrc = adl_get_video_card_info_by_adlno(
                              dh->iAdapterIndex,
                              dh->iDisplayIndex,
                              card_info);
   return modulate_rc(adlrc, RR_ADL);
}

// Read from and write to the display

/** Issues a DDC write through ADL.
 *
 * @param  dh            display handle
 * @param  pSendMsgBuf   pointer to bytes to send
 * @param  sendMsgLen    number of bytes to send
 *
 * @return modulated ADL status code
 */
Modulated_Status_ADL
adlshim_ddc_write_only(
      Display_Handle* dh,
      Byte *  pSendMsgBuf,
      int     sendMsgLen)
{
   assert(dh->io_mode == DDC_IO_ADL);
   Base_Status_ADL adlrc = adl_ddc_write_only(dh->iAdapterIndex, dh->iDisplayIndex, pSendMsgBuf, sendMsgLen);
   return modulate_rc(adlrc, RR_ADL);
}

/** Issues a DDC read through ADL.
 *
 * @param  dh            display handle
 * @param  pRcvMsgBuf    pointer to buffer in which to return data read
 * @param  pRcvBytect    pointer to location where number of bytes read is stored
 *
 * @return modulated ADL status code
 */
Modulated_Status_ADL adlshim_ddc_read_only(
      Display_Handle* dh,
      Byte *  pRcvMsgBuf,
      int *   pRcvBytect)
{
   assert(dh->io_mode == DDC_IO_ADL);
   Base_Status_ADL adlrc = adl_ddc_read_only(dh->iAdapterIndex, dh->iDisplayIndex, pRcvMsgBuf, pRcvBytect);
   return modulate_rc(adlrc, RR_ADL);
}

//Base_Status_ADL adl_ddc_write_read(
//      int     iAdapterIndex,
//      int     iDisplayIndex,
//      Byte *  pSendMsgBuf,
//      int     sendMsgLen,
//      Byte *  pRcvMsgBuf,
//      int *   pRcvBytect);

//Base_Status_ADL adl_ddc_write_read_onecall(
//      int     iAdapterIndex,
//      int     iDisplayIndex,
//      Byte *  pSendMsgBuf,
//      int     sendMsgLen,
//      Byte *  pRcvMsgBuf,
//      int *   pRcvBytect);
