<div class='page_content'>
    {if $nav}
        {include file="tutorial_nav.tpl" prev=$prev next=$next up=$up prevtitle=$prevtitle nexttitle=$nexttitle uptitle=$uptitle}
    {/if}

    {$contents}

    {if $nav}
        {include file="tutorial_nav.tpl" prev=$prev next=$next up=$up prevtitle=$prevtitle nexttitle=$nexttitle uptitle=$uptitle}
    {/if}
</div>