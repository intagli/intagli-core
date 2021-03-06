#ifndef _intagli_internal_h_
#define _intagli_internal_h_

#include <wayland-client.h>
#include "intagli/backends/wayland/xdg-shell.h"

void _iglInitCairo();

void _iglCallbackXdgToplevelConfigure(void *data, struct xdg_toplevel *xdgToplevel, int32_t width, int32_t height, struct wl_array *states);

void _iglCallbackXdgToplevelClose(void *data, struct xdg_toplevel *xdgToplevel);

void _iglCallbackXdgSurfaceConfigure(void *data, struct xdg_surface *surface, uint32_t name);

void _iglCallbackXdgWmBasePing(void *data, struct xdg_wm_base *shell, uint32_t name);

void _iglCallbackWaylandRegistryAddObject(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version);

void _iglCallbackWaylandRegistryRemoveObject(void *data, struct wl_registry *registry, uint32_t name);

#endif
