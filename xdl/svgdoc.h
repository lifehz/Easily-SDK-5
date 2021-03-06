﻿/***********************************************************************
	Easily xdl v5.5

	(c) 2013-2016 JianDe LiFang Technology Corporation.  All Rights Reserved.

	@author ZhangWenQuan, JianDe HangZhou ZheJiang China, Mail: powersuite@hotmaol.com

	@doc title document

	@module	svgdoc.h | svg document interface file

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

#ifndef _SVGDOC_H
#define _SVGDOC_H

#include "xdldef.h"

#ifdef XDL_SUPPORT_DOC

/************************************************Properties***************************************************************************/

/*
@PROPER name: string.
@SET set_svg_node_name: set the svg node name.
*/
#define set_svg_node_name(nlk,token)						set_dom_node_name(nlk,token,-1)
/*
@PROPER name: string.
@GET get_svg_node_name_ptr: get the svg node name.
*/
#define get_svg_node_name_ptr(nlk)							get_dom_node_name_ptr(nlk)
/*
@PROPER text: string.
@SET set_svg_node_text: set the svg node text.
*/
#define set_svg_node_text(nlk,token,len)					set_dom_node_text(nlk,token,len)
/*
@PROPER name: string.
@GET get_svg_node_text_ptr: get the svg node text.
*/
#define get_svg_node_text_ptr(nlk)							get_dom_node_text_ptr(nlk)

#define set_svg_node_attr(nlk,key,val)						set_dom_node_attr(nlk,key,-1,val,-1)
#define get_svg_node_attr_ptr(nlk,key)						get_dom_node_attr_ptr(nlk,key)

