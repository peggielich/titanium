/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#ifndef _PIPE_H_
#define _PIPE_H_

#include <api/module.h>
#include <api/binding/binding.h>
#include <Poco/Process.h>
#include <Poco/PipeStream.h>
#include <Poco/Exception.h>

namespace ti
{
	class Pipe : public StaticBoundObject
	{
	public:
		Pipe(Poco::PipeIOS *pipe);
		virtual ~Pipe();
	private:
		Poco::PipeIOS *pipe;
		bool closed;

		void Write(const ValueList& args, SharedValue result);
		void Read(const ValueList& args, SharedValue result);
		void Close(const ValueList& args, SharedValue result);
		void Close();
	};
}

#endif
