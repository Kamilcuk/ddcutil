/*
 * i2c_shim.h
 *
 *  Created on: Nov 17, 2015
 *      Author: rock
 */

#ifndef I2C_SHIM_H_
#define I2C_SHIM_H_

#include <util/coredefs.h>
#include <base/status_code_mgt.h>
#include <i2c/i2c_base_io.h>


typedef struct {
   I2C_Writer i2c_writer;
   I2C_Reader i2c_reader;
   char *     i2c_writer_name;
   char *     i2c_reader_name;
} I2C_IO_Strategy;

// extern I2C_IO_Strategy * i2c_io_strategy;    // for i2c_shim


void set_i2c_io_strategy(I2C_IO_Strategy_Id strategy_id);






Global_Status_Code shim_i2c_writer(
      int    fh,
      int    bytect,
      Byte * bytes_to_write,
      int    sleep_millisec);
Global_Status_Code shim_i2c_reader(
       int        fh,
       int        bytect,
       Byte *     readbuf,
       int        sleep_millisec);

#endif /* I2C_SHIM_H_ */
