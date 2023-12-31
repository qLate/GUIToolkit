#pragma once
#include <string>

#include "Component.h"
#include "xdg-shell-client-protocol.h"


class Button;

class WindoW : public Component
{
	inline static constexpr int resizeBorder = 10;
	inline static constexpr int minSize = 100;
	inline static constexpr int headerHeight = 30;

	xdg_surface_listener xSurfListener {
		.configure = configureXSurf
	};
	xdg_toplevel_listener topListener {
		.configure = configureTop,
		.close = closeTop
	};

	wl_callback_listener wCallbackListener {
		.done = frameNew
	};

	xdg_toplevel* top = nullptr;
	xdg_surface* xSurf = nullptr;
	wl_subsurface* subsurf = nullptr;

	glm::vec2 wSize {};
	wl_surface* wSurf = nullptr;
	wl_callback* wSurfCallback = nullptr;
	wl_buffer* wBuf = nullptr;
	uint8_t* wPixels = nullptr;
	wl_shm_pool* wPixels_pool = nullptr;
	int wPixelsCapacity = 0;

	bool isFullscreen = false;

public:
	bool isResizeable = true;
	bool isMaximized;

	SubComponent* header;
	Button* buttonMinimize;
	Button* buttonMaximize;
	Button* buttonClose;

	WindoW(const std::string& name, glm::vec2 size = {500, 500});
	void initSurfaces(const std::string& name);
	void createHeader(const std::string& name);

	~WindoW() override;

	void resize(glm::vec2 size) final;

	void switchFullscreen();

private:
	static void frameNew(void* data, wl_callback* cb, uint32_t a);
	static void configureXSurf(void* data, xdg_surface* xSurf, uint32_t serial);
	static void configureTop(void* data, xdg_toplevel* xSurf, int32_t w_, int32_t h_, wl_array* stat);
	static void closeTop(void* data, xdg_toplevel* top);

	friend class Component;
	friend class GUI;
	friend class GUIListeners;
};
