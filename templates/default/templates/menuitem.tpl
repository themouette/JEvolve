{if $menuitem.text != ""}
    {if $subitem == true}
        <ul>
    {else}
        <ul id='browser'>
    {/if}

    <li id="{$menuitem.ID}">
        {if $menuitem.type == 'class' || $menuitem.type=='file' || $menuitem.type=='todo' || $menuitem.type =='errors'}
                <span class="{$menuitem.type}"><a href='{$menuitem.href}' rel='contents' class='menuitem'>{$menuitem.text}</a></span>
        {else}
                <span class="{$menuitem.type}">{$menuitem.text}</span>
        {/if}
        {if $menuitem.children} 
            {foreach from=$menuitem.children key=key item=value}
                {include file='menuitem.tpl' menuitem=$value subitem=true}
            {/foreach}
        {/if}
    </li>
</ul>
{/if}
