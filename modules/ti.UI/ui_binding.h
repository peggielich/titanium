/**
 g* Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#ifndef _UI_BINDING_H_
#define _UI_BINDING_H_

#include <api/module.h>
#include <api/binding/binding.h>
#include "menu_item.h"

namespace ti
{
	class UIBinding : public StaticBoundObject
	{
		friend class SharedPtr<BoundObject>;

	public:
		UIBinding(Host *host);
		virtual ~UIBinding();

	protected:
		Host* host;

	private:
		void _CreateMenu(const ValueList& args, SharedValue result);
		void _CreateTrayMenu(const ValueList& args, SharedValue result);
		void _SetMenu(const ValueList& args, SharedValue result);
		void _GetMenu(const ValueList& args, SharedValue result);
		void _SetContextMenu(const ValueList& args, SharedValue result);
		void _GetContextMenu(const ValueList& args, SharedValue result);
		void _SetIcon(const ValueList& args, SharedValue result);
		void _AddTray(const ValueList& args, SharedValue result);
		void _ClearTray(const ValueList& args, SharedValue result);

		virtual SharedPtr<MenuItem> CreateMenu(bool trayMenu) = 0;
		virtual void SetMenu(SharedPtr<MenuItem>) = 0;
		virtual void SetContextMenu(SharedPtr<MenuItem>) = 0;
		virtual void SetIcon(SharedString icon_path) = 0;
		virtual SharedPtr<TrayItem> AddTray(SharedString icon_path,
		                                    SharedBoundMethod cb) = 0;

		void _GetIdleTime(const ValueList& args, SharedValue result);

		/* OS X specific callbacks */
		void _SetDockIcon(const ValueList& args, SharedValue result);
		void _SetDockMenu(const ValueList& args, SharedValue result);
		void _SetBadge(const ValueList& args, SharedValue result);
		void _SetBadgeImage(const ValueList& args, SharedValue result);

		/* These have empty impls, because are OS X-only for now */
		virtual void SetDockIcon(SharedString icon_path) {}
		virtual void SetDockMenu(SharedPtr<MenuItem>) {}
		virtual void SetBadge(SharedString badge_label) {}
		virtual void SetBadgeImage(SharedString badge_path) {}

		virtual long GetIdleTime() = 0;

	};
}

#endif
