/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */
#include "ui_module.h"

#define GET_ARG_OR_RETURN(INDEX, TYPE, VAR) \
	if ((int) args.size() < INDEX - 1 || !args.at(INDEX)->Is##TYPE()) \
		return; \
	VAR = args.at(INDEX)->To##TYPE();

#define GET_ARG(INDEX, TYPE, VAR) \
	if ((int) args.size() > INDEX && args.at(INDEX)->Is##TYPE()) \
		VAR = args.at(INDEX)->To##TYPE();

namespace ti
{
	UIBinding::UIBinding(Host *host) : host(host)
	{
		this->SetMethod("createMenu", &UIBinding::_CreateMenu);
		this->SetMethod("createTrayMenu", &UIBinding::_CreateTrayMenu);
		this->SetMethod("setMenu", &UIBinding::_SetMenu);
		this->SetMethod("getMenu", &UIBinding::_GetMenu);
		this->SetMethod("setContextMenu", &UIBinding::_SetContextMenu);
		this->SetMethod("getContextMenu", &UIBinding::_GetContextMenu);
		this->SetMethod("setIcon", &UIBinding::_SetIcon);
		this->SetMethod("addTray", &UIBinding::_AddTray);
		this->SetMethod("clearTray", &UIBinding::_ClearTray);

		this->SetMethod("setDockIcon", &UIBinding::_SetDockIcon);
		this->SetMethod("setDockMenu", &UIBinding::_SetDockMenu);
		this->SetMethod("setBadge", &UIBinding::_SetBadge);
		this->SetMethod("setBadgeImage", &UIBinding::_SetBadgeImage);

		this->SetMethod("getIdleTime", &UIBinding::_GetIdleTime);
	}

	UIBinding::~UIBinding()
	{
	}

	void UIBinding::_CreateMenu(const ValueList& args, SharedValue result)
	{
		SharedPtr<MenuItem> menu = this->CreateMenu(false);
		result->SetList(menu);
	}

	void UIBinding::_CreateTrayMenu(const ValueList& args, SharedValue result)
	{
		SharedPtr<MenuItem> menu = this->CreateMenu(true);
		result->SetList(menu);
	}

	void UIBinding::_SetMenu(const ValueList& args, SharedValue result)
	{
		SharedPtr<MenuItem> menu = NULL; // A NULL value is an unset
		if (args.size() > 0 && args.at(0)->IsList())
		{
			menu = args.at(0)->ToList().cast<MenuItem>();
		}
		UIModule::SetMenu(menu);
		this->SetMenu(menu);
	}

	void UIBinding::_GetMenu(const ValueList& args, SharedValue result)
	{
		SharedPtr<MenuItem> menu = UIModule::GetMenu();
		if (menu.get() != NULL)
		{
			SharedBoundList list = menu;
			result->SetList(list);
		}
		else
		{
			result->SetUndefined();
		}
	}

	void UIBinding::_SetContextMenu(const ValueList& args, SharedValue result)
	{
		SharedPtr<MenuItem> menu = NULL; // A NULL value is an unset
		if (args.size() > 0 && args.at(0)->IsList())
		{
			menu = args.at(0)->ToList().cast<MenuItem>();
		}
		UIModule::SetContextMenu(menu);
		this->SetContextMenu(menu);
	}

	void UIBinding::_GetContextMenu(const ValueList& args, SharedValue result)
	{
		SharedPtr<MenuItem> menu = UIModule::GetContextMenu();
		if (menu.get() != NULL)
		{
			SharedBoundList list = menu;
			result->SetList(list);
		}
		else
		{
			result->SetUndefined();
		}
	}

	void UIBinding::_SetIcon(const ValueList& args, SharedValue result)
	{
		SharedString icon_path = NULL; // a NULL value is an unset
		if (args.size() > 0 && args.at(0)->IsString())
		{
			const char *icon_url = args.at(0)->ToString();
			icon_path = UIModule::GetResourcePath(icon_url);
		}
		UIModule::SetIcon(icon_path);
		this->SetIcon(icon_path);
	}

	void UIBinding::_AddTray(const ValueList& args, SharedValue result)
	{
		const char *icon_url;
		GET_ARG_OR_RETURN(0, String, icon_url);
		SharedString icon_path = UIModule::GetResourcePath(icon_url);
		if (icon_path.isNull())
			return;

		SharedBoundMethod cb = SharedBoundMethod(NULL);
		GET_ARG(1, Method, cb);

		SharedPtr<TrayItem> item = this->AddTray(icon_path, cb);

		UIModule::AddTrayItem(item);
		result->SetObject(item);
	}

	void UIBinding::_ClearTray(const ValueList& args, SharedValue result)
	{
		UIModule::ClearTrayItems();
	}


	void UIBinding::_SetDockIcon(const ValueList& args, SharedValue result)
	{
		SharedString icon_path = NULL; // a NULL value is an unset
		if (args.size() > 0 && args.at(0)->IsString())
		{
			const char *icon_url = args.at(0)->ToString();
			icon_path = UIModule::GetResourcePath(icon_url);
		}
		this->SetDockIcon(icon_path);
	}

	void UIBinding::_SetDockMenu(const ValueList& args, SharedValue result)
	{
		SharedPtr<MenuItem> menu = NULL; // A NULL value is an unset
		if (args.size() > 0 && args.at(0)->IsList())
		{
			menu = args.at(0)->ToList().cast<MenuItem>();
		}
		this->SetDockMenu(menu);
	}

	void UIBinding::_SetBadge(const ValueList& args, SharedValue result)
	{
		// badges are just labels right now
		// we might want to support custom images too
		SharedString badge_path = NULL; // a NULL value is an unset
		if (args.size() > 0 && args.at(0)->IsString())
		{
			const char *badge_url = args.at(0)->ToString();
			if (badge_url!=NULL)
			{
				badge_path = SharedString(new std::string(badge_url));
			}
		}
		this->SetBadge(badge_path);
	}

	void UIBinding::_SetBadgeImage(const ValueList& args, SharedValue result)
	{
		SharedString image_path = NULL; // a NULL value is an unset
		if (args.size() > 0 && args.at(0)->IsString())
		{
			const char *image_url = args.at(0)->ToString();
			if (image_url!=NULL)
			{
				image_path = UIModule::GetResourcePath(image_url);
			}
		}

		this->SetBadgeImage(image_path);
	}

	void UIBinding::_GetIdleTime(
		const ValueList& args,
		SharedValue result)
	{
		result->SetDouble(this->GetIdleTime());
	}

}

