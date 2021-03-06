/*!(c) 2006-2008 Appcelerator, Inc. http://appcelerator.org
 * Licensed under the Apache License, Version 2.0. Please visit
 * http://license.appcelerator.com for full copy of the License.
 **/


$(document).ready(function()
{
	var testCount = 0;
	var suiteCount = 0;
	var cellWidth = 0;
	
	// processing vars
	var currentAssertion = 0;
	var currentSuite = 1;
	
	// counts
	var failureCount = 0;
	var passedCount = 0;
	var errorCount = 0;
	
	function resetStats()
	{
		testCount = 0;
		suiteCount = 0;
		failureCount = 0;
		passedCount = 0;
		errorCount = 0;
		
		$('#status_bar').empty();
		$("#results").empty();
	}
	
	// keyboard shortcuts
	$(document).bind('keydown',function(e)
	{
		if (e.keyCode == 37 || e.keyCode==39)
		{
			// left
			var select = $("#selector select").get(0);
			var idx = select.selectedIndex;
			switch(e.keyCode)
			{
				case 37:
				{
					idx-=1;
					break;
				}
				case 39:
				{
					idx+=1;
					break;
				}
			}
			if (idx < 0) idx = select.options.length-1;
			if (idx > select.options.length-1) idx = 0;
			select.selectedIndex = idx;
			return false;
		}
		else if (e.keyCode == 82)
		{
			if (e.metaKey || e.ctrlKey) return; // command+r is reload, ignore these
			// run is 'r' or runall is 'shift+r'
			var id = "#run" + (e.shiftKey ? "all":"");
			$(id).click();
			return false;
		}
	});
	
	var lastSuite = null;
	$.getJSON('app://' + Titanium.App.getID() + '/tests/manifest.js',function(json)
	{
		$.each(json.suites,function(i)
		{
			$.getScript('app://' + Titanium.App.getID() + '/tests/'+this, function() {
					if (i == json.suites.length - 1) {
						TestMonkey.fireEventAsync('manifestLoaded');
					}
			});
		});
	});
	
	$("#run").click(function()
	{
		var test = $("#selector select").val();
		Titanium.App.Properties.setString("testmonkey.test", test);// remember the last test we ran
//		$.cookie('testmonkey.test',test); 
		testRunner(test);
	});
	
	$("#runall").click(function()
	{
		var select = $("#selector select").get(0);
		var tests = [];
		for (var c=0;c<select.length;c++)
		{
			tests.push(select.options[c].text);
		}
		testRunner(tests);
	});
	
	function leadingWhitepsace(code)
	{
		var count = 0;
		var spacing = '';
		while (true)
		{
			var ch = code.charAt(count);
			if (ch != ' ')
			{
				break;
			}
			count++;
			spacing+=' ';
		}
		return spacing;
	}
	
	var idCounter = 1;
	TestMonkey.installTestRunnerPlugin({
		onAddTestSuite: function(suitename) {
			//var lastTest = $.cookie('testmonkey.test');
			//var lastTest = Titanium.App.Properties.getString("testmonkey.test")
			//Titanium.API.debug("lasttest="+lastTest);
			var sel = '';
			//if (lastTest == result) sel = 'selected';
			
			$("#selector select").append("<option "+sel+">"+suitename+"</option>");
			
		},
		
		onAfterAssertionCount: function(assertionCount)
		{
			// set assertion count total
			$('#assert_count').html(assertionCount)
			
			// display
			$('#summary').css('display','block')
			
			// reset 
			$('#status_bar').empty();
			currentAssertion = 0;
			currentSuite  = 1;
			
			// calculate cell width and pre-populate
			var width = $('#status_bar').width();
			var totalCellWidth = (width -20) /assertionCount;
			cellWidth = totalCellWidth - 5;

			for (var i=0;i<assertionCount;i++)
			{
				$('#status_bar').append('<div id="assert_bar_'+i+'" style="float:left;background-color:#ccc;margin-left:5px;height:15px;width:'+cellWidth+'px"></div>');
			}
		},
		
		onBeforeTestRunner: function(suites)
		{
			resetStats();
			$('#test_summary_title').show();
				
			suiteCount = suites.length;					
			$('#suite_count').html(suiteCount);
			for (var i=0;i<suiteCount;i++)
			{
				$("#results").append("<div id='testsuite_"+ (i+1) +"' class='testsuite'><div class='test_count' id='passed_count_" + (i+1) + "'>0</div><div class='test_count' id='failed_count_" + (i+1) + "'>0</div><div class='test_count' id='error_count_" + (i+1) + "'>0</div><div class='testsuite_name'><a href='#' id='testsuite_detail_link_"+(i+1)+"'>"+suites[i][0]+"</a></div><div style='clear:both;padding-top:20px;' id='testsuite_detail_"+(i+1)+"'><div id='testsuite_detail_error_"+(i+1)+"' style='display:none'></div><div id='testsuite_detail_failed_"+(i+1)+"' style='display:none'></div><div id='testsuite_detail_passed_"+(i+1)+"' style='display:none;'></div><div id='testsuite_detail_all_"+(i+1)+"' style='display:none;'></div></div></div>");						

				$('#testsuite_detail_link_' + (i+1)).click(function()
				{
					var suite = this.id.substring(this.id.length-1)
					var el = $('#testsuite_detail_all_' + suite);
					var errorDiv = $('#testsuite_detail_error_' + suite);
					var failedDiv = $('#testsuite_detail_failed_' + suite);
					var passedDiv = $('#testsuite_detail_passed_' + suite);
					if (el.css('display') == 'block') 
					{
						el.slideUp() 
					}
					else
					{
						el.slideDown();
						errorDiv.slideUp();
						failedDiv.slideUp();
						passedDiv.slideUp();
					} 
				});

				$('#passed_count_' + (i+1)).click(function()
				{
					var suite = this.id.substring(this.id.length-1)
					var el = $('#testsuite_detail_passed_' + suite);
					var errorDiv = $('#testsuite_detail_error_' + suite);
					var failedDiv = $('#testsuite_detail_failed_' + suite);
					var all = $('#testsuite_detail_all_' + suite);
					if (el.css('display') == 'block') 
					{
						el.slideUp(); 
					}
					else
					{
						el.slideDown();
						errorDiv.slideUp();
						failedDiv.slideUp();
						all.slideUp();
					} 
				});
				$('#failed_count_' + (i+1)).click(function()
				{
					var suite = this.id.substring(this.id.length-1)
					var el = $('#testsuite_detail_failed_' + suite);
					var errorDiv = $('#testsuite_detail_error_' + suite);
					var passedDiv = $('#testsuite_detail_passed_' + suite);
					var all = $('#testsuite_detail_all_' + suite);
					if (el.css('display') == 'block') 
					{
						el.slideUp(); 
					}
					else
					{
						el.slideDown();
						errorDiv.slideUp();
						passedDiv.slideUp();
						all.slideUp();
					} 

				});
				$('#error_count_' + (i+1)).click(function()
				{
					var suite = this.id.substring(this.id.length-1)
					var el = $('#testsuite_detail_error_' + suite);
					var passedDiv = $('#testsuite_detail_passed_' + suite);
					var failedDiv = $('#testsuite_detail_failed_' + suite);
					var all = $('#testsuite_detail_all_' + suite);				
					if (el.css('display') == 'block') 
					{
						el.slideUp() 
					}
					else
					{
						el.slideDown();
						failedDiv.slideUp();
						passedDiv.slideUp();
						all.slideUp();
					} 

				});
			}
		},
		
		onAfterTestRunner: function()
		{
			$(".testdetail").click(function()
			{
				var child = $(this).children('.result');
				$(child).css('display') == 'none' ? $(child).slideDown() : $(child).slideUp();
			});
		},
		
		onBeforeTestCases: function(tests)
		{
			testCount = tests.length;
			$('#test_count').html(tests.length);
		},
		
		onAfterTestSuite: function()
		{
			currentSuite++;
			failureCount = 0;
			passedCount = 0;
			errorCount = 0;
		},
		
		onAfterTestCase: function(test)
		{
			try
			{
				var errorMessage = test.testcase;
				var failedCount = 0;
				$.each(test.results,function()
				{							
					failedCount +=this.result ? 0 : 1;
					var cls = this.result ? 'passed' : 'failed';
					var idx = errorMessage.indexOf(this.assert);
					if (this.error)
					{
						errorCount++;
						$('#error_count_'+currentSuite).html(errorCount).addClass('error_count');				
						$('#assert_bar_' + currentAssertion).css('background-color','red');
					}
					else if (cls=='passed')
					{
						passedCount++
						$('#passed_count_'+currentSuite).html(passedCount).addClass('passed_count');
						$('#assert_bar_' + currentAssertion).css('background-color','green');
					}
					else 
					{
						failureCount++
						$('#failed_count_'+currentSuite).html(failureCount).addClass('failed_count');
						$('#assert_bar_' + currentAssertion).css('background-color','orange');
					}
					currentAssertion++;
					if (idx != -1)
					{
						var newMessage = errorMessage.substring(0,idx);
						var line = errorMessage.substring(idx,idx+this.assert.length);
						newMessage+=leadingWhitepsace(line);
						newMessage+='<span class="'+cls+'">';
						newMessage+=$.trim(line);
						newMessage+='</span>';
						if (this.error)
						{
							newMessage+='<span class="error">'+this.error+'</span>';	
						}
						else if (!this.result && this.message)
						{
							newMessage+='<span class="error">'+this.message+'</span>';	
						}
						newMessage+=errorMessage.substring(idx+this.assert.length);
						errorMessage = newMessage;
					}
				});
				if (test.explicitFailure)
				{
					if (failedCount==0) failedCount=1;
					var re = /fail[\s]?\((.*)?\)/;
					var matches = [];
					errorMessage=$.gsub(errorMessage,re,function(m)
					{
						if (m[0].indexOf(test.message) > 0)
						{
							matches.push(extractCodeLine(errorMessage,m[0]));
						}
						return m[0];
					});
					$.each(matches,function()
					{
						var idx = errorMessage.indexOf(this);
						var newMessage = errorMessage.substring(0,idx);
						var line = errorMessage.substring(idx,idx+this.length);
						newMessage+=leadingWhitepsace(line);
						newMessage+='<span class="failed">';
						newMessage+=$.trim(line);
						newMessage+='</span>';
						newMessage+=errorMessage.substring(idx+this.length);
						errorMessage=newMessage;
					});
				}
				if (typeof(test.timeout)!='undefined')
				{
					errorMessage = 'testAsync("' + test.name + '",'+test.timeout+',<span class="fn">'+errorMessage+'</span>);'
				}
				else
				{
					errorMessage = 'test("' + test.name + '",<span class="fn">'+errorMessage+'</span>);'
				}
				var id = idCounter++;
				var html = "<div class='testdetail'><div class='testresult "+(test.failed?'failed':'passed')+"'>"+(test.error ? 'Error' : test.failed?('Failed <span class="count">('+failedCount+')</span>'):'Passed')+"</div><div>"+test.name+"</div><div class='clear'></div>"; 
				html+="<div  style='display:none' class='result "+(test.error?'error':'')+"'>";
				html+=errorMessage;
				if (test.error)
				{
					html+="<div class='error_msg'>" + test.error + "</div>";
				}
				if (test.logs)
				{
					html+="<div class='logs'><h1>Test log results:</h1>" + test.logs.join("\n") + "</div>";
				}
				html+='<div class="html"><h1>HTML page (after test):</h1><pre><code>'+$.escapeHTML(test.after_dom)+'</code></pre></div>';
				html+="</div></div>";

				$('#testsuite_detail_all_'+currentSuite).append(html)
				if (test.error)
				{
					$('#testsuite_detail_error_' + currentSuite).append(html)
				}
				else if (test.failed)
				{
					$('#testsuite_detail_failed_' + currentSuite).append(html)
				}
				else
				{
					$('#testsuite_detail_passed_' + currentSuite).append(html)							
				}
			}
			catch (E)
			{
				//$.error(E);
				Titanium.API.debug(E);
			}
		}
	});
});

