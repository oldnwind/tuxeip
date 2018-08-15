/***************************************************************************
 *   Copyright (C) 2006 by TuxPLC					                                 *
 *   Author Stephane JEANNE s.jeanne@tuxplc.net                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _LGX_H
#define _LGX_H

#include "TuxDef.h"
#include "CIP_Types.h"
#include "Ethernet_IP.h"
#include "CM.h"
#include "AB.h"

#define CIP_LIST_ALL_OBJECTS  0x4B
#define CIP_DATA_READ         0x4C
#define CIP_DATA_WRITE        0x4D
#define CIP_MULTI_REQUEST     0x0A

#define DT_BOOL               0xC1
#define DT_BITARRAY           0xD3
#define DT_SINT               0xC2
#define DT_INT                0xC3
#define DT_DINT               0xC4
#define DT_REAL               0xCA

// Data Access

typedef struct _CommonDataService_Reply {
	CIP_USINT Service;
	CIP_SINT Reserved;
	CIP_UINT Status;
} PACKED CommonDataService_Reply;

typedef struct _ReadDataService_Request {
	CIP_SINT Service;
	CIP_USINT PathSize;
	CIP_UINT Number;  // number of elements to read
} PACKED ReadDataService_Request;

typedef struct _ReadDataService_Reply {
	CIP_USINT Service;
	CIP_SINT Reserved;
	CIP_UINT Status;
	CIP_UINT DataType;  // only valid for simple Datatypes!!
} PACKED ReadDataService_Reply;

typedef struct _WriteDataService_Request {
	CIP_SINT Service;
	CIP_USINT PathSize;
	CIP_UINT DataType;
	CIP_UINT Number;  // number of elements to read
} PACKED WriteDataService_Request;

typedef struct _WriteDataService_Reply {
	CIP_USINT Service;
	CIP_SINT Reserved;
	CIP_UINT Status;
} PACKED WriteDataService_Reply;

typedef struct _MultiService_Request {
	CIP_SINT Service;
	CIP_USINT PathSize;  // 2
	BYTE Path[4];  //msg router path
	CIP_UINT Count;  // number of element
} PACKED MultiService_Request;

typedef struct _MultiService_Reply {
	CIP_USINT Service;
	CIP_SINT Reserved;
	CIP_USINT GenSTS;
	CIP_SINT Reserved2;
	CIP_UINT Count;
} PACKED MultiService_Reply;

typedef struct _LGX_Read {
	LGX_Data_Type type;
	int Varcount;
	int totalsize;
	int elementsize;
	unsigned int mask;
} PACKED LGX_Read;

typedef struct _IdList {
	int IdCount;
	CIP_UDINT Id[1];
} PACKED IdList;

#define CELL_MAX_TAGS   21000
#define CELL_MAX_PROGS  512
#define CELL_MAX_SBASE  256
#define CELL_MAX_RACKID 32
#define CELL_PKT_SIZE   256
#define CELL_TAG_LEN    64

typedef struct {
	unsigned short type;
	unsigned short arraysize;
	char name[CELL_TAG_LEN];
	BYTE *data;
} _element;

typedef struct {
	unsigned long base;
	unsigned short linkid;
	unsigned short count;
	unsigned short detailsize;
	char name[CELL_TAG_LEN];
	_element *data[64];
} _struct_base;

typedef struct {
	unsigned short count;
	_struct_base *base[CELL_MAX_SBASE];
} _struct_list;

typedef struct {
	unsigned long base;
	unsigned long linkid;
	BYTE name[32];
} _prog_detail;

typedef struct {
	_prog_detail *prog[CELL_MAX_PROGS];
	int count;
} _prog_list;

typedef struct {
	unsigned long topbase;
	unsigned long base;
	unsigned long alias_topbase;
	unsigned long alias_base;
	unsigned long id;
	unsigned long alias_id;
	unsigned long linkid;
	unsigned long memory;
	unsigned long alias_linkid;
	unsigned short type;
	unsigned short alias_type;
	unsigned short displaytype;
	unsigned short size;
	unsigned short alias_size;
	unsigned long arraysize1;
	unsigned long arraysize2;
	unsigned long arraysize3;
	unsigned short datalen;
	long dirty;
	BYTE *data;
	BYTE name[CELL_TAG_LEN];
} _tag_detail;

typedef struct {
	_tag_detail *tag[CELL_MAX_TAGS];
	int count;
	int reference;
} _tag_data;

typedef struct {
	int count;
	_tag_detail *tag[64];
} _tag_list;

// Global Variables

ReadDataService_Request *_BuildLgxReadDataRequest(const char *address, CIP_UINT number, int *requestsize);
WriteDataService_Request *_BuildLgxWriteDataRequest(const char *address, LGX_Data_Type datatype, void *data,
	int datasize, CIP_UINT number, int *requestsize);

LGX_Read *_ReadLgxData(Eip_Session *session, Eip_Connection *connection, const char *address, CIP_UINT number);

#define ReadLgxData(session, connection, address, number) _ReadLgxData(session, connection, address, number)

int _WriteLgxData(Eip_Session *session, Eip_Connection *connection, const char *address, LGX_Data_Type datatype, void *data, CIP_UINT number);

#define WriteLgxData(session, connection, address, datatype, data, number) _WriteLgxData(session, connection, address, datatype, data, number)

CommonDataService_Reply *_GetService_Reply(Eip_Item *dataitem, unsigned int index);

CIP_INT _GetExtendedStatus(CommonDataService_Reply *reply);

int _GetService_ReplyNumber(Eip_Item *dataitem);

CommonDataService_Reply *_GetService_Reply(Eip_Item *dataitem, unsigned int index);

void *_GetData(CommonDataService_Reply *reply);

#define GetData(reply) _GetData(reply)

int _GetLGXDataSize(LGX_Data_Type DataType);

LGX_Data_Type _LGXDataType(Data_Type DataType);

#define LGXDataType _LGXDataType

LGX_Data_Type _GetLGXDataType(CommonDataService_Reply *reply);

LGX_Read *_DecodeLGX(CommonDataService_Reply *reply, int replysize);

int _GetLGXValueAsInteger(LGX_Read *reply, int index);

#define GetLGXValueAsInteger(reply, index) _GetLGXValueAsInteger(reply, index)

float _GetLGXValueAsFloat(LGX_Read *reply, int index);
#define GetLGXValueAsFloat(reply, index) _GetLGXValueAsFloat(reply, index)

void _FreeLGXRead(LGX_Read *Data);

IdList *_GetIdList(Eip_Session *session, BYTE routepath[], int routepathsize);

#endif // _LGX_H
