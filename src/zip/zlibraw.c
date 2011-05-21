/*!The Tiny Box Library
 * 
 * TBox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * TBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with TBox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2009 - 2010, ruki All rights reserved.
 *
 * \author		ruki
 * \file		zlibraw.c
 *
 */
/* /////////////////////////////////////////////////////////
 * includes
 */
#include "zlibraw.h"

/* /////////////////////////////////////////////////////////
 * implements
 */
static __tplat_inline__ tb_zip_zlibraw_t* tb_zip_zlibraw_cast(tb_zip_t* zip)
{
	TB_ASSERT_RETURN_VAL(zip && zip->algo == TB_ZIP_ALGO_ZLIBRAW, TB_NULL);
	return (tb_zip_zlibraw_t*)zip;
}
static void tb_zip_zlibraw_close(tb_zip_t* zip)
{
	tb_zip_zlibraw_t* zlibraw = tb_zip_zlibraw_cast(zip);
	if (zlibraw) 
	{
		// close zst
		if (zip->action == TB_ZIP_ACTION_INFLATE) inflateEnd(&(zlibraw->zst));
		else if (zip->action == TB_ZIP_ACTION_DEFLATE) deflateEnd(&(zlibraw->zst));

		// reset it
		memset(zlibraw, 0, sizeof(tb_zip_zlibraw_t));
	}
}
static tb_zip_status_t tb_zip_zlibraw_spank_deflate(tb_zip_t* zip, tb_bstream_t* ist, tb_bstream_t* ost)
{
	tb_zip_zlibraw_t* zlibraw = tb_zip_zlibraw_cast(zip);
	TB_ASSERT_RETURN_VAL(zlibraw && ist && ost, TB_ZIP_STATUS_FAIL);

	// the input stream
	tb_byte_t* ip = ist->p;
	tb_byte_t* ie = ist->e;
	TB_ASSERT_RETURN_VAL(ip && ie, TB_ZIP_STATUS_FAIL);

	// the output stream
	tb_byte_t* op = ost->p;
	tb_byte_t* oe = ost->e;
	TB_ASSERT_RETURN_VAL(op && oe, TB_ZIP_STATUS_FAIL);

	// attach zst
	zlibraw->zst.next_in = (Bytef*)ip;
	zlibraw->zst.avail_in = (uInt)(ie - ip);

	zlibraw->zst.next_out = (Bytef*)op;
	zlibraw->zst.avail_out = (uInt)(oe - op);

	// deflate 
	tb_int_t ret = deflate(&zlibraw->zst, Z_NO_FLUSH);
	TB_ASSERT_RETURN_VAL(ret == Z_OK || ret == Z_STREAM_END, TB_ZIP_STATUS_FAIL);
	//TB_DBG("deflate: %d", zlibraw->zst.total_out);

	// update 
	ist->p = (tb_byte_t*)zlibraw->zst.next_in;
	ost->p = (tb_byte_t*)zlibraw->zst.next_out;

	// ok?
	return (ret == Z_STREAM_END)? TB_ZIP_STATUS_END : TB_ZIP_STATUS_OK;
}
static tb_zip_status_t tb_zip_zlibraw_spank_inflate(tb_zip_t* zip, tb_bstream_t* ist, tb_bstream_t* ost)
{
	tb_zip_zlibraw_t* zlibraw = tb_zip_zlibraw_cast(zip);
	TB_ASSERT_RETURN_VAL(zlibraw && ist && ost, TB_ZIP_STATUS_FAIL);

	// the input stream
	tb_byte_t* ip = ist->p;
	tb_byte_t* ie = ist->e;
	TB_ASSERT_RETURN_VAL(ip && ie, TB_ZIP_STATUS_FAIL);

	// the output stream
	tb_byte_t* op = ost->p;
	tb_byte_t* oe = ost->e;
	TB_ASSERT_RETURN_VAL(op && oe, TB_ZIP_STATUS_FAIL);

	// attach zst
	zlibraw->zst.next_in = (Bytef*)ip;
	zlibraw->zst.avail_in = (uInt)(ie - ip);

	zlibraw->zst.next_out = (Bytef*)op;
	zlibraw->zst.avail_out = (uInt)(oe - op);

	// inflate 
	tb_int_t ret = inflate(&zlibraw->zst, Z_NO_FLUSH);
	TB_ASSERT_RETURN_VAL(ret == Z_OK || ret == Z_STREAM_END, TB_ZIP_STATUS_FAIL);
	//TB_DBG("inflate: %d", zlibraw->zst.total_out);

	// update 
	ist->p = (tb_byte_t*)zlibraw->zst.next_in;
	ost->p = (tb_byte_t*)zlibraw->zst.next_out;

	// ok?
	return (ret == Z_STREAM_END)? TB_ZIP_STATUS_END : TB_ZIP_STATUS_OK;
}

/* /////////////////////////////////////////////////////////
 * interfaces
 */

tb_zip_t* tb_zip_zlibraw_open(tb_zip_zlibraw_t* zlibraw, tb_zip_action_t action)
{
	tb_zip_t* zip = (tb_zip_t*)zlibraw;
	TB_ASSERT_RETURN_VAL(zip, TB_NULL);
	
	// init zip
	memset(zip, 0, sizeof(tb_zip_zlibraw_t));
	zip->algo 		= TB_ZIP_ALGO_ZLIBRAW;
	zip->action 	= action;
	zip->close 		= tb_zip_zlibraw_close;

	// open zst
	switch (action)
	{
	case TB_ZIP_ACTION_INFLATE:
		{
			zip->spank = tb_zip_zlibraw_spank_inflate;

			// no zlib header
			if (inflateInit(&zlibraw->zst) != Z_OK) return TB_NULL;
		}
		break;
	case TB_ZIP_ACTION_DEFLATE:
		{
			zip->spank = tb_zip_zlibraw_spank_deflate;
			if (deflateInit(&zlibraw->zst, Z_DEFAULT_COMPRESSION) != Z_OK) return TB_NULL;
		}
		break;
	default:
		break;
	}

	return zip;
}