#include "intagli/intagli_wayland.h"

void _iglInternal_XdgToplevelConfigure(void *data, struct xdg_toplevel *xdgToplevel, int32_t width, int32_t height, struct wl_array *states)
{
	struct iglWaylandWindow *w = data;

	w->state.isFullscreen = false;
	w->state.isMaximized = false;

	uint32_t *state;
	wl_array_for_each(state, states)
	{
		switch (*state) {
		case XDG_TOPLEVEL_STATE_FULLSCREEN:
			w->state.isFullscreen = true;
			break;
		case XDG_TOPLEVEL_STATE_MAXIMIZED:
			w->state.isMaximized = true;
			break;
		}
	}

	if (width > 0 && height > 0) {
		w->size.x = width;
		w->size.y = height;
	}
	else if (!w->state.isFullscreen && !w->state.isMaximized)
	{

	}

	if (w->wlEGLWindow)
		wl_egl_window_resize(w->wlEGLWindow, w->size.x, w->size.y, 0, 0);
}

void _iglInternal_XdgToplevelClose(void *data, struct xdg_toplevel *xdgToplevel)
{
	struct iglWaylandWindow *w = data;
	w->state.shouldClose = true;
}

void _iglInternal_XdgSurfaceConfigure(void *data, struct xdg_surface *surface, uint32_t name)
{
	struct iglWaylandWindow *w = data;
	xdg_surface_ack_configure(surface, name);
	w->xdgSurfaceConfigureFlag = false;
}

void _iglInternal_XdgWmBasePing(void *data, struct xdg_wm_base *shell, uint32_t name)
{
	xdg_wm_base_pong(shell, name);
}

//Wayland Callback function manages window states
void _iglInternal_CallbackWaylandRegistryAddObject(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version)
{
	struct iglWaylandWindow *w = data;
	if (strcmp(interface, "wl_compositor") == 0)
	{
		w->wlCompositor = wl_registry_bind(registry, name, &wl_compositor_interface, 4);
	}
	else if (strcmp(interface, "xdg_wm_base") == 0)
	{
		w->xdgWmBase = wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
		xdg_wm_base_add_listener(w->xdgWmBase, &w->XdgWmBaseListener, w);
	}
}

void _iglInternal_CallbackWaylandRegistryRemoveObject(void *data, struct wl_registry *registry, uint32_t name)
{
	//
}

