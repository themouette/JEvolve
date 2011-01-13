var CurrentPage = null;

/** Page Load **/
$(document).ready(function() {
    /** Load the menu and initial page **/

	$tabs = $("#tabs").tabs({
		tabTemplate: '<li><a href="#{href}">#{label}</a> <span class="ui-icon ui-icon-close">Remove Tab</span></li>',
		load: function(event, ui) { AttachLinkEvents(); }
		
	});
	
	$('#tabs span.ui-icon-close').live('click', function() {
		var index = $('li',$tabs).index($(this).parent());
		$tabs.tabs('remove', index);
	});

    loadMenu();
});

/** Loads the menu in the sidebar **/
function loadMenu() {
    GetPage('treemenu.html', 
        function(pResponse) {  
            $("#menu").jstree({
				"core" : { 'animation':0},
                "plugins" : [ "html_data", "ui", "themes", "cookies"],
                "cookies" : {
                    "save_opened" : "evolved_tree",
                    "save_selected" : "evolved_selected",
                    "auto_save" : true
                },
                "themes" : {
                    "theme" : "classic",
                    "dots" : false,
                    "icons" : false
                },
                "html_data" : {
                    "data" : pResponse
                }
            });
            ShowLoading(false);
            AttachLinkEvents();

            var goTo = $.query.get('GoTo');
            if(goTo) {
                ShowPage(unescape(goTo));
            } else {

            }
        }
    );
}

/** Gets the content of a page **/
function GetPage(pUrl, pSuccessCallBack, pErrorCallBack){
    $.ajax({
            type: "POST",
            url: pUrl,
            dataType: 'html',
            async: true,
            error: function (){
                new pErrorCallBack();
            },
            success: function (response) {
                new pSuccessCallBack(response);
            }
        })
}

/** Puts the content of a page in the page div + rescan the page for the href's **/
function ShowPage(pUrl) {
	
	classs = $(pUrl).parent().attr('class');
	title = '<span class="' + classs + '">' + $(pUrl).text() + '</span>';
	url = pUrl.href;

	$tabs.tabs('add', url, title);
	
    AttachLinkEvents();

    CurrentPage = pUrl;
    var anchor = pUrl.hash;
    if(anchor) {
        document.location.hash = anchor;
    }
}

/** Attach the onclick events to all links that have rel=contents **/
function AttachLinkEvents() {
    var aElements = document.getElementsByTagName("a");
    for (i = 0; i < aElements.length; i++) {
        if (aElements[i].rel == "contents") {
            aElements[i].onclick = function() {
                var todomain = getDomain(this.href);
                var thisdomain = getDomain(window.location);
                if(todomain == thisdomain) {
                    ShowPage(this);
                    return false;
                } else {
                    window.open(this.href); return false;
                }
            }
        }
    }  
}

/** Opens a new window with the current state as url **/
function OpenNewWindow() {
    if(CurrentPage != null && window.location != CurrentPage) {
        window.open('?GoTo=' + escape(CurrentPage));
    } else {
        window.open('');
    }
    return false;
}

function ShowLoading(pShow) {
    if(pShow) {
        $('#loading').show();
        $('#loading-mask').show();        
    } else {
        $('#loading').hide();
        $('#loading-mask').hide();
    }
}

function getDomain(pUrl) {
    var url = new String(pUrl);
    return url.match(/:\/\/(www\.)?(.[^/:]+)/)[2];
}
