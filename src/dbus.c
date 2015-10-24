/*
 * system-dbus
 *
 * Copyright (c) 2012 - 2013 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include "common.h"
#include "dbus.h"

#define DBUS_REPLY_TIMEOUT	(-1)

/** extract from dbus/dbus-protocol.h
 * (GDbus use the same maximum value.)
 * Max length in bytes of a bus name, interface, or member (not object
 * path, paths are unlimited). This is limited because lots of stuff
 * is O(n) in this number, plus it would be obnoxious to type in a
 * paragraph-long method name so most likely something like that would
 * be an exploit.
 */
#define DBUS_MAXIMUM_NAME_LENGTH 255

struct pending_call_data {
	dbus_pending_cb func;
	void *data;
};

struct proxy_node {
	GDBusProxy *proxy;
	char *dest;
	char *path;
	char *interface;
	bool deleted;
};

static GList *proxy_pool;

static int g_dbus_error_to_errno(int code)
{
	/**
	 * if device is not supported,
	 * deviced does not register the method call of the device.
	 * in this case, dbus will return UNKNOWN_METHOD error.
	 */
	/* refer to gio/gioenums.h */
	if (code == G_DBUS_ERROR_ACCESS_DENIED)
		return -EACCES;
	else if (code == G_DBUS_ERROR_UNKNOWN_METHOD)
		return -ENOTSUP;
	return -ECOMM;
}

static GVariant *append_g_variant(const char *sig, char *param[])
{
	GVariantBuilder builder;
	char *ch;
	int i;

	if (!sig || !param)
		return NULL;

	g_variant_builder_init(&builder, G_VARIANT_TYPE_TUPLE);

	for (ch = (char*)sig, i = 0; *ch != '\0'; ++i, ++ch) {
		switch (*ch) {
		case 'i':
			g_variant_builder_add(&builder, "i", atoi(param[i]));
			break;
		case 'u':
			g_variant_builder_add(&builder, "u", strtoul(param[i], NULL, 10));
			break;
		case 't':
			g_variant_builder_add(&builder, "t", atoll(param[i]));
			break;
		case 's':
			g_variant_builder_add(&builder, "s", param[i]);
			break;
		default:
			return NULL;
		}
	}

	return g_variant_builder_end(&builder);
}

static struct proxy_node *find_matched_proxy_node(const char *dest,
		const char *path,
		const char *interface)
{
	GList *elem;
	struct proxy_node *node;
	int plen;

	if (!dest || !path || !interface)
		return NULL;

	plen = strlen(path) + 1;

	/* find matched proxy object */
	for (elem = proxy_pool; elem; elem = elem->next) {
		node = elem->data;
		if (!node || node->deleted)
			continue;
		if (!strncmp(node->dest, dest, DBUS_MAXIMUM_NAME_LENGTH) &&
		    !strncmp(node->path, path, plen) &&
		    !strncmp(node->interface, interface,
			    DBUS_MAXIMUM_NAME_LENGTH))
			return node;
	}

	return NULL;
}

static void remove_unused_proxy_node(void)
{
	GList *elem;
	GList *next;
	struct proxy_node *node;

	for (elem = proxy_pool, next = g_list_next(elem); elem;
			elem = next, next = g_list_next(elem)) {
		node = elem->data;
		if (!node)
			continue;
		if (node->deleted) {
			proxy_pool = g_list_delete_link(proxy_pool, elem);
			g_object_unref(node->proxy);
			free(node->dest);
			free(node->path);
			free(node->interface);
			free(node);
		}
	}
}

static gboolean release_proxy_node(gpointer data)
{
	remove_unused_proxy_node();
	return false;
}

static void on_name_owner_notify(GObject *object,
		GParamSpec *pspec,
		gpointer user_data)
{
	struct proxy_node *node = user_data;

	if (!node)
		return;

	node->deleted = true;

	g_idle_add(release_proxy_node, NULL);
}

