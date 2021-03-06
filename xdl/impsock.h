﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc socket document

	@module	impsock.h | socket interface file

	@devnote 张文权 2005.01 - 2007.12	v3.0
	@devnote 张文权 2008.01 - 2009.12	v3.5
	@devnote 张文权 2012.01 - 2015.12	v4.0
	@devnote 张文权 2016.01 - 2016.12	v4.5
	@devnote 张文权 2017.01 - 2017.12	v5.0
	@devnote 张文权 2018.01 - 2018.12	v5.5
***********************************************************************/

/**********************************************************************
This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
LICENSE.GPL3 for more details.
***********************************************************************/

#ifndef _IMPSOCK_H
#define _IMPSOCK_H

#include "xdldef.h"

#ifdef XDK_SUPPORT_SOCK


#ifdef	__cplusplus
extern "C" {
#endif

/*
@FUNCTION xsocket_tcp: create tcp socket.
@INPUT int ver: reserved for socker version.
@INPUT dword_t fmode: file open mode, FILE_OPEN_OVERLAP for asynchronous reading and writing.
@RETURN res_file_t: if succeeds return socket system resource handle, fails return INVALID_FILE.
*/
XDL_API res_file_t xsocket_tcp(int ver, dword_t fmode);

/*
@FUNCTION xsocket_udp: create udp socket.
@INPUT int ver: reserved for socker version.
@INPUT dword_t fmode: file open mode, FILE_OPEN_OVERLAP for asynchronous reading and writing.
@RETURN res_file_t: if succeeds return socket system resource handle, fails return INVALID_FILE.
*/
XDL_API res_file_t xsocket_udp(int ver, dword_t fmode);

/*
@FUNCTION xsocket_icmp: create icmp socket.
@INPUT int ver: reserved for socker version.
@INPUT dword_t fmode: file open mode, FILE_OPEN_OVERLAP for asynchronous reading and writing.
@RETURN res_file_t: if succeeds return socket system resource handle, fails return INVALID_FILE.
*/
XDL_API res_file_t xsocket_icmp(int ver, dword_t fmode);

/*
@FUNCTION xsocket_shutdown: disable socket read or write.
@INPUT res_file_t so: socket resource handle.
@INPUT int how: 0 for disable receiving, 1 for disable sending, 2 for both.
@RETURN void: none.
*/
XDL_API void xsocket_shutdown(res_file_t so, int how);

/*
@FUNCTION xsocket_close: close socket and free resource.
@INPUT res_file_t so: socket resource handle.
@RETURN void: none.
*/
XDL_API void xsocket_close(res_file_t so);

/*
@FUNCTION xsocket_wait: wait socket event.
@INPUT res_file_t so: socket resource handle.
@INPUT dword_t msk: socket event mask for listening, it can be FD_READ, FD_WRITE, FD_ACCEPT, FD_CONNECT or combined.
@RETURN dword_t: the raised event mask, it can be FD_READ, FD_WRITE, FD_ACCEPT, FD_CONNECT or combined.
*/
XDL_API dword_t xsocket_wait(res_file_t so, dword_t msk, int ms);

/*
@FUNCTION xsocket_bind: bind socket to a locall address and a port.
@INPUT res_file_t so: socket resource handle.
@INPUT res_addr_t saddr: the local address and port.
@INPUT int slen: the address struct size in bytes, eg: sizeof(SOCKADDR).
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xsocket_bind(res_file_t so, res_addr_t saddr, int slen);

/*
@FUNCTION xsocket_listen: the listen socket come to waiting client socket connection.
@INPUT res_file_t so: socket resource handle.
@INPUT int max: the maximize connection allowed.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xsocket_listen(res_file_t so, int max);

/*
@FUNCTION xsocket_connect: the client socket connect to listen socket.
@INPUT res_file_t so: socket resource handle.
@INPUT res_addr_t saddr: the remote server address and port.
@INPUT int slen: the address struct size in bytes, eg: sizeof(SOCKADDR).
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	xsocket_connect(res_file_t so, res_addr_t saddr, int slen);

/*
@FUNCTION xsocket_accept: the listen socket accept a connection from client socket.
@INPUT res_file_t so: socket resource handle.
@OUTPUT res_addr_t saddr: the remote client address and port.
@OUTPUT int* plen: return the address struct size in bytes.
@INPUT async_t* pov: the aync struct for operation mode, socket operation can be ASYNC_QUEUE, ASYNC_EVENT async mode or ASYNC_BLOCK blocking mode.
@RETURN res_file_t: if succeeds return socket resource handle, fails return INVALID_FILE.
*/
XDL_API res_file_t xsocket_accept(res_file_t so, res_addr_t saddr, int *plen, async_t* pov);

/*
@FUNCTION xsocket_sendto: write data to some address directly, used by connectionless udp socket.
@INPUT res_file_t so: socket resource handle.
@INPUT res_addr_t saddr: the remote address and port.
@INPUT int alen: the address struct size in bytes.
@INPUT const byte_t*: the data buffer for writing.
@INPUT dword_t size: the data size in bytes.
@INOUTPUT async_t* pov: the aync struct for operation mode, socket operation can be ASYNC_QUEUE, ASYNC_EVENT async mode or ASYNC_BLOCK blocking mode.
the async operation completed bytes returned in async_t struct.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xsocket_sendto(res_file_t so, res_addr_t saddr, int alen, const byte_t* buf, dword_t size, async_t* pov);

/*
@FUNCTION xsocket_recvfrom: read data from some address directly, used by connectionless udp socket.
@INPUT res_file_t so: socket resource handle.
@INPUT res_addr_t saddr: the remote address and port.
@INPUT int alen: the address struct size in bytes.
@OUTPUT byte_t*: the data buffer for reading.
@INPUT dword_t size: the buffer size in bytes.
@INPUT async_t* pov: the aync struct for operation mode, socket operation can be ASYNC_QUEUE, ASYNC_EVENT async mode or ASYNC_BLOCK blocking mode.
the async operation completed bytes returned in async_t struct.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xsocket_recvfrom(res_file_t so, res_addr_t saddr, int *plen, byte_t* buf, dword_t size, async_t* pov);

/*
@FUNCTION xsocket_send: socket write data.
@INPUT res_file_t so: socket resource handle.
@INPUT const byte_t*: the data buffer for writing.
@INPUT dword_t size: the data size in bytes.
@INOUTPUT async_t* pov: the aync struct for operation mode, socket operation can be ASYNC_QUEUE, ASYNC_EVENT async mode or ASYNC_BLOCK blocking mode.
the async operation completed bytes returned in async_t struct.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	xsocket_send(res_file_t so, const byte_t* buf, dword_t size, async_t* pov);

/*
@FUNCTION xsocket_send: socket read data.
@INPUT res_file_t so: socket resource handle.
@OUTPUT byte_t*: the data buffer for reading.
@INPUT dword_t size: the buffer size in bytes.
@INPUT async_t* pov: the aync struct for operation mode, socket operation can be ASYNC_QUEUE, ASYNC_EVENT async mode or ASYNC_BLOCK blocking mode.
the async operation completed bytes returned in async_t struct.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	xsocket_recv(res_file_t so, byte_t* buf, dword_t size, async_t* pov);

/*
@FUNCTION xsocket_setopt: set socket options.
@INPUT res_file_t so: socket resource handle.
@INPUT int optname: the option type.
@INPUT const char* optval: the option value.
@INPUT int optlen: the option value length in characters, not include terminate character.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	xsocket_setopt(res_file_t so, int optname, const char* optval, int optlen);

/*
@FUNCTION xsocket_getopt: get socket options.
@INPUT res_file_t so: socket resource handle.
@INPUT int optname: the option type.
@INPUT char* optval: characters buffer for returning option value.
@INPUT int* plen: the returned option value length in characters, not include terminate character.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	xsocket_getopt(res_file_t so, int optname, char* pval, int* plen);

/*
@FUNCTION xsocket_set_linger: set the socket send routing behave after socket close function called.
@INPUT res_file_t so: socket resource handle.
@INPUT bool_t wait: if nonzero the send routing will continue some time, zero the send routing will be breaked.
@INPUT int sec: if wait is nonzero, the value of second indicate the time will to wait, if wait is zero the value ignored.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	xsocket_set_linger(res_file_t so, bool_t wait, int sec);

/*
@FUNCTION xsocket_set_sndbuf: set the socket inner send buffer size.
@INPUT res_file_t so: socket resource handle.
@INPUT int size: the request buffer size.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	xsocket_set_sndbuf(res_file_t so, int size);

/*
@FUNCTION xsocket_set_rcvbuf: set the socket inner recv buffer size.
@INPUT res_file_t so: socket resource handle.
@INPUT int size: the request buffer size.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	xsocket_set_rcvbuf(res_file_t so, int size);

/*
@FUNCTION xsocket_set_nonblk: set the socket none blocking mode.
@INPUT res_file_t so: socket resource handle.
@INPUT bool_t none: zero for blocking mode.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	xsocket_set_nonblk(res_file_t so, bool_t none);

/*
@FUNCTION host_addr: peer the host name to a ip address.
@INPUT const tchar_t* host: host name.
@OUTPUT tchar_t*: string buffer for returning address.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t	host_addr(const tchar_t* host, tchar_t* addr);

/*
@FUNCTION fill_addr: fill the address struct by ip string token and port.
@OUTPUT net_addr_t* paddr: address struct.
@INTPUT unsigned short port: the port value.
@RETURN void: none.
*/
XDL_API void	fill_addr(net_addr_t* paddr, unsigned short port, const tchar_t* addr);

/*
@FUNCTION conv_addr: convert the address struct to ip string token and port.
@INPUT net_addr_t* paddr: address struct.
@OUTPUT unsigned short* port: the short pointer for returning port value.
@OUTPUT tchar_t* addr: string buffer for returning ip token.
@RETURN void: none.
*/
XDL_API void	conv_addr(net_addr_t* paddr, unsigned short* port, tchar_t* addr);

/*
@FUNCTION xsocket_addr: get socket address binded.
@OUTPUT net_addr_t* paddr: address struct.
@RETURN void: none.
*/
XDL_API void	xsocket_addr(res_file_t so, net_addr_t* paddr);

/*
@FUNCTION xsocket_peer: get remote connected socket address.
@OUTPUT net_addr_t* paddr: address struct.
@RETURN void: none.
*/
XDL_API void	xsocket_peer(res_file_t so, net_addr_t* paddr);

/*
@FUNCTION xsocket_share: describe the socket and write some data into other process.
@INPUT dword_t procid: the destination process id.
@INPUT res_file_t procfd: the file resource handle for writing.
@INPUT res_file_t so: the socket resource handle.
@INPUT const byte_t* data: the extract data need to send.
@INPUT dword_t size: the extract data size in bytes.
@RETURN bool_t: if succeeds return nonzero, fails return zero.
*/
XDL_API bool_t xsocket_share(dword_t procid, res_file_t procfd, res_file_t so, const byte_t* data, dword_t size);

/*
@FUNCTION xsocket_dupli: restore socket from file and read some extract data.
@INPUT res_file_t procfd: the file resource handle for writing.
@INPUT dword_t fmode: the file operation mode, it can be FILE_OPEN_OVERLAP for async reading.
@OUTPUT byte_t* data: the extract data want to read.
@INOUTPUT dword_t* pb: the buffer size in bytes, and return the size of data readed in bytes.
@RETURN res_file_t: if succeeds return socket resource handle, fails return INVALID_FILE.
*/
XDL_API res_file_t xsocket_dupli(res_file_t procfd, dword_t fmode, byte_t* data, dword_t* pb);


#ifdef	__cplusplus
}
#endif

/*declared for inner use*/
int	xsocket_write(void* pso, unsigned char* buf, int len);

/*declared for inner use*/
int	xsocket_read(void* pso, unsigned char* buf, int len);

#endif /*XDK_SUPPORT_SOCK*/

#endif /*IMPSOCK_H*/