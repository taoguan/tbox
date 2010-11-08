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
 * \file		node.c
 *
 */

/* /////////////////////////////////////////////////////////
 * includes
 */
#include "node.h"
#include "nlist.h"
#include <stdarg.h>

/* /////////////////////////////////////////////////////////
 * details
 */
static tb_xml_node_t* tb_xml_node_childs_select_node(tb_xml_node_t* node, tb_string_t* parent, tb_char_t const* path)
{
	TB_ASSERT(node);
	if (!node || !node->childs) return TB_NULL;
	
	// init
	tb_xml_node_t* ret = TB_NULL;
	tb_stack_string_t s;
	tb_string_init_stack_string(&s);

	// find it
	tb_xml_node_t* head = (tb_xml_node_t*)node->childs;
	tb_xml_node_t* item = head->next;
	while (item && item != head)
	{
		if (item->type == TB_XML_NODE_TYPE_ELEMENT)
		{
			// append path
			tb_string_clear((tb_string_t*)&s);
			if (parent) tb_string_append((tb_string_t*)&s, parent);
			tb_string_append_char((tb_string_t*)&s, '/');
			tb_string_append((tb_string_t*)&s, &item->name);
			//TB_DBG("%s: %s", tb_string_c_string(&item->name), tb_string_c_string(&s));
			
			// is this?
			if (TB_TRUE == tb_string_compare_c_string((tb_string_t*)&s, path))
			{
				ret = item;
				break;
			}
			else
			{
				ret = tb_xml_node_childs_select_node(item, (tb_string_t*)&s, path);
				if (ret) break;
			}
		}
		item = item->next;
	}

	tb_string_uninit((tb_string_t*)&s);
	return ret;
}

/* /////////////////////////////////////////////////////////
 * interfaces
 */

void tb_xml_node_init(tb_xml_node_t* node, void* document, tb_size_t type)
{
	if (node)
	{
		// init it
		memset(node, 0, sizeof(tb_xml_node_t));
		node->type = type;
		node->document = document;
		node->prev = node;
		node->next = node;
		tb_string_init(&node->name);
		tb_string_init(&node->value);
	}
}
void tb_xml_node_uninit(tb_xml_node_t* node)
{
	if (node)
	{
		// free specificed data
		if (node->free) node->free(node);

		// free name & value
		tb_string_uninit(&node->name);
		tb_string_uninit(&node->value);

		// free childs
		if (node->childs) tb_xml_nlist_destroy(node->childs);
		node->childs = TB_NULL;

		// free attributes
		if (node->attributes) tb_xml_nlist_destroy(node->attributes);
		node->attributes = TB_NULL;
	}
}
tb_xml_node_t* tb_xml_node_create(void* document, tb_size_t type)
{
	// alloc node
	tb_xml_node_t* node = (tb_xml_node_t*)tb_malloc(sizeof(tb_xml_node_t));
	if (!node) return TB_NULL;

	// init it
	tb_xml_node_init(node, document, type);

	return node;
}
void tb_xml_node_destroy(tb_xml_node_t* node)
{
	if (node)
	{
		// uninit it
		tb_xml_node_uninit(node);

		// free it
		tb_free(node);
	}
}

void tb_xml_node_childs_append(tb_xml_node_t* node, tb_xml_node_t* child)
{
	TB_ASSERT(node && child);
	if (!node || !child) return ;

	if (!node->childs) node->childs = tb_xml_nlist_create();
	if (node->childs) 
	{
		tb_xml_nlist_add(node->childs, child);
		child->parent = node;
	}
}
void tb_xml_node_childs_remove(tb_xml_node_t* node, tb_xml_node_t* child)
{
	TB_ASSERT(node && child);
	if (!node || !child) return ;

	if (node->childs) 
	{
		tb_xml_nlist_det(node->childs, child);
		child->parent = TB_NULL;
	}
}
tb_xml_node_t* tb_xml_node_childs_head(tb_xml_node_t* node)
{
	if (node && node->childs) return node->childs->base.next;
	return TB_NULL;
}
tb_xml_node_t* tb_xml_node_childs_tail(tb_xml_node_t* node)
{
	if (node && node->childs) return node->childs->base.prev;
	return TB_NULL;
}