void iglWaylandInitWindow(struct iglWaylandWindow* window)
{
	window->wlCompositor = NULL;
	window->xdgWmBase = NULL;
	
	//Registry Listener
	window->wlRegistryListener.global = _iglInternal_CallbackWaylandRegistryAddObject;
	window->wlRegistryListener.global_remove = _iglInternal_CallbackWaylandRegistryRemoveObject;
	
	//XDG-WM-BASE Listener
	window->XdgWmBaseListener.ping = _iglInternal_XdgWmBasePing;	
	
	//XDG-SURFACE Listener
	window->xdgSurfaceListener.configure = _iglInternal_XdgSurfaceConfigure;
	window->wlDisplay = wl_display_connect(NULL);
	
	//XDG-TOPLEVEL Listener	
	window->xdgToplevelListener.configure = _iglInternal_XdgToplevelConfigure;
	window->xdgToplevelListener.close = _iglInternal_XdgToplevelClose;
	
	//Registry
	window->wlRegistry = wl_display_get_registry(window->wlDisplay);
	wl_registry_add_listener(window->wlRegistry, &window->wlRegistryListener, window);

	wl_display_dispatch(window->wlDisplay);
	wl_display_roundtrip(window->wlDisplay);


	//Display
	window->eglDisplay = eglGetDisplay(window->wlDisplay);
	assert(window->eglDisplay != EGL_NO_DISPLAY);
		
	window->wlSurface = wl_compositor_create_surface(window->wlCompositor);
	
	//XDG-WM-BASE
	window->xdgSurface = xdg_wm_base_get_xdg_surface(window->xdgWmBase, window->wlSurface);
	xdg_surface_add_listener(window->xdgSurface, &window->xdgSurfaceListener, window);
	
	//egl config
	EGLConfig config;
	EGLint config_num;
	EGLint config_attributes[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 1,
		EGL_GREEN_SIZE, 1,
		EGL_BLUE_SIZE, 1,
		EGL_ALPHA_SIZE, 1,
		EGL_DEPTH_SIZE, 1,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NONE
	};
	
	static const EGLint context_attributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
    	};

	//XDG-TOPLEVEL-BASE
	window->xdgToplevel = xdg_surface_get_toplevel(window->xdgSurface);
	xdg_toplevel_add_listener(window->xdgToplevel, &window->xdgToplevelListener, window);
	xdg_toplevel_set_title(window->xdgToplevel, "aaaaaa!");
	
	//Create EGL Wayland Window
	
	window->wlEGLWindow = wl_egl_window_create(window->wlSurface, 640, 480);

	//OPAQUE REGION
	window->wlRegion = wl_compositor_create_region(window->wlCompositor);
	wl_region_add(window->wlRegion, 0, 0,
		  640,
		  480);

	wl_surface_set_opaque_region(window->wlSurface, window->wlRegion);

	//
	window->xdgSurfaceConfigureFlag = true;
	wl_surface_commit(window->wlSurface);
	
	//init EGL
	eglInitialize(window->eglDisplay, NULL, NULL);
	
	//Set as OpenGL
	eglBindAPI(EGL_OPENGL_API);	
	
	//Init window
	eglChooseConfig(window->eglDisplay, config_attributes, &config, 1, &config_num);
	window->eglContext = eglCreateContext(window->eglDisplay, config, EGL_NO_CONTEXT, context_attributes);

	eglMakeCurrent(window->eglDisplay, window->eglSurface, window->eglSurface, window->eglContext);

	window->eglSurface = eglCreateWindowSurface(window->eglDisplay, config, window->wlEGLWindow, NULL);
	
	//Init Cairo	
	window->cairo.cairoDevice = cairo_egl_device_create(window->eglDisplay, window->eglContext);
	cairo_status_t cstatus = cairo_device_status(window->cairo.cairoDevice);
	if (cstatus != CAIRO_STATUS_SUCCESS)
	{
		//TODO:ERROR HANDLING
		printf("cairo err:%s\n", cairo_status_to_string(cstatus));
		exit(-1);
	}

}

void iglWaylandRender(struct iglWaylandWindow* window)
{
	int st = 0;
	
	eglSwapBuffers(window->eglDisplay, window->eglSurface);

	window->cairo.cairoSurface = cairo_gl_surface_create_for_egl(window->cairo.cairoDevice, window->eglSurface, 640, 480);

	window->state.shouldClose = false;
	wl_display_dispatch_pending(window->wlDisplay);

	window->cairo.cairo = cairo_create(window->cairo.cairoSurface);
	
	//Cairo
	cairo_set_source_rgba(window->cairo.cairo, 1.0, 1.0, 1.0, 0.8);
	cairo_paint(window->cairo.cairo);
	
	cairo_set_source_rgb(window->cairo.cairo, 0.0, 0.0, 0.0);

	cairo_select_font_face(window->cairo.cairo, "cairo:sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	
	cairo_set_font_size(window->cairo.cairo, 20.0);
	cairo_move_to(window->cairo.cairo, 10.0, 100.0);
	cairo_show_text(window->cairo.cairo, "Hi");

	cairo_gl_surface_swapbuffers(window->cairo.cairoSurface);


	while (( !window->state.shouldClose ) && ( st != -1 ))
	{
		if (window->xdgSurfaceConfigureFlag)
		{
			st = wl_display_dispatch(window->wlDisplay);
		}
		else
		{
			st = wl_display_dispatch_pending(window->wlDisplay);
		}
	}

	//Destroy Cairo
	cairo_destroy(window->cairo.cairo);	
}

void iglWaylandDestroyWindow(struct iglWaylandWindow* window)
{
	eglDestroySurface(window->eglDisplay, window->eglSurface);
	wl_egl_window_destroy(window->wlEGLWindow);
	
	wl_surface_destroy(window->wlSurface);
	eglDestroyContext(window->eglDisplay, window->eglContext);
}
