<?php /* Smarty version 2.6.0, created on 2011-01-17 16:17:53
         compiled from ric.tpl */ ?>
<?php require_once(SMARTY_DIR . 'core' . DIRECTORY_SEPARATOR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('modifier', 'htmlentities', 'ric.tpl', 4, false),)), $this); ?>
<div class='page_content'>
    <div align="center"><h1><?php echo $this->_tpl_vars['name']; ?>
</h1></div>
    <pre>
        <?php echo ((is_array($_tmp=$this->_tpl_vars['contents'])) ? $this->_run_mod_handler('htmlentities', true, $_tmp) : smarty_modifier_htmlentities($_tmp)); ?>

    </pre>
</div>