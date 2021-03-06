/**
 * Appcelerator Titanium - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2008 Appcelerator, Inc. All Rights Reserved.
 */
#include "sound.h"

namespace ti
{
	Sound::Sound (std::string &s) : url(s)
	{
		this->SetMethod("play",&Sound::Play);
		this->SetMethod("pause",&Sound::Pause);
		this->SetMethod("stop",&Sound::Stop);
		this->SetMethod("reload",&Sound::Reload);
		this->SetMethod("setVolume",&Sound::SetVolume);
		this->SetMethod("getVolume",&Sound::GetVolume);
		this->SetMethod("setLooping",&Sound::SetLooping);
		this->SetMethod("isLooping",&Sound::IsLooping);
		this->SetMethod("isPlaying",&Sound::IsPlaying);
		this->SetMethod("isPaused",&Sound::IsPaused);
		this->SetMethod("onComplete",&Sound::OnComplete);
	}
	Sound::~Sound()
	{
	}
	void Sound::Play(const ValueList& args, SharedValue result)
	{
		this->Play();
	}
	void Sound::Pause(const ValueList& args, SharedValue result)
	{
		if (!this->IsPlaying())
		{
			throw ValueException::FromString("Sounds is not currently playing");
		}
		this->Pause();
	}
	void Sound::Stop(const ValueList& args, SharedValue result)
	{
		if (!this->IsPlaying() && !this->IsPaused())
		{
			return;
		}
		this->Stop();
	}
	void Sound::Reload(const ValueList& args, SharedValue result)
	{
		this->Reload();
	}
	void Sound::SetVolume(const ValueList& args, SharedValue result)
	{
		if (args.size()!=1)
		{
			throw ValueException::FromString("setVolume takes 1 parameter");
		}
		this->SetVolume(args.at(0)->ToDouble());
	}
	void Sound::GetVolume(const ValueList& args, SharedValue result)
	{
		result->SetDouble(this->GetVolume());
	}
	void Sound::SetLooping(const ValueList& args, SharedValue result)
	{
		if (args.size()!=1)
		{
			throw ValueException::FromString("setLooping takes 1 parameter");
		}
		this->SetLooping(args.at(0)->ToBool());
	}
	void Sound::IsLooping(const ValueList& args, SharedValue result)
	{
		result->SetBool(this->IsLooping());
	}
	void Sound::IsPlaying(const ValueList& args, SharedValue result)
	{
		result->SetBool(this->IsPlaying());
	}
	void Sound::IsPaused(const ValueList& args, SharedValue result)
	{
		result->SetBool(this->IsPaused());
	}
	void Sound::OnComplete(const ValueList& args, SharedValue result)
	{
		if (args.size()!=1)
		{
			throw ValueException::FromString("onComplete takes 1 parameter");
		}
		if (!args.at(0)->IsMethod())
		{
			throw ValueException::FromString("onComplete takes a function parameter");
		}
		SharedBoundMethod method = args.at(0)->ToMethod();
		this->OnComplete(method);
	}
}
