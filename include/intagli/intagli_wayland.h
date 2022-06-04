#ifndef _intagli_intagli_wayland_h_
#define _intagli_intagli_wayland_h_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <wayland-egl-core.h>
#include <EGL/egl.h>
#include <GL/gl.h>
#include <GLES2/gl2.h>
#include <cairo/cairo.h>
#include <cairo/cairo-gl.h>

#include "intagli/backends/wayland/xdg-shell.h"
#include "intagli/intagli_types.h"

struct iglWaylandWindow
{
	//Wayland
	struct wl_display *wlDisplay;
	struct wl_surface *wlSurface;
	struct wl_egl_window *wlEGLWindow;
	struct wl_compositor *wlCompositor;
	struct wl_region *wlRegion;
	struct xdg_wm_base *xdgWmBase;
	struct xdg_wm_base_listener XdgWmBaseListener;
	struct xdg_surface *xdgSurface;
	struct wl_registry *wlRegistry;
	struct wl_registry_listener wlRegistryListener;	
	struct xdg_surface_listener xdgSurfaceListener;	
	struct xdg_toplevel *xdgToplevel;
	struct xdg_toplevel_listener xdgToplevelListener;
	bool xdgSurfaceConfigureFlag;

	//EGL
	EGLDisplay eglDisplay;
	EGLContext eglContext;
	EGLSurface eglSurface;
	
	//Cairo
	struct
	{
		cairo_t *cairo;
		cairo_surface_t *cairoSurface;
		cairo_device_t *cairoDevice;
	} cairo;

	//Window
	struct
	{
		bool shouldClose;
		bool isMaximized;
		bool isFullscreen;
		bool waitConfigure;
	} state;

	char* title;

	vec2 size;
};

void iglWaylandInitWindow(struct iglWaylandWindow* window);

void iglWaylandRender(struct iglWaylandWindow* window);

void iglWaylandDestroyWindow(struct iglWaylandWindow* window);

#endif
