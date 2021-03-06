/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2008 Appcelerator, Inc. All Rights Reserved.
 */

#ifndef SNARL_WIN32_H_
#define SNARL_WIN32_H_

#include <kroll/kroll.h>
#include <string>
#include "../growl_binding.h"

namespace ti {
	class SnarlWin32 : public GrowlBinding {
	public:
		SnarlWin32(SharedBoundObject global);
		virtual ~SnarlWin32();

		virtual bool IsRunning();
		virtual void ShowNotification(std::string& title, std::string& description, std::string& iconURL, int notification_delay, SharedBoundMethod callback);
	};
}

#endif /* SNARL_WIN32_H_ */
