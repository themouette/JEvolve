<?php /* Smarty version 2.6.0, created on 2011-01-17 16:18:07
         compiled from menuitem.tpl */ ?>
<?php if ($this->_tpl_vars['menuitem']['text'] != ""): ?>
    <?php if ($this->_tpl_vars['subitem'] == true): ?>
        <ul>
    <?php else: ?>
        <ul id='browser'>
    <?php endif; ?>

    <li id="<?php echo $this->_tpl_vars['menuitem']['ID']; ?>
">
        <?php if ($this->_tpl_vars['menuitem']['type'] == 'class' || $this->_tpl_vars['menuitem']['type'] == 'file' || $this->_tpl_vars['menuitem']['type'] == 'todo' || $this->_tpl_vars['menuitem']['type'] == 'errors'): ?>
                <span class="<?php echo $this->_tpl_vars['menuitem']['type']; ?>
"><a href='<?php echo $this->_tpl_vars['menuitem']['href']; ?>
' rel='contents' class='menuitem'><?php echo $this->_tpl_vars['menuitem']['text']; ?>
</a></span>
        <?php else: ?>
                <span class="<?php echo $this->_tpl_vars['menuitem']['type']; ?>
"><?php echo $this->_tpl_vars['menuitem']['text']; ?>
</span>
        <?php endif; ?>
        <?php if ($this->_tpl_vars['menuitem']['children']): ?> 
            <?php if (count($_from = (array)$this->_tpl_vars['menuitem']['children'])):
    foreach ($_from as $this->_tpl_vars['key'] => $this->_tpl_vars['value']):
?>
                <?php $_smarty_tpl_vars = $this->_tpl_vars;
$this->_smarty_include(array('smarty_include_tpl_file' => 'menuitem.tpl', 'smarty_include_vars' => array('menuitem' => $this->_tpl_vars['value'],'subitem' => true)));
$this->_tpl_vars = $_smarty_tpl_vars;
unset($_smarty_tpl_vars);
 ?>
            <?php endforeach; unset($_from); endif; ?>
        <?php endif; ?>
    </li>
</ul>
<?php endif; ?>