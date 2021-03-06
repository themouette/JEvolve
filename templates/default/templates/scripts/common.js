var CurrentPage = null;

/** Page Load **/
$(document).ready(function() {
    /** Load the menu and initial page **/

	$tabs = $("#tabs").tabs({
		tabTemplate: '<li><a href="#{href}">#{label}</a> <span class="ui-icon ui-icon-close">Remove Tab</span></li>',
		load: function(event, ui) { AttachLinkEvents(); },
		add: function (event, ui) { $tabs.tabs('select', '#' + ui.panel.id); },
		ajaxOptions: {
			error: function( xhr, status, index, anchor ) {
				$( anchor.hash ).html("Couldn't load this tab. We'll try to fix this as soon as possible. ");
				}
			}
		
	});
	
	$('#tabs span.ui-icon-close').live('click', function() {
		var index = $('li',$tabs).index($(this).parent());
		$tabs.tabs('remove', index);
	});

    loadMenu();

	$('body').layout({
		applyDefaultStyles: true,
		west: {
			resizable:		true,
			size:					250,
			onresize_end: resizeLeft},
		south: {
			resizable:		false,
			size:					31,
			spacing_open: 0,
			slidable:     false}});
	$('#left').layout({
		north: {
			resizable:		false,
			spacing_open: 0,
			size:					30,
			slidable:			false}});
	
	
		resizeSearch()

		$("#search").autocomplete({
			minLength: 0,
			delay:300,
			search: function(event, ui){
				searchFor(this.value)
				return false;
			}});
		searchFor($("#search").val());

});

function resizeSearch(){
	$('#search').width($('#left').innerWidth() - 18);
}

function resizeLeft() {
	resizeSearch();
	$('#left').layout('resizeAll')
}

function searchFor(search) {
	if(search && search.length)
	{
		$("#menu")
			.jstree('search', search)
			.find('li:not(:has(.jstree-search))')
				.css('display', 'none')
			.end()
			.find('li:has(.jstree-search)')
				.css('display', 'block')
			.end();
	}
	else
	{
		$("#menu")
			.find('.jstree-search')
				.removeClass('jstree-search')
			.end()
			.find('li.jstree-last')
				.css('display', null)
			.end();
	}
}

/** Loads the menu in the sidebar **/
function loadMenu() {
    GetPage('treemenu.html', 
        function(pResponse) {  
            $("#menu").jstree({
				"core" : { 'animation':0},
                "plugins" : [ "html_data", "ui", "themes", "cookies", "search"],
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
                },
								"search": {
									"case_insensitive" : true,
									"ajax": false
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
            type: "GET",
            url: pUrl,
            dataType: 'html',
            async: true,
            error: function (){
                $('<div class="error" title="error" ><p>Unable to load '+pUrl+'</p></div>')
                  .dialog({
                    modal: true
                    })
                  .css('z-index', 20001);
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
	$tabs.tabs('select', $tabs.tabs('length') - 1 );
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
