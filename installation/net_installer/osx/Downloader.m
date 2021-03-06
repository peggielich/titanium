/**
 * Appcelerator Kroll - licensed under the Apache Public License 2
 * see LICENSE in the root folder for details on the license.
 * Copyright (c) 2009 Appcelerator, Inc. All Rights Reserved.
 */

#import "Downloader.h"

#if !USEURLREQUEST
#import "CURLHandle.h"
#endif

@implementation Downloader

-(id)initWithURL:(NSURL*)url progress:(NSProgressIndicator*)p
{
	self = [super init];
	if (self)
	{
		progress = p;
		[progress startAnimation:self];

#if USEURLREQUEST
		NSMutableURLRequest * downloadRequest = [[NSMutableURLRequest alloc] initWithURL:url];
		[downloadRequest setTimeoutInterval:4.0];
		[downloadRequest setValue:@"Mozilla/5.0 (compatible; Titanium_Downloader/0.3; Mac)" 
			   forHTTPHeaderField:@"User-Agent"];
		
		data = [[NSMutableData alloc] init];
		downloadConnection = [[NSURLConnection alloc] initWithRequest:downloadRequest delegate:self];
		[downloadRequest release];
		[downloadConnection start];
		[self setCompleted:NO];

#else
		handle = (CURLHandle *)[url URLHandleUsingCache:NO];
		[handle retain];
		[handle setFailsOnError:YES];	
		[handle setFollowsRedirects:YES];
		[handle setConnectionTimeout:4];
		[handle setUserAgent:@"Mozilla/5.0 (compatible; Titanium_Downloader/0.2; Mac)"];
		[handle addClient:self];
		[handle setProgressIndicator:progress];
		// directly call up the results
		[self URLHandleResourceDidFinishLoading:handle];
//		if (NSURLHandleLoadFailed == [handle status])
//		{
//			[oResultCode setStringValue:[mURLHandle failureReason]];
//		}
#endif
	}
	return self;
}
-(void)dealloc
{
	[handle release];
	[data release];
	[super dealloc];
}

-(NSData*)data
{
	return [[data retain] autorelease];
}

- (BOOL)completed;
{
	return completed;
}

- (void)setCompleted:(BOOL)value;
{
	completed = value;
	if (completed) {
		[progress stopAnimation:self];
#if !USEURLREQUEST
		[handle removeClient:self];
#endif
	} else {
		[progress startAnimation:self];
	}
}

-(BOOL)isDownloadComplete
{
	return [self completed];
}

#pragma mark NSURLConnection delegate methods

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
	[data setLength:0];
	expectedBytes = [response expectedContentLength];
}


- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)newData
{
	[data appendData:newData];
	if (expectedBytes != 0) {
		[progress setIndeterminate:NO];
		[progress setMaxValue:(double)expectedBytes];
		[progress setDoubleValue:(double)[data length]];
	}
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
	[progress setIndeterminate:YES];
	[self setCompleted:YES];
}


- (void)connectionDidFinishLoading:(NSURLConnection *)connection;
{
	[self setCompleted:YES];
}


#if	!USEURLREQUEST

- (void)URLHandle:(NSURLHandle *)sender resourceDataDidBecomeAvailable:(NSData *)newBytes
{
	id contentLength = [sender propertyForKeyIfAvailable:@"content-length"];
	bytesRetrievedSoFar += [newBytes length];
	
	if (nil != contentLength)
	{
		double total = [contentLength doubleValue];
		[progress setIndeterminate:NO];
		[progress setMaxValue:total];
		[progress setDoubleValue:bytesRetrievedSoFar];
	}
}

- (void)URLHandleResourceDidBeginLoading:(NSURLHandle *)sender
{
	[self setCompleted:NO];
}

- (void)URLHandleResourceDidFinishLoading:(NSURLHandle *)sender
{
	data = [handle resourceData];
	[data retain];
	[self setCompleted:YES];
}

- (void)URLHandleResourceDidCancelLoading:(NSURLHandle *)sender
{
	[progress setIndeterminate:YES];
	[self setCompleted:YES];
}

- (void)URLHandle:(NSURLHandle *)sender resourceDidFailLoadingWithReason:(NSString *)reason
{
	[progress setIndeterminate:YES];
	[self setCompleted:YES];
}
#endif

@end
