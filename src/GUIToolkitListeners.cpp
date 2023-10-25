#include "GUIToolkitListeners.h"

#include <cstring>
#include <iostream>
#include <linux/input-event-codes.h>

#include "GUIToolkit.h"
#include "Window.h"

void GUIToolkitListeners::registerGlobal(void* data, wl_registry* reg, uint32_t name, const char* intf, uint32_t v)
{
	auto toolkit = (GUIToolkit*)data;

	if (!strcmp(intf, wl_compositor_interface.name))
	{
		toolkit->compositor = (wl_compositor*)wl_registry_bind(reg, name, &wl_compositor_interface, 5);
	}
	else if (!strcmp(intf, wl_shm_interface.name))
	{
		toolkit->sharedMemory = (wl_shm*)wl_registry_bind(reg, name, &wl_shm_interface, 1);
	}
	else if (!strcmp(intf, xdg_wm_base_interface.name))
	{
		toolkit->sharedBase = (xdg_wm_base*)wl_registry_bind(reg, name, &xdg_wm_base_interface, 1);
		xdg_wm_base_add_listener(toolkit->sharedBase, &toolkit->listeners.sharedBaseListener, toolkit);
	}
	else if (!strcmp(intf, wl_seat_interface.name))
	{
		toolkit->seat = (wl_seat*)wl_registry_bind(reg, name, &wl_seat_interface, 1);
		wl_seat_add_listener(toolkit->seat, &toolkit->listeners.seatListener, toolkit);
	}
}
void GUIToolkitListeners::registerGlobalRemove(void* data, wl_registry* reg, uint32_t name) {}

void GUIToolkitListeners::seatCap(void* data, wl_seat* seat, uint32_t cap)
{
	auto toolkit = (GUIToolkit*)data;
	if (cap & WL_SEAT_CAPABILITY_KEYBOARD && !toolkit->keyboard)
	{
		toolkit->keyboard = wl_seat_get_keyboard(seat);
		wl_keyboard_add_listener(toolkit->keyboard, &toolkit->listeners.keyboardListener, toolkit);
	}
	if (cap & WL_SEAT_CAPABILITY_POINTER)
	{
		toolkit->pointer = wl_seat_get_pointer(seat);
		wl_pointer_add_listener(toolkit->pointer, &toolkit->listeners.pointerListener, toolkit);
	}
}
void GUIToolkitListeners::seatName(void* data, wl_seat* seat, const char* name) {}

void GUIToolkitListeners::keyboardKey(void* data, wl_keyboard* kb, uint32_t ser, uint32_t t, uint32_t key, uint32_t stat)
{
	auto toolkit = (GUIToolkit*)data;
	if (key == 1) toolkit->closeTrigger = true;
	else if (key == 30)
	{
		GUIToolkit::windows[0]->setPos(500, 500);
		std::cout << "a\n";
	}
	else if (key == 32)
	{
		std::cout << "d\n";
	}
}
void GUIToolkitListeners::pointerButton(void* data, wl_pointer* pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
{
	auto toolkit = (GUIToolkit*)data;
	if (button == BTN_LEFT && state == WL_POINTER_BUTTON_STATE_PRESSED && GUIToolkit::activeWindow != nullptr)
	{
		xdg_toplevel_move(GUIToolkit::activeWindow->top, toolkit->seat, serial);
	}
}
void GUIToolkitListeners::pointerEnter(void* data, wl_pointer* pointer, uint32_t serial, wl_surface* surface, wl_fixed_t x, wl_fixed_t yInt32)
{
	auto window = (Window*)wl_surface_get_user_data(surface);
	GUIToolkit::activeWindow = window;
}
void GUIToolkitListeners::pointerExit(void* data, wl_pointer* pointer, uint32_t serial, wl_surface* surface)
{
	GUIToolkit::activeWindow = nullptr;
}