#ifdef	__cplusplus
extern "C" {
#endif

/************************************************Functions***************************************************************************/

/*
@FUNCTION create_svg_doc: create a svg document.
@RETURN link_t_ptr: return the svg document link component.
*/
XDL_API link_t_ptr create_svg_doc(void);

/*
@FUNCTION destroy_svg_doc: destroy a svg document.
@INPUT link_t_ptr ptr: the svg document link component.
@RETURN void: none.
*/
XDL_API void destroy_svg_doc(link_t_ptr ptr);

/*
@FUNCTION clear_svg_doc: clear a svg document, all of child nodes in svg document will be emptied.
@INPUT link_t_ptr ptr: the svg document link component.
@RETURN void: none.
*/
XDL_API void clear_svg_doc(link_t_ptr ptr);

/*
@FUNCTION is_svg_doc: test is a svg document.
@INPUT link_t_ptr ptr: the svg document link component.
@RETURN bool_t: return nonzero for being a svg document, otherwise return zero.
*/
XDL_API bool_t is_svg_doc(link_t_ptr ptr);

/*
@FUNCTION is_svg_node: test is a svg child node.
@INPUT link_t_ptr ptr: the svg document link component.
@INPUT link_t_ptr nlk: the svg node link component.
@RETURN bool_t: return nonzero for being a svg child node, otherwise return zero.
*/
XDL_API bool_t is_svg_node(link_t_ptr ptr, link_t_ptr nlk);

/*
@FUNCTION insert_svg_node: insert a new svg child node.
@INPUT link_t_ptr nlk: the svg node link component.
@RETURN link_t_ptr: return the new svg node link component.
*/
XDL_API link_t_ptr insert_svg_node(link_t_ptr nlk);

/*
@FUNCTION delete_svg_node: delete a svg node and its child nodes.
@INPUT link_t_ptr nlk: the svg node link component.
@RETURN void: none.
*/
XDL_API void delete_svg_node(link_t_ptr nlk);

/*
@FUNCTION get_svg_first_child_node: get svg first child node.
@INPUT link_t_ptr nlk: the parent svg node link component.
@RETURN link_t_ptr: return the node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_svg_first_child_node(link_t_ptr nlk);

/*
@FUNCTION get_svg_last_child_node: get svg last child node.
@INPUT link_t_ptr nlk: the parent svg node link component.
@RETURN link_t_ptr: return the node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_svg_last_child_node(link_t_ptr nlk);

/*
@FUNCTION get_svg_parent_node: get svg parent node.
@INPUT link_t_ptr nlk: the svg node link component.
@RETURN link_t_ptr: return the parent node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_svg_parent_node(link_t_ptr nlk);

/*
@FUNCTION get_svg_next_sibling_node: get svg next sibling node.
@INPUT link_t_ptr nlk: the svg node link component.
@RETURN link_t_ptr: return the next sibling node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_svg_next_sibling_node(link_t_ptr nlk);

/*
@FUNCTION get_svg_prev_sibling_node: get svg previous sibling node.
@INPUT link_t_ptr nlk: the svg node link component.
@RETURN link_t_ptr: return the previous sibling node link component if exists, otherwise return NULL.
*/
XDL_API link_t_ptr get_svg_prev_sibling_node(link_t_ptr nlk);

/*****************************************************************************************************************/

/*
@FUNCTION set_svg_width: set svg width.
@INPUT link_t_ptr ptr: the svg link component.
@INPUT float width: the width in millimeter.
@RETURN void: none.
*/
XDL_API void set_svg_width(link_t_ptr ptr, float width);

/*
@FUNCTION get_svg_width: get svg width.
@INPUT link_t_ptr ptr: the svg link component.
@RETURN float: return the width in millimeter.
*/
XDL_API float get_svg_width(link_t_ptr ptr);

/*
@FUNCTION set_svg_height: set svg height.
@INPUT link_t_ptr ptr: the svg link component.
@INPUT float height: the height in millimeter.
@RETURN void: none.
*/
XDL_API void set_svg_height(link_t_ptr ptr, float height);

/*
@FUNCTION get_svg_height: get svg height.
@INPUT link_t_ptr ptr: the svg link component.
@RETURN float: return the height in millimeter.
*/
XDL_API float get_svg_height(link_t_ptr ptr);

/*
@FUNCTION set_svg_viewbox: set svg view box.
@INPUT link_t_ptr ptr: the svg link component.
@INPUT const xrect_t* pbox: the rect struct, using long member.
@RETURN void: none.
*/
XDL_API void set_svg_viewbox(link_t_ptr ptr, const xrect_t* pbox);

/*
@FUNCTION get_svg_viewbox: get svg view box.
@INPUT link_t_ptr ptr: the svg link component.
@OUTPUT xrect_t* pbox: the rect struct, using long member.
@RETURN void: none.
*/
XDL_API void get_svg_viewbox(link_t_ptr ptr, xrect_t* pbox);

/*
@FUNCTION write_line_to_svg_node: write line attributes to the svg node.
@INPUT link_t_ptr glk: the svg node component.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt1: the one point struct using long member.
@INPUT const xpoint_t* ppt2: the two point struct using long member.
@RETURN void: none.
*/
XDL_API void write_line_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2);

/*
@FUNCTION read_line_from_svg_node: read line attributes from the svg node.
@INPUT link_t_ptr glk: the svg node component.
@OUTPUT xpen_t* pxp: the pen struct for returning.
@OUTPUT xpoint_t* ppt1: the one point struct for returning in long member.
@OUTPUT xpoint_t* ppt2: the two point struct for returning in long member.
@RETURN void: none.
*/
XDL_API void read_line_from_svg_node(link_t_ptr glk, xpen_t* pxp, xpoint_t* ppt1, xpoint_t* ppt2);

/*
@FUNCTION write_polyline_to_svg_node: write polyline attributes to the svg node.
@INPUT link_t_ptr glk: the svg node component.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt: the point struct array.
@INPUT int n: the number of point in array.
@RETURN void: none.
*/
XDL_API void write_polyline_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xpoint_t* ppt, int n);

/*
@FUNCTION read_polyline_from_svg_node: read ployline attributes from the svg node.
@INPUT link_t_ptr glk: the svg node component.
@OUTPUT xpen_t* pxp: the pen struct for returning.
@OUTPUT xpoint_t* ppt: the point struct array for returning in long member, you can let it NULL to test the number of points beforehand.
@INPUT int n: the array size.
@RETURN int: return the number of points.
*/
XDL_API int read_polyline_from_svg_node(link_t_ptr glk, xpen_t* pxp, xpoint_t* ppt, int n);

