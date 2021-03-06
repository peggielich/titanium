/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#include "../ui_module.h"
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>
#include <gdk/gdkx.h>
#include <Poco/Thread.h>

namespace ti
{
	GtkUIBinding::GtkUIBinding(Host *host) : UIBinding(host)
	{
		/* Prepare the custom curl URL handler */
		curl_register_local_handler(&Titanium_app_url_handler);

		/* Register the script evaluator */
		evaluator = new ScriptEvaluator();
		addScriptEvaluator(evaluator);

		double ver = host->GetGlobalObject()->Get("version")->ToDouble();
		char buf[256];
		snprintf(buf, 256, "%s/%0.2f", PRODUCT_NAME, ver);
		g_set_prgname(buf);
	}

	SharedPtr<MenuItem> GtkUIBinding::CreateMenu(bool trayMenu)
	{
		SharedPtr<MenuItem> menu = new GtkMenuItemImpl();
		return menu;
	}

	void GtkUIBinding::SetMenu(SharedPtr<MenuItem> new_menu)
	{
		// Notify all windows that the app menu has changed.
		std::vector<UserWindow*>& windows = UserWindow::GetWindows();
		std::vector<UserWindow*>::iterator i = windows.begin();
		while (i != windows.end())
		{
			GtkUserWindow* guw = dynamic_cast<GtkUserWindow*>(*i);
			if (guw != NULL)
				guw->AppMenuChanged();

			i++;
		}
	}

	void GtkUIBinding::SetContextMenu(SharedPtr<MenuItem> new_menu)
	{
	}

	void GtkUIBinding::SetIcon(SharedString icon_path)
	{

		// Notify all windows that the app icon has changed.
		std::vector<UserWindow*>& windows = UserWindow::GetWindows();
		std::vector<UserWindow*>::iterator i = windows.begin();
		while (i != windows.end())
		{
			GtkUserWindow* guw = dynamic_cast<GtkUserWindow*>(*i);
			if (guw != NULL)
				guw->AppIconChanged();

			i++;
		}
	}

	SharedPtr<TrayItem> GtkUIBinding::AddTray(
		SharedString icon_path,
		SharedBoundMethod cb)
	{
		SharedPtr<TrayItem> item = new GtkTrayItem(icon_path, cb);
		return item;
	}

	long GtkUIBinding::GetIdleTime()
	{
		Display *display = gdk_x11_get_default_xdisplay();
		if (display == NULL)
			return -1;
		int screen = gdk_x11_get_default_screen();

		XScreenSaverInfo *mit_info = XScreenSaverAllocInfo();
		XScreenSaverQueryInfo(display, RootWindow(display, screen), mit_info);
		long idle_time = mit_info->idle;
		XFree(mit_info);

		return idle_time;
	}

}