static GDBusProxy *get_proxy_from_proxy_pool(const char *dest,
		const char *path,
		const char *interface,
		GError **err)
{
	GDBusConnection *conn;
	GDBusProxy *proxy;
	struct proxy_node *node;

	if (!dest || !path || !interface) {
		if (err)
			g_set_error(err, G_IO_ERROR,
					G_IO_ERROR_INVALID_ARGUMENT,
					"Cannot determine destination address");
		return NULL;
	}

	/* find matched proxy node in proxy pool */
	node = find_matched_proxy_node(dest, path, interface);
	if (node)
		return node->proxy;

	conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, err);
	if (!conn)
		return NULL;

	proxy = g_dbus_proxy_new_sync(conn,
			G_DBUS_PROXY_FLAGS_NONE,
			NULL,      /* GDBusinterfaceinfo */
			dest,      /* bus name */
			path,      /* object path */
			interface, /* interface name */
			NULL,      /* GCancellable */
			err);
	if (!proxy)
		return NULL;

	node = malloc(sizeof(struct proxy_node));
	if (!node) {
		g_object_unref(proxy);
		if (err)
			g_set_error(err, G_IO_ERROR,
					G_IO_ERROR_FAILED,
					"Cannot allocate proxy_node memory");
		return NULL;
	}

	node->proxy = proxy;
	node->dest = strdup(dest);
	node->path = strdup(path);
	node->interface = strdup(interface);
	node->deleted = false;

	/* interest notify::g-name-owner of proxy */
	g_signal_connect(proxy, "notify::g-name-owner",
			G_CALLBACK(on_name_owner_notify), node);

	proxy_pool = g_list_append(proxy_pool, node);

	return proxy;
}

int dbus_method_sync(const char *dest, const char *path,
		const char *interface, const char *method,
		const char *sig, char *param[])
{
	GDBusProxy *proxy;
	GError *err = NULL;
	GVariant *output;
	int result;

#if !GLIB_CHECK_VERSION(2,35,0)
	g_type_init();
#endif

	proxy = get_proxy_from_proxy_pool(dest, path, interface, &err);
	if (!proxy) {
		_E("fail to get proxy from proxy pool : %s-%s (%d-%s)",
				interface, method, err->code, err->message);
		result = g_dbus_error_to_errno(err->code);
		g_clear_error(&err);
		return result;
	}

	output = g_dbus_proxy_call_sync(proxy,
			method,                       /* method name */
			append_g_variant(sig, param), /* parameters */
			G_DBUS_CALL_FLAGS_NONE,
			DBUS_REPLY_TIMEOUT,           /* timeout */
			NULL,                         /* GCancellable */
			&err);
	if (!output) {
		if (err) {
			_E("g_dbus_proxy_call_sync error : %s-%s (%d-%s)",
					interface, method, err->code, err->message);
			result = g_dbus_error_to_errno(err->code);
			g_clear_error(&err);
		} else {
			_E("g_dbus_proxy_call_sync error : %s-%s",
					interface, method);
			result = -ECOMM;
		}
		g_object_unref(proxy);
		return result;
	}

	/* get output value */
	g_variant_get(output, "(i)", &result);

	g_variant_unref(output);

	return result;
}

static void cb_pending(GDBusProxy *proxy,
		GAsyncResult *res,
		gpointer user_data)
{
	struct pending_call_data *data = user_data;
	GError *err = NULL;
	GVariant *output;

	output = g_dbus_proxy_call_finish(proxy,
			res, /* GAsyncResult */
			&err);
	if (!output)
		_E("g_dbus_proxy_call_finish error : %d-%s",
				err->code, err->message);

	if (data && data->func)
		data->func(data->data, output, err);

	if (err)
		g_clear_error(&err);
	if (output)
		g_variant_unref(output);
	free(data);
}

int dbus_method_async_with_reply(const char *dest, const char *path,
		const char *interface, const char *method,
		const char *sig, char *param[],
		dbus_pending_cb cb, int timeout, void *data)
{
	GDBusProxy *proxy;
	GError *err = NULL;
	struct pending_call_data *pdata;
	int result;

#if !GLIB_CHECK_VERSION(2,35,0)
	g_type_init();
#endif

	proxy = get_proxy_from_proxy_pool(dest, path, interface, &err);
	if (!proxy) {
		_E("fail to get proxy from proxy pool : %s-%s (%d-%s)",
				interface, method, err->code, err->message);
		result = g_dbus_error_to_errno(err->code);
		g_clear_error(&err);
		return result;
	}

	pdata = malloc(sizeof(struct pending_call_data));
	if (!pdata) {
		_E("malloc error : %s-%s",
				interface, method);
		return -ENOMEM;
	}

	pdata->func = cb;
	pdata->data = data;

	g_dbus_proxy_call(proxy,
			method,                          /* method name */
			append_g_variant(sig, param),    /* parameters */
			G_DBUS_CALL_FLAGS_NONE,
			DBUS_REPLY_TIMEOUT,              /* timeout */
			NULL,                            /* GCancellable */
			(GAsyncReadyCallback)cb_pending, /* GAsyncReadyCallback */
			pdata);                          /* user data */

	return 0;
}