void tb_xml_node_attributes_clear(tb_xml_node_t* node)
{
	if (node)
	{
		if (node->attributes) tb_xml_nlist_destroy(node->attributes);
		node->attributes = TB_NULL;
	}
}
tb_xml_node_t* tb_xml_node_attributes_add_string(tb_xml_node_t* node, tb_char_t const* name, tb_string_t const* value)
{
	TB_ASSERT(node && name && value);
	if (!node || !name || !value) return TB_NULL;

	// create attribute
	tb_xml_node_t* attribute = (tb_xml_node_t*)tb_xml_document_create_attribute(node->document, name);
	if (!attribute) return TB_NULL;

	// init attribute
	tb_string_assign(&attribute->value, value);

	// create attributes
	if (!node->attributes) node->attributes = tb_xml_nlist_create();
	if (!node->attributes) goto fail;

	// add attribute
	tb_xml_nlist_add(node->attributes, attribute);

	return attribute;
fail:
	if (attribute) tb_xml_node_destroy(attribute);
	return TB_NULL;
}
tb_xml_node_t* tb_xml_node_attributes_add_c_string(tb_xml_node_t* node, tb_char_t const* name, tb_char_t const* value)
{
	TB_ASSERT(node && name && value);
	if (!node || !name || !value) return TB_NULL;

	// create attribute
	tb_xml_node_t* attribute = (tb_xml_node_t*)tb_xml_document_create_attribute(node->document, name);
	if (!attribute) return TB_NULL;

	// init attribute
	tb_string_assign_c_string(&attribute->value, value);

	// create attributes
	if (!node->attributes) node->attributes = tb_xml_nlist_create();
	if (!node->attributes) goto fail;

	// add attribute
	tb_xml_nlist_add(node->attributes, attribute);

	return attribute;
fail:
	if (attribute) tb_xml_node_destroy(attribute);
	return TB_NULL;
}
tb_xml_node_t* tb_xml_node_attributes_add_int(tb_xml_node_t* node, tb_char_t const* name, tb_int_t value)
{
	TB_ASSERT(node && name);
	if (!node || !name) return TB_NULL;

	// create attribute
	tb_xml_node_t* attribute = (tb_xml_node_t*)tb_xml_document_create_attribute(node->document, name);
	if (!attribute) return TB_NULL;

	// init attribute
	tb_string_assign_format(&attribute->value, "%d", value);

	// create attributes
	if (!node->attributes) node->attributes = tb_xml_nlist_create();
	if (!node->attributes) goto fail;

	// add attribute
	tb_xml_nlist_add(node->attributes, attribute);

	return attribute;
fail:
	if (attribute) tb_xml_node_destroy(attribute);
	return TB_NULL;
}
tb_xml_node_t* tb_xml_node_attributes_add_float(tb_xml_node_t* node, tb_char_t const* name, tb_float_t value)
{
	TB_ASSERT(node && name);
	if (!node || !name) return TB_NULL;

	// create attribute
	tb_xml_node_t* attribute = (tb_xml_node_t*)tb_xml_document_create_attribute(node->document, name);
	if (!attribute) return TB_NULL;

	// init attribute
	tb_string_assign_format(&attribute->value, "%g", value);

	// create attributes
	if (!node->attributes) node->attributes = tb_xml_nlist_create();
	if (!node->attributes) goto fail;

	// add attribute
	tb_xml_nlist_add(node->attributes, attribute);

	return attribute;
fail:
	if (attribute) tb_xml_node_destroy(attribute);
	return TB_NULL;
}
tb_xml_node_t* tb_xml_node_attributes_add_bool(tb_xml_node_t* node, tb_char_t const* name, tb_bool_t value)
{
	TB_ASSERT(node && name);
	if (!node || !name) return TB_NULL;

	// create attribute
	tb_xml_node_t* attribute = tb_xml_document_create_attribute(node->document, name);
	if (!attribute) return TB_NULL;

	// init attribute
	tb_string_assign_c_string(&attribute->value, value == TB_TRUE? "true" : "false");

	// create attributes
	if (!node->attributes) node->attributes = tb_xml_nlist_create();
	if (!node->attributes) goto fail;

	// add attribute
	tb_xml_nlist_add(node->attributes, attribute);

	return attribute;
fail:
	if (attribute) tb_xml_node_destroy(attribute);
	return TB_NULL;
}
tb_xml_node_t* tb_xml_node_attributes_add_format(tb_xml_node_t* node, tb_char_t const* name, tb_char_t const* fmt, ...)
{
	TB_ASSERT(node && name && fmt);
	if (!node || !name || !fmt) return TB_NULL;

	// format text
	tb_char_t text[4096];
	tb_size_t size = 0;
	va_list argp;
	va_start(argp, fmt);
	size = vsnprintf(text, 4096 - 1, fmt, argp);
	va_end(argp);
	if (!size) return TB_NULL;

	// create attribute
	tb_xml_node_t* attribute = tb_xml_document_create_attribute(node->document, name);
	if (!attribute) return TB_NULL;

	// init attribute
	tb_string_assign_c_string(&attribute->value, text);

	// create attributes
	if (!node->attributes) node->attributes = tb_xml_nlist_create();
	if (!node->attributes) goto fail;

	// add attribute
	tb_xml_nlist_add(node->attributes, attribute);

	return attribute;
fail:
	if (attribute) tb_xml_node_destroy(attribute);
	return TB_NULL;
}

