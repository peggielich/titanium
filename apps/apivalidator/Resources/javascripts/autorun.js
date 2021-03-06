/*!(c) 2006-2008 Appcelerator, Inc. http://appcelerator.org
 * Licensed under the Apache License, Version 2.0. Please visit
 * http://license.appcelerator.com for full copy of the License.
 **/

// A simple plugin that automatically runs all test suites on page load
$(document).ready(function() {
	Titanium.API.debug("install autorunner plugin..");
	var autoclose = true;
	
	TestMonkey.installTestRunnerPlugin({
		onManifestLoaded: function ()
		{
			testRunner(testSuiteNames);
		},
		
		onAfterTestRunner: function()
		{
			if (autoclose) {
				Titanium.App.exit();
			}
		}
	});
});