/*
@FUNCTION write_polygon_to_svg_node: write polygon attributes to the svg node.
@INPUT link_t_ptr glk: the svg node component.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xpoint_t* ppt: the point struct array.
@INPUT int n: the number of point in array.
@RETURN void: none.
*/
XDL_API void write_polygon_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, const xpoint_t* ppt, int n);

/*
@FUNCTION read_polygon_from_svg_node: read ploygon attributes from the svg node.
@INPUT link_t_ptr glk: the svg node component.
@OUTPUT xpen_t* pxp: the pen struct for returning.
@OUTPUT xbrush_t* pxb: the brush struct for returning.
@OUTPUT xpoint_t* ppt: the point struct array for returning in long member, you can let it NULL to test the number of points beforehand.
@INPUT int n: the array size.
@RETURN int: return the number of points.
*/
XDL_API int read_polygon_from_svg_node(link_t_ptr glk, xpen_t* pxp, xbrush_t* pxb, xpoint_t* ppt, int n);

/*
@FUNCTION write_bezier_to_svg_node: write bezier attributes to the svg node.
@INPUT link_t_ptr glk: the svg node component.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xpoint_t* ppt1: the start point.
@INPUT const xpoint_t* ppt2: the control point.
@INPUT const xpoint_t* ppt3: the control point.
@INPUT const xpoint_t* ppt4: the end point.
@RETURN void: none.
*/
XDL_API void write_bezier_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xpoint_t* ppt1, const xpoint_t* ppt2, const xpoint_t* ppt3, const xpoint_t* ppt4);

/*
@FUNCTION write_rect_to_svg_node: write rect attributes to the svg node.
@INPUT link_t_ptr glk: the svg node component.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* prt: the rect struct using long member.
@RETURN void: none.
*/
XDL_API void write_rect_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt);

/*
@FUNCTION read_rect_from_svg_node: read rect attributes from the svg node.
@INPUT link_t_ptr glk: the svg node component.
@OUTPUT xpen_t* pxp: the pen struct for returning.
@OUTPUT xbrush_t* pxb: the brush struct for returning.
@OUTPUT xrect_t* prt: the rect struct for returning in long member.
@RETURN void: none
*/
XDL_API void read_rect_from_svg_node(link_t_ptr glk, xpen_t* pxp, xbrush_t* pxb, xrect_t* prt);

/*
@FUNCTION svg_node_is_round: test the node is round node.
@INPUT link_t_ptr glk: the svg node component.
@RETURN bool_t: return nonzero for round node, otherwise return zero.
*/
XDL_API bool_t svg_node_is_round(link_t_ptr glk);

/*
@FUNCTION write_round_to_svg_node: write round attributes to the svg node.
@INPUT link_t_ptr glk: the svg node component.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* prt: the rect struct using long member.
@INPUT long rx: the rx value.
@INPUT long ry: the ry value.
@RETURN void: none.
*/
XDL_API void write_round_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt, long rx, long ry);

/*
@FUNCTION read_round_from_svg_node: read round attributes from the svg node.
@INPUT link_t_ptr glk: the svg node component.
@OUTPUT xpen_t* pxp: the pen struct for returning.
@OUTPUT xbrush_t* pxb: the brush struct for returning.
@OUTPUT xrect_t* prt: the rect struct for returning in long member.
@OUTPUT long* prx: for returning rx value.
@OUTPUT long* pry: for returning ry value.
@RETURN void: none
*/
XDL_API void read_round_from_svg_node(link_t_ptr glk, xpen_t* pxp, xbrush_t* pxb, xrect_t* prt, long* prx, long* pry);

/*
@FUNCTION write_ellipse_to_svg_node: write ellipse attributes to the svg node.
@INPUT link_t_ptr glk: the svg node component.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* prt: the rect struct using long member.
@RETURN void: none.
*/
XDL_API void write_ellipse_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt);

/*
@FUNCTION read_ellipse_from_svg_node: read ellipse attributes from the svg node.
@INPUT link_t_ptr glk: the svg node component.
@OUTPUT xpen_t* pxp: the pen struct for returning.
@OUTPUT xbrush_t* pxb: the brush struct for returning.
@OUTPUT xrect_t* prt: the rect struct for returning in long member.
@RETURN void: none
*/
XDL_API void read_ellipse_from_svg_node(link_t_ptr glk, xpen_t* pxp, xbrush_t* pxb, xrect_t* prt);

/*
@FUNCTION svg_node_is_pie: test the node is pie node.
@INPUT link_t_ptr glk: the svg node component.
@RETURN bool_t: return nonzero for pie node, otherwise return zero.
*/
XDL_API bool_t svg_node_is_pie(link_t_ptr glk);

/*
@FUNCTION write_pie_to_svg_node: write pie attributes to the svg node.
@INPUT link_t_ptr glk: the svg node component.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* prt: the rect struct using long member.
@INPUT double fang: the start angle.
@INPUT double tang: the end angle.
@RETURN void: none.
*/
XDL_API void write_pie_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* pxr, double fang, double tang);

/*
@FUNCTION read_ellipse_from_svg_node: read pie attributes from the svg node.
@INPUT link_t_ptr glk: the svg node component.
@OUTPUT xpen_t* pxp: the pen struct for returning.
@OUTPUT xbrush_t* pxb: the brush struct for returning.
@OUTPUT xrect_t* prt: the rect struct for returning in long member.
@OUTPUT double* pfang: for returning start angle.
@OUTPUT double* ptang: for returning end angle.
@RETURN void: none
*/
XDL_API void read_pie_from_svg_node(link_t_ptr glk, xpen_t* pxp, xbrush_t* pxb, xrect_t* pxr, double* pfang, double* ptang);

/*
@FUNCTION write_text_to_svg_node: write text to the svg node.
@INPUT link_t_ptr glk: the svg node component.
@INPUT const xfont_t* pxf: the font struct.
@INPUT const xface_t* pxa: the face struct.
@INPUT const xrect_t* prt: the rect struct using long member.
@INPUT const tchar_t* text: the text string token.
@INPUT int len: the length of text token in characters.
@RETURN void: none.
*/
XDL_API void write_text_to_svg_node(link_t_ptr glk, const xfont_t* pxf, const xface_t* pxa, const xrect_t* prt, const tchar_t* text, int len);

/*
@FUNCTION read_text_from_svg_node: read text attributes from the svg node.
@INPUT link_t_ptr glk: the svg node component.
@OUTPUT xfont_t* pxf: the font struct for returning.
@OUTPUT xface_t* pxa: the face struct for returning.
@OUTPUT xrect_t* prt: the rect struct for returning in long member.
@RETURN const tchar_t*: return text token.
*/
XDL_API const tchar_t* read_text_from_svg_node(link_t_ptr glk, xfont_t* pxf, xface_t* pxa, xrect_t* prt);

/*
@FUNCTION write_shape_to_svg_node: write shape attributes to the svg node.
@INPUT link_t_ptr glk: the svg node component.
@INPUT const xpen_t* pxp: the pen struct.
@INPUT const xbrush_t* pxb: the brush struct.
@INPUT const xrect_t* prt: the rect struct using long member.
@INPUT const tchar_t* shape: the shape name.
@RETURN void: none.
*/
XDL_API void write_shape_to_svg_node(link_t_ptr glk, const xpen_t* pxp, const xbrush_t* pxb, const xrect_t* prt, const tchar_t* shape);

/*
@FUNCTION write_ximage_to_svg_node: write image attributes to the svg node.
@INPUT link_t_ptr glk: the svg node component.
@INPUT const ximage_t* pxi: the image struct.
@INPUT const xrect_t* prt: the rect struct using long member.
@RETURN void: none.
*/
XDL_API void write_ximage_to_svg_node(link_t_ptr glk, const ximage_t* pxi, const xrect_t* prt);

/*
@FUNCTION read_ximage_from_svg_node: read image attributes from the svg node.
@INPUT link_t_ptr glk: the svg node component.
@OUTPUT ximage_t* pxi: the image struct for returning.
@OUTPUT xrect_t* prt: the rect struct for returning in long member.
@RETURN void: none.
*/
XDL_API void read_ximage_from_svg_node(link_t_ptr glk, ximage_t* pxi, xrect_t* prt);

#ifdef	__cplusplus
}
#endif

#endif /*XDL_SUPPORT_DOC*/

#endif /*_SVGDOC_H*/