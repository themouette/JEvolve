<?php /* Smarty version 2.6.0, created on 2011-01-17 16:18:08
         compiled from filesource.tpl */ ?>
<div class='page_content'>
    <?php ob_start(); ?>File Source for <?php echo $this->_tpl_vars['name'];  $this->_smarty_vars['capture']['tutle'] = ob_get_contents(); ob_end_clean(); ?>
    <h1>Source for file <?php echo $this->_tpl_vars['name']; ?>
</h1>
    <p>Documentation is available at <?php echo $this->_tpl_vars['docs']; ?>
</p>
    <div class="src-code">
    <?php echo $this->_tpl_vars['source']; ?>

    </div>
</div>