tb_xml_node_t* tb_xml_node_add_element(tb_xml_node_t* node, tb_char_t const* name)
{
	TB_ASSERT(node && name);
	if (!node || !name) return TB_NULL;

	// create element
	tb_xml_node_t* element = tb_xml_document_create_element(node->document, name);
	if (!element) return TB_NULL;

	// add element
	tb_xml_node_childs_append(node, element);

	return element;
}
tb_xml_node_t* tb_xml_node_add_text(tb_xml_node_t* node, tb_char_t const* data)
{
	TB_ASSERT(node && data);
	if (!node || !data) return TB_NULL;

	// create text
	tb_xml_node_t* text = tb_xml_document_create_text(node->document, data);
	if (!text) return TB_NULL;

	// add text
	tb_xml_node_childs_append(node, text);

	return text;
}
tb_xml_node_t* tb_xml_node_add_cdata(tb_xml_node_t* node, tb_char_t const* data)
{
	TB_ASSERT(node && data);
	if (!node || !data) return TB_NULL;

	// create cdata
	tb_xml_node_t* cdata = tb_xml_document_create_cdata(node->document, data);
	if (!cdata) return TB_NULL;

	// add cdata
	tb_xml_node_childs_append(node, cdata);

	return cdata;
}
tb_xml_node_t* tb_xml_node_add_comment(tb_xml_node_t* node, tb_char_t const* data)
{
	TB_ASSERT(node && data);
	if (!node || !data) return TB_NULL;

	// create comment
	tb_xml_node_t* comment = tb_xml_document_create_comment(node->document, data);
	if (!comment) return TB_NULL;

	// add comment
	tb_xml_node_childs_append(node, comment);

	return comment;
}
tb_xml_node_t* tb_xml_node_add_attribute(tb_xml_node_t* node, tb_char_t const* name)
{
	TB_ASSERT(node && name);
	if (!node || !name) return TB_NULL;

	// create attribute
	tb_xml_node_t* attribute = tb_xml_document_create_attribute(node->document, name);
	if (!attribute) return TB_NULL;

	// add attribute
	tb_xml_node_childs_append(node, attribute);

	return attribute;
}

tb_xml_node_t* tb_xml_node_childs_select(tb_xml_node_t* node, tb_char_t const* path)
{
	TB_ASSERT(node && node->childs && path);
	if (!node || !node->childs || !path) return TB_NULL;

	return tb_xml_node_childs_select_node(node, TB_NULL, path);
}