<?php
/**
 * Custom converter based on the Extjs converter template
 * <p>
 * Based on Extjs {@link http://zymengine.com/dev/news/30-phpdoc-extjs-converter-template Extjs Website}
 * </p>
 * <p> 
 * This converter has an extra tag namespace to solve the namespace shortcomings of phpdocumentor in the UI
 * It does not change the way phpdocumentor handles namespaces but does allow you to organize your generated documentation a bit more.
 * 
 * Allot of the Extjs code is still in the converter, the changes are in the generation of the tree view and in making the namespace tag available
 *
 * </p>
 * @project JEvolve
 * @package Converters
 */
class HTMLSmartyJEvolveConverter extends HTMLSmartyConverter {
    /** @var string */
    var $name = 'Smarty/JEvolve';
     
    /**
     * sets {@link $base_dir} to $targetDir
     * @see Converter()
     */
    function HTMLSmartyJEvolveConverter(&$allp, &$packp, &$classes, &$procpages, $po, $pp, $qm, $targetDir, $templateDir, $title)
    {
        Converter::Converter($allp, $packp, $classes, $procpages,$po, $pp, $qm, $targetDir, $templateDir, $title);
        $this->base_dir = $targetDir;
    }
    
    /**
     * @param string full path to the source file
     * @param string fully highlighted source code
     */
    function writeSource($path, $value) {
        $templ = &$this->newSmarty();
        $pathinfo = $this->proceduralpages->getPathInfo($path, $this);
        $templ->assign('source',$value);
        $templ->assign('package',$pathinfo['package']);
        $templ->assign('subpackage',$pathinfo['subpackage']);
        $templ->assign('name',$pathinfo['name']);
        $templ->assign('source_loc',$pathinfo['source_loc']);
        $templ->assign('docs',$pathinfo['docs']);
        $templ->assign("subdir",'');
        $templ->register_outputfilter('HTMLSmartyJEvolve_outputfilter');
        $this->setTargetDir($this->getFileSourcePath($this->base_dir));
        phpDocumentor_out("\n");
        $this->setSourcePaths($path);
        $this->writefile($this->getFileSourceName($path).'.html',$templ->fetch('filesource.tpl'));
    }
    
    function writeExample($title, $path, $source) {
        $templ = &$this->newSmarty();
        $templ->assign('source',$source);
        if (empty($title))
        {
            $title = 'example';
            addWarning(PDERROR_EMPTY_EXAMPLE_TITLE, $path, $title);
        }
        $templ->assign('title',$title);
        $templ->assign('file',$path);
        $templ->assign("subdir",'');
        $templ->register_outputfilter('HTMLSmartyJEvolve_outputfilter');
        $this->setTargetDir($this->base_dir . PATH_DELIMITER . '__examplesource');
        phpDocumentor_out("\n");
        $this->writefile('exsource_'.$path.'.html',$templ->fetch('examplesource.tpl'));
    }

    function getExampleLink($path, $title)  {
        return $this->returnLink('{$subdir}__examplesource' . PATH_DELIMITER . 'exsource_'.$path.'.html',$title);
    }
    
    function getSourceLink($path)  {
        return $this->returnLink('{$subdir}__filesource/' .
        $this->getFileSourceName($path).'.html','Source Code for this file');
    }

    /**
     * Used to convert the <<code>> tag in a docblock
     * @param string
     * @param boolean
     * @return string
     */
    function ProgramExample($example, $tutorial = false, $inlinesourceparse = null/*false*/,    $class = null/*false*/, $linenum = null/*false*/, $filesourcepath = null/*false*/)   {
        return parent::ProgramExample($example, $tutorial, $inlinesourceparse, $class, $linenum , $filesourcepath);
    }
    
    /**
     * @param string
     */
    function TutorialExample($example) {
        $trans = $this->template_options['desctranslate'];
        $this->template_options['desctranslate'] = array();
        $example = '<ol>' . parent::TutorialExample($example)
               .'</ol>';
        $this->template_options['desctranslate'] = $trans;
        if (!isset($this->template_options['desctranslate'])) return $example;
        if (!isset($this->template_options['desctranslate']['code'])) return $example;
        $example = $this->template_options['desctranslate']['code'] . $example;
        if (!isset($this->template_options['desctranslate']['/code'])) return $example;
        return $example . $this->template_options['desctranslate']['/code'];
    }
    
    function getCurrentPageLink() {
        return $this->page_dir . '/' . $this->curname . '.html';
    }

    /**
     * Uses htmlspecialchars() on the input
     */
    function postProcess($text) {
        if ($this->highlightingSource) {
            return str_replace(array(' ',"\t"), array('&nbsp;', '&nbsp;&nbsp;&nbsp;'),
                htmlspecialchars($text));
        }
        return htmlspecialchars($text);
    }
    
    /**
     * Use the template tutorial_toc.tpl to generate a table of contents for HTML
     * @return string table of contents formatted for use in the current output format
     * @param array format: array(array('tagname' => section, 'link' => returnsee link, 'id' => anchor name, 'title' => from title tag),...)
     */
    function formatTutorialTOC($toc) {
        $template = &$this->newSmarty();
        $template->assign('toc',$toc);
        return $template->fetch('tutorial_toc.tpl');
    }
    
    function &SmartyInit(&$templ) {
        if (!isset($this->package_index))
        foreach($this->all_packages as $key => $val)
        {
            if (isset($this->pkg_elements[$key]))
            {
                if (!isset($start)) $start = $key;
                $this->package_index[] = array('link' => "li_$key.html", 'title' => $key);
            }
        }
        $templ->assign("packageindex",$this->package_index);
        $templ->assign("subdir",'');
        return $templ;
    }
    
    /**
     * Writes out the template file of {@link $class_data} and unsets the template to save memory
     * @see registerCurrentClass()
     * @see parent::endClass()
     */
    function endClass()  {
        $a = '../';
        if (!empty($this->subpackage)) $a .= '../';
        if ($this->juststarted)
        {
            $this->juststarted = false;
            phpDocumentor_out("\n");
            flush();
        }
        $this->setTargetDir($this->base_dir . PATH_DELIMITER . $this->class_dir);
        $this->class_data->assign("subdir", '');
        $this->class_data->register_outputfilter('HTMLSmartyJEvolve_outputfilter');
        $this->writefile($this->class . '.html',$this->class_data->fetch('class.tpl'));
        unset($this->class_data);
    }
    
    /**
     * Writes out the template file of {@link $page_data} and unsets the template to save memory
     * @see registerCurrent()
     * @see parent::endPage()
     */
    function endPage() {
        $this->package = $this->curpage->package;
        $this->subpackage = $this->curpage->subpackage;
        $a = '../';
        if (!empty($this->subpackage)) $a .= '../';
        $this->setTargetDir($this->base_dir . PATH_DELIMITER . $this->page_dir);
        $this->page_data->assign("package",$this->package);
        $this->page_data->assign("subdir", '');
        $this->page_data->register_outputfilter('HTMLSmartyJEvolve_outputfilter');
        $this->writefile($this->page . '.html',$this->page_data->fetch('page.tpl'));
        unset($this->page_data);
    }
    
    /**
     * @param string
     * @param string
     * @return string &lt;a href="'.$link.'">'.$text.'</a&gt;
     */
    function returnLink($link,$text) {
        return '<a href="'.$link.'" rel="contents">'.$text.'</a>';
    }
    
    function makeLeft() {
        foreach($this->page_elements as $package => $o1) {
            foreach($o1 as $subpackage => $links) {
                for($i=0;$i<count($links);$i++) {
                    $left[$package][$subpackage]['files'][] = array("link" => $this->getId($links[$i]), "title" => $links[$i]->name, 'project' => (isset($links[$i]->project)) ? $links[$i]->project : null);
                }
            }
        }

        $interfaces = $classes = false;
        foreach($this->class_elements as $package => $o1) {
            foreach($o1 as $subpackage => $links) {
                for($i=0;$i<count($links);$i++) {
                    $class = $this->classes->getClassByPackage($links[$i]->name, $links[$i]->package);
                    $isinterface = $isclass = false;
                    if ($class->isInterface()) {
                        $isinterface = true;
                        $interfaces = true;
                    } else {
                        $isclass = true;
                        $classes = true;
                    }

                    if ($class && isset($class->docblock) && $class->docblock->hasaccess) {
                        $left[$package][$subpackage]['classes'][] =
                            array("link" => $this->getId($links[$i]), 
                                  "title" => $links[$i]->name,
                                  'is_interface' => $isinterface,
                                  'is_class' => $isclass,
                                  'project' => (isset($links[$i]->project)) ? $links[$i]->project : null,
                                  "access" => $class->docblock->tags['access'][0]->value,
                                  "abstract" => isset ($class->docblock->tags['abstract'][0]));
                    } else {
                        $left[$package][$subpackage]['classes'][] =
                            array("link" => $this->getId($links[$i]), 
                                  "title" => $links[$i]->name,
                                  'is_interface' => $isinterface,
                                  'is_class' => $isclass,
                                  'project' => (isset($links[$i]->project)) ? $links[$i]->project : null,
                                  "access" => 'public',
                                  "abstract" => isset ($class->docblock->tags['abstract'][0]));
                    }
                }
            }
        }
        
        foreach($this->function_elements as $package => $o1) {
            foreach($o1 as $subpackage => $links) {
                for($i=0;$i<count($links);$i++) {
                    $left[$package][$subpackage]['functions'][] =  array("link" => $this->getId($links[$i]), "title" => $links[$i]->name);
                }
            }
        }
        $ret = array();
        
        foreach($left as $package => $r) {
            $pd = 'blank';
            if (isset($this->package_pages[$package])) $pd = $package.'/package_'.$package.'.html';
            if (!isset($r[''])) {
                $pt = false;
                $ptnoa = false;
                $ptt = $package;
                if ($t = $this->hasTutorial('pkg',$package,$package,'')) {
                    $pt = $t->getLink($this);
                    $ptnoa = $this->getId($t->getLink($this,true));
                    $ptt = $t->getTitle($this);
                }
                $tutes = array();
                foreach($this->tutorial_tree as $root => $tr) {
                    if ($tr['tutorial']->package == $package && $tr['tutorial']->subpackage == '') {
                        $tutes[$tr['tutorial']->tutorial_type][] =
                            $this->getTutorialTree($tr['tutorial']);
                    }
                }
                if (isset($this->childless_tutorials[$package][$subpackage])) {
                    foreach($this->childless_tutorials[$package][$subpackage] as $ext => $other) {
                        foreach($other as $tutorial) {
                            $tutes[$tutorial->tutorial_type][] = $this->getTutorialTree($tutorial);
                        }
                    }
                }

                $ret[$package][] = array(
                        'package' => $package,
                        'subpackage' => '',
                        'packagedoc' => $pd,
                        'packagetutorial' => $pt,
                        'packagetutorialnoa' => $ptnoa,
                        'packagetutorialtitle' => $ptt,
                        'files' => array(),
                        'functions' => array(),
                        'classes' => array(),
                        'tutorials' => $tutes,
                        );
            }

            foreach($r as $subpackage => $info) {
                $my = array();
                $my['package'] = $package;
                if (isset($this->package_pages[$package]))
                $my['packagedoc'] = $pd;
                else
                $my['packagedoc'] = 'blank';
                $my['subpackage'] = $subpackage;
                if (empty($subpackage)) {
                    if ($t = $this->hasTutorial('pkg',$package,$package,$subpackage)) {
                        $my['packagetutorial'] = $t->getLink($this);
                        $my['packagetutorialnoa'] = $this->getId($t->getLink($this,true));
                        $my['packagetutorialtitle'] = $t->getTitle($this);
                    } else {
                        $my['packagetutorial'] = '<a href="blank.html" rel="contents">No Package-Level Tutorial</a>';
                        $my['packagetutorialnoa'] = 'blank.html';
                        $my['packagetutorialtitle'] = $package;
                    }
                } else {
                    if ($t = $this->hasTutorial('pkg',$subpackage,$package,$subpackage)) {
                        $my['subpackagetutorial'] = $this->returnSee($this->getTutorialLink($t));
                        $my['subpackagetutorialnoa'] = $this->getId($t->getLink($this,true));
                        $my['subpackagetutorialtitle'] = $t->getTitle($this);
                    } else {
                        $my['subpackagetutorial'] = false;
                        $my['subpackagetutorialnoa'] = false;
                        $my['subpackagetutorialtitle'] = $subpackage;
                    }
                }
                $tutes = array();
                foreach($this->tutorial_tree as $root => $tr) {
                    if ($tr['tutorial']->package == $package && $tr['tutorial']->subpackage == $subpackage) {
                        $tutes[$tr['tutorial']->tutorial_type][] = $this->getTutorialTree($tr['tutorial']);
                    }
                }
                
                $my['tutorials'] = $tutes;
                $my['files'] = $my['classes'] = $my['functions'] = array();
                if (isset($info['files']))
                $my['files'] = $info['files'];
                if (isset($info['classes']))
                $my['classes'] = $info['classes'];
                $my['hasclasses'] = $classes;
                $my['hasinterfaces'] = $interfaces;
                if (isset($info['functions']))
                $my['functions'] = $info['functions'];
                $ret[$package][] = $my;
            }
        }
        return $ret;
    }
    /*
    function getTutorialTree($tutorial,$k = false)
    {
        $ret = '';
        if (is_object($tutorial)) $tree = parent::getTutorialTree($tutorial); else $tree = $tutorial;
//        debug($this->vardump_tree($tree));exitexit;
        if (!$tree)
        {
            $template = &$this->newSmarty();
            $template->assign('subtree',false);
            $template->assign('name',str_replace('.','',$tutorial->name));
            $template->assign('parent',false);
            $template->assign('haskids',false);
            $template->assign('kids','');
            $link = new tutorialLink;
            $t = $tutorial;
            $link->addLink('',$t->path,$t->name,$t->package,$t->subpackage,$t->getTitle($this));
            $main = array('link' => $this->getId($link), 'title' => $link->title);
            $template->assign('main',$main);
            return $template->fetch('tutorial_tree.tpl');
        }
        if (isset($tree['kids']))
        {
            foreach($tree['kids'] as $subtree)
            {
                $ret .= $this->getTutorialTree($subtree, true);
            }
        }
        $template = &$this->newSmarty();
        $template->assign('subtree',$k);
        $template->assign('name',str_replace('.','',$tree['tutorial']->name));
        $template->assign('parent',($k ? str_replace('.','',$tree['tutorial']->parent->name) : false));
        $template->assign('haskids',strlen($ret));
        $template->assign('kids',$ret);
        $link = new tutorialLink;
        $t = $tree['tutorial'];
        $link->addLink('',$t->path,$t->name,$t->package,$t->subpackage,$t->getTitle($this));
        $main = array('link' => $this->getId($link), 'title' => $link->title);
        $template->assign('main',$main);
        $ret = $template->fetch('tutorial_tree.tpl');
				return $ret;
    }
    */
    
    /**
     * HTMLdefaultConverter chooses to format both package indexes and the complete index here
     *
     * This function formats output for the elementindex.html and pkgelementindex.html template files.  It then
     * writes them to the target directory
     * @see generateElementIndex(), generatePkgElementIndex()
     */
    function formatPkgIndex()
    {
        list($package_indexes,$packages,$mletters) = $this->generatePkgElementIndexes();
        for($i=0;$i<count($package_indexes);$i++)
        {
            $template = &$this->newSmarty();
            $this->package = $package_indexes[$i]['package'];
            $this->subpackage = '';
            $template->assign("index",$package_indexes[$i]['pindex']);
            $template->assign("package",$package_indexes[$i]['package']);
            $template->assign("letters",$mletters[$package_indexes[$i]['package']]);
            $template->register_outputfilter('HTMLSmartyJEvolve_outputfilter');
            $this->setTargetDir($this->base_dir);
                $this->writefile('elementindex_'.$package_indexes[$i]['package'].'.html',$template->fetch('pkgelementindex.tpl'));
        }
        phpDocumentor_out("\n");
        flush();
        }
    
    /**
     * HTMLdefaultConverter uses this function to format template index.html and packages.html
     *
     * This function generates the package list from {@link $all_packages}, eliminating any
     * packages that don't have any entries in their package index (no files at all, due to @ignore
     * or other factors).  Then it uses the default package name as the first package index to display.
     * It sets the right pane to be either a blank file with instructions on making package-level docs,
     * or the package-level docs for the default package.
     * @global string Used to set the starting package to display
     */
    function formatIndex() {
        global $phpDocumentor_DefaultPackageName;
        list($elindex,$mletters) = $this->generateElementIndex();
        $template = &$this->newSmarty();
        $template->assign("index",$elindex);
        $template->assign("letters",$mletters);
        $template->register_outputfilter('HTMLSmartyJEvolve_outputfilter');
        phpDocumentor_out("\n");
        flush();
        $this->setTargetDir($this->base_dir);
        $this->writefile('elementindex.html',$template->fetch('elementindex.tpl'));
        usort($this->package_index,"HTMLSmarty_pindexcmp");
        $index = &$this->newSmarty();
        foreach($this->all_packages as $key => $val)
        {
            if (isset($this->pkg_elements[$key]))
            {
                if (!isset($start)) $start = $key;
                if (!isset($this->package_pages[$key])) $this->writeNewPPage($key);
            }
        }
        // Created index.html
        if (isset($this->pkg_elements[$phpDocumentor_DefaultPackageName])) $start = $phpDocumentor_DefaultPackageName;
        $this->package = $start;
        $this->subpackage = '';
        $index->assign("package_count",count($this->pkg_elements));
        if (count($this->ric_set))
        $index->assign("package_count",2);
        $index->assign("date",date("r",time()));
        $index->assign("title",$this->title);
        $index->register_outputfilter('HTMLSmartyJEvolve_outputfilter');
        if (isset($this->tutorials[$start]['']['pkg'][$start . '.pkg']))
        {
            $index->assign("blank",$start.'/tutorial_'.$start.'.pkg');
        } elseif (isset($this->package_pages[$start]))
        {
            $index->assign("blank",$start.'/package_'.$start);
        }
        else
        {
            $index->assign("blank","blank");
            $blank = &$this->newSmarty();
            $blank->assign('package',$this->package);
            $this->setTargetDir($this->base_dir);
            $this->writefile("blank.html",$blank->fetch('blank.tpl'));
        }
        phpDocumentor_out("\n");
        flush();
        $this->setTargetDir($this->base_dir);
        $this->writefile("index.html",$index->fetch('index.tpl'));
        
        // Make welcome
        $welcome =& $this->newSmarty();
        $this->writefile('welcome.html', $welcome->fetch('welcome.tpl'));

        unset($index);
    }
    
    function writeNewPPage($key)  {  return; }
    
    /**
     * Creates the basic structure for the treeview data
     * @param string $pProject project name
     * @param string $pPackage package name
     * @param string $pSubPackage subpackage name
     * @param array $pTreeData array containing the current tree structure
     *
     * @return null, $pTreeData is passed by reference and is modified
     **/
    private function createStructure($pProject, $pPackage, $pSubPackage, &$pTreeData) {
        if(!empty($pProject)) {
            if(!isset($pTreeData['children'][$pProject])) { 
                $pTreeData['children'][$pProject] = array(
                    'type'              => 'project',
                    'text'              => $pProject,
                    'children'          => array(), 
                    'ID' => md5(mt_rand())
                );
            }
        }
        
        if(!empty($pPackage)) {
            $info = array('type'=> 'package','text'=> $pPackage,'children'=> array(), 'ID' => md5(mt_rand()));
            if(!empty($pProject)) {
                if(!isset($pTreeData['children'][$pProject]['children'][$pPackage])) { $pTreeData['children'][$pProject]['children'][$pPackage] = $info; }
            } else {
                if(!isset($pTreeData['children'][$pPackage])) { $pTreeData['children'][$pPackage] = $info; }
            }
        }
        
        if(!empty($pSubPackage)) {
            $info = array('text'=> $pSubPackage,'type'=> 'subpackage','children'=> array(), 'ID' => md5(mt_rand()));
            if(!empty($pProject)) {
                if(!isset($pTreeData['children'][$pProject]['children'][$pPackage]['children'][$pSubPackage])) {
                    $pTreeData['children'][$pProject]['children'][$pPackage]['children'][$pSubPackage] = $info;
                }
            } else {
                if(!isset($pTreeData['children'][$pPackage]['children'][$pSubPackage])) {
                    $pTreeData['children'][$pPackage]['children'][$pSubPackage] = $info;
                }
            }
        }
    }
    
    /**
     * Adds a class to the treeview structure
     * @param array $pClassData array containing information about the class
     * @param array $pTreeData array passed by reference containing the current tree structure 
     *
     * @return null, $pTreeData is passed by reference and is modified
     **/
    private function addClassToTreeView($pClassData, &$pTreeData) {
        $classTreeRoot = array('type'=> 'classes', 'text'=> 'Classes', 'children'=> array(), 'ID' => md5(mt_rand()));        
        $classTreeChildren = array('type' => 'class','text' => $pClassData['title'], 'href'=> $pClassData['link'], 'project' => $pClassData['project'], 'ID' => md5(mt_rand()));

        $this->createStructure($pClassData['project'], $pClassData['package'], $pClassData['subpackage'], $pTreeData);

        if(empty($pClassData['project'])) {
            if(!empty($pClassData['subpackage'])) { 
                if(!isset($pTreeData['children'][$pClassData['package']]['children'][$pClassData['subpackage']]['children']['Classes'])) {
                    $pTreeData['children'][$pClassData['package']]['children'][$pClassData['subpackage']]['children']['Classes'] = $classTreeRoot;
                }
                $pTreeData['children'][$pClassData['package']]['children'][$pClassData['subpackage']]['children']['Classes']['children'][] = $classTreeChildren;
            } else {
                if(!isset($pTreeData['children'][$pClassData['package']]['children']['Classes'])) {
                    $pTreeData['children'][$pClassData['package']]['children']['Classes'] = $classTreeRoot;
                }
                $pTreeData['children'][$pClassData['package']]['children']['Classes']['children'][] = $classTreeChildren;
            }
        } else {
            if(!empty($pClassData['subpackage'])) { 
                if(!isset($pTreeData['children'][$pClassData['project']]['children'][$pClassData['package']]['children'][$pClassData['subpackage']]['children']['Classes'])) {
                    $pTreeData['children'][$pClassData['project']]['children'][$pClassData['package']]['children'][$pClassData['subpackage']]['children']['Classes'] = $classTreeRoot;
                }
                $pTreeData['children'][$pClassData['project']]['children'][$pClassData['package']]['children'][$pClassData['subpackage']]['children']['Classes']['children'][] = $classTreeChildren;
            } else {
                if(!isset($pTreeData['children'][$pClassData['project']]['children'][$pClassData['package']]['children']['Classes'])) {
                    $pTreeData['children'][$pClassData['project']]['children'][$pClassData['package']]['children']['Classes'] = $classTreeRoot;
                }
                $pTreeData['children'][$pClassData['project']]['children'][$pClassData['package']]['children']['Classes']['children'][] = $classTreeChildren;
            }
        }
    }
    
    /**
     * Adds a file to the treeview structure
     * @param array $FileData array containing information about the file
     * @param array $pTreeData array passed by reference containing the current tree structure 
     *
     * @return null, $pTreeData is passed by reference and is modified
     **/
    private function addFileToTreeView($pFileInfo, &$pTreeData) {
        $filesTreeRoot = array('type'=> "files",'text'=> 'Files','children'=> array(), 'ID' => md5(mt_rand()));
    
        $filesTreeView = array(
            'type'    => "file",
            'text'    => $pFileInfo['title'],
            'href'    => $pFileInfo['link'],
            'ID' => md5(mt_rand())
        );
        
        $this->createStructure($pFileInfo['project'], $pFileInfo['package'], $pFileInfo['subpackage'], $pTreeData);
        
        if(!empty($pFileInfo['project'])) {
            if(!empty($pFileInfo['subpackage'])) {
                if(!isset($pTreeData['children'][$pFileInfo['project']]['children'][$pFileInfo['package']]['children'][$pFileInfo['subpackage']]['children']['Files'])) {
                    $pTreeData['children'][$pFileInfo['project']]['children'][$pFileInfo['package']]['children'][$pFileInfo['subpackage']]['children']['Files'] = $filesTreeRoot;
                }
                $pTreeData['children'][$pFileInfo['project']]['children'][$pFileInfo['package']]['children'][$pFileInfo['subpackage']]['children']['Files']['children'][] = $filesTreeView;
            } else {
                if(!isset($pTreeData['children'][$pFileInfo['project']]['children'][$pFileInfo['package']]['children']['Files'])) {
                    $pTreeData['children'][$pFileInfo['project']]['children'][$pFileInfo['package']]['children']['Files'] = $filesTreeRoot;
                }
                $pTreeData['children'][$pFileInfo['project']]['children'][$pFileInfo['package']]['children']['Files']['children'][] = $filesTreeView;
            }
        } else {
            if(!empty($pFileInfo['subpackage'])) {
                if(!isset($pTreeData['children'][$pFileInfo['package']]['children'][$pFileInfo['subpackage']]['children']['Files'])) {
                    $pTreeData['children'][$pFileInfo['package']]['children'][$pFileInfo['subpackage']]['children']['Files'] = $filesTreeRoot;
                }
                $pTreeData['children'][$pFileInfo['package']]['children'][$pFileInfo['subpackage']]['children']['Files']['children'][] = $filesTreeView;
            } else {
                if(!isset($pTreeData['children'][$pFileInfo['package']]['children']['Files'])) {
                    $pTreeData['children'][$pFileInfo['package']]['children']['Files'] = $filesTreeRoot;
                }
                $pTreeData['children'][$pFileInfo['package']]['children']['Files']['children'][] = $filesTreeView;
            }
        }
    }
    
    var $topelements = array('errors', 'todo');
    private function SortTreeview($a, $b) {
        if(isset($a['children'])) {
            usort($a['children'], array($this, 'SortTreeview'));
        }
        if(isset($b['children'])) {
            usort($b['children'], array($this, 'SortTreeview'));
        }
        if(isset($a['type']) && in_array($a['type'], $this->topelements)) {
            if(!isset($b['type']) || !in_array($b['type'], $this->topelements)) { return -1; }
            return strcmp($a['text'], $b['text']);
        } elseif(isset($b['type']) && in_array($b['type'], $this->topelements)) {
            return 1;
        } else {
            if(isset($a['text']) && isset($b['text'])) {
                return strcmp($a['text'], $b['text']);
                } elseif(isset($a['text'])) {
                    return 1;
                } else {
                    return -1;
            }
        }
    }

    /**
     * Generate the treeview.html output files
     *
     * <p>
     *  This function creates the menu structure in the treeview.html file.
     *  The menu shows the projects, packages and subpackages
     * </p>
     */
    function formatLeftIndex() {
        phpDocumentor_out("\n");
        flush();
        $this->setTargetDir($this->base_dir);


        /** Root for the tree view **/
        $treeData = array(
            'ID'                => 'root',
            'type'              => 'root',
            'text'              => 'Api Documentation',
            'children'          => array(
                array(
                    'ID'                => 'errors',
                    'type'              => 'errors',
                    'text'              => 'Error List',
                    'href'               => 'errors.html',
                    'children'          => array()
                )
            )
        );

        if(count($this->todoList) > 0) {
            $treeData['children'][] = array(
                    'ID'                => 'todolist',
                    'type'              => 'todo',
                    'text'              => 'Todo List',
                    'href'               => 'todolist.html',
                    'children'          => array()
            );
        }

       
        /** Gets the data required for to make the treeview **/
        $pInfo = $this->makeLeft();
        
        /** start adding classes and files to the tree view **/
        foreach($pInfo as $package => $info) {
            if (!isset($this->pkg_elements[$package])) { continue; }
            
            foreach($info as $subpackage) {
                if (count($subpackage['classes'])) {
                    // Handle classes
                    foreach ($subpackage['classes'] as $class) {
                        $class['package'] = $package;
                        $class['subpackage'] = $subpackage['subpackage'];
                        $this->addClassToTreeView($class, $treeData);
                    }   
                }
                
                if (count($subpackage['files'])) {
                    // Handle files
                    foreach ($subpackage['files'] as $file) {
                        $file['package'] = $package;
                        $file['subpackage'] = $subpackage['subpackage'];
                        $this->addFileToTreeView($file, $treeData);
                    }   
                }
            }
        }
        usort($treeData['children'], array($this, 'SortTreeview'));
        
        // Create class tree page
        $template = &$this->newSmarty();
        $template->assign("menuitem", $treeData);
        $template->register_outputfilter('HTMLSmartyJEvolve_outputfilter');
        $this->writefile("treemenu.html",$template->fetch('menuitem.tpl'));
        phpDocumentor_out("\n");
        flush();

        // free up considerable memory
        unset($this->elements);
        unset($this->pkg_elements);
    }
    
    /**
     * This function takes an {@link abstractLink} descendant and returns an html link
     *
     * @param abstractLink a descendant of abstractlink should be passed, and never text
     * @param string text to display in the link
     * @param boolean this parameter is not used, and is deprecated
     * @param boolean determines whether the returned text is enclosed in an <a> tag
     */
    function returnSee(&$element, $eltext = false, $with_a = true)
    {
        if (!is_object($element) || !$element) return false;
        if (!$with_a) return $this->getId($element, false);
        if (!$eltext)
        {
            $eltext = '';
            switch($element->type)
            {
                case 'tutorial' :
                $eltext = strip_tags($element->title);
                break;
                case 'method' :
                case 'var' :
                case 'const' :
                $eltext .= $element->class.'::';
                case 'page' :
                case 'define' :
                case 'class' :
                case 'function' :
                case 'global' :
                default :
                $eltext .= $element->name;
                if ($element->type == 'function' || $element->type == 'method') $eltext .= '()';
                break;
            }
        }
        return '<a rel="contents" href="'.$this->getId($element).'">'.$eltext.'</a>';
    }
    
    function getId($element, $fullpath = true){
        if(is_string($element)) { return; }
        if (phpDocumentor_get_class($element) == 'parserdata') {
            $element = $this->addLink($element->parent);
            $elp = $element->parent;
        } elseif (is_a($element, 'parserbase')) {
            $elp = $element;
            $element = $this->addLink($element);
        }
        $c = '';
        if (!empty($element->subpackage))  {
            $c = '/'.$element->subpackage;
        }
        $b = '{$subdir}';
        switch ($element->type) {
            case 'page' :
                if ($fullpath) {
                    return $b.$element->package.$c.'/'.$element->fileAlias.'.html';
                }
                return 'top';
            case 'define' :
            case 'global' :
            case 'function' :
                if ($fullpath) {
                    return $b.$element->package.$c.'/'.$element->fileAlias.'.html#'.$element->type.$element->name;
                }
                return $element->type.$element->name;
            case 'class' :
                if ($fullpath) {
                    return $b.$element->package.$c.'/'.$element->name.'.html';
                }   
                return 'top';
            case 'method' :
            case 'var' :
            case 'const' :
                if ($fullpath) {
                    return $b.$element->package.$c.'/'.$element->class.'.html#'.$element->type.$element->name;
                }
                return $element->type.$element->name;
            case 'tutorial' :
                $d = '';
                if ($element->section) {
                    $d = '#'.$element->section;
                }
                return $b.$element->package.$c.'/tutorial_'.$element->name.'.html'.$d;
        }
    }
    
    /**
     * Convert README/INSTALL/CHANGELOG file contents to output format
     * @param README|INSTALL|CHANGELOG
     * @param string contents of the file
     */
    function Convert_RIC($name, $contents) {
        $template = &$this->newSmarty();
        $template->assign('contents',$contents);
        $template->assign('name',$name);
        $this->setTargetDir($this->base_dir);
        $this->writefile('ric_'.$name . '.html',$template->fetch('ric.tpl'));
        $this->ric_set[$name] = true;
    }
    
    function ConvertTodoList() {
        $todolist = array();
        foreach($this->todoList as $package => $alltodos) {
            foreach($alltodos as $todos) {
                $converted = array();
                $converted['link'] = $this->returnSee($todos[0]);
                if (!is_array($todos[1]))  {
                    $converted['todos'][] = $todos[1]->Convert($this);
                } else {
                    foreach($todos[1] as $todo) {
                        $converted['todos'][] = $todo->Convert($this);
                    }
                }
                $todolist[$package][] = $converted;
            }
        }
        $templ = &$this->newSmarty();
        $templ->assign('todos',$todolist);
        $templ->register_outputfilter('HTMLSmartyJEvolve_outputfilter');
        $this->setTargetDir($this->base_dir);
        $this->writefile('todolist.html',$templ->fetch('todolist.tpl'));
    }
    
    /**
     * Create errors.html template file output
     *
     * This method takes all parsing errors and warnings and spits them out ordered by file and line number.
     * @global ErrorTracker We'll be using it's output facility
     */
    function ConvertErrorLog() {
        global $phpDocumentor_errors;
        $allfiles = array();
        $files = array();
        $warnings = $phpDocumentor_errors->returnWarnings();
        $errors = $phpDocumentor_errors->returnErrors();
        $template = &$this->newSmarty();
        foreach($warnings as $warning) {
            $file = '##none';
            $linenum = 'Warning';
            if ($warning->file) {
                $file = $warning->file;
                $allfiles[$file] = 1;
                $linenum .= ' on line '.$warning->linenum;
            }
            $files[$file]['warnings'][] = array('name' => $linenum, 'listing' => $warning->data);
        }
        foreach($errors as $error)  {
            $file = '##none';
            $linenum = 'Error';
            if ($error->file) {
                $file = $error->file;
                $allfiles[$file] = 1;
                $linenum .= ' on line '.$error->linenum;
            }
            $files[$file]['errors'][] = array('name' => $linenum, 'listing' => $error->data);
        }
        $i=1;
        $af = array();
        foreach($allfiles as $file => $num) {
            $af[$i++] = $file;
        }
        $allfiles = $af;
        usort($allfiles,'strnatcasecmp');
        $allfiles[0] = "Post-parsing";
        foreach($allfiles as $i => $a)  {
            $allfiles[$i] = array('file' => $a);
        }
        $out = array();
        foreach($files as $file => $data)  {
            if ($file == '##none') $file = 'Post-parsing';
            $out[$file] = $data;
        }
        
        $template->assign("files",$allfiles);
        $template->assign("all",$out);
        $template->assign("title","phpDocumentor Parser Errors and Warnings");
        
        $this->setTargetDir($this->base_dir);
        $this->writefile("errors.html",$template->fetch('errors.tpl'));
        unset($template);
       
       phpDocumentor_out("\n\nTo view errors and warnings, look at ".$this->base_dir. PATH_DELIMITER . "errors.html\n");
        flush();
    }
    
    function getTutorialId($package,$subpackage,$tutorial,$id)  {
        return $id;
    }
    
    function getCData($value) { return '<pre>'.htmlentities($value).'</pre>';  }
    
    /**
     * Converts package page and sets its package as used in {@link $package_pages}
     * @param parserPackagePage
     */
    function convertPackagepage(&$element) {
        phpDocumentor_out("\n");
        flush();
        $this->package = $element->package;
        $this->subpackage = '';
        $contents = $element->Convert($this);
        $this->package_pages[$element->package] = str_replace('{$subdir}', '',$contents);
        phpDocumentor_out("\n");
        flush();
        $this->setTargetDir($this->base_dir . PATH_DELIMITER . $element->package);
        $this->writeFile('package_'.$element->package.'.html',str_replace('{$subdir}','',$contents));
    }
    
    /**
     * @param parserTutorial
     */
    function convertTutorial(&$element) {
    /*
      THIS WAS THE Evolve function
      Due to HTMLSmartyConverter way of returning data,
      we have to rewrite whole function.
      This piece of code is here to keep track while refactoring.

        phpDocumentor_out("\n");
        flush();
        $template = &parent::convertTutorial($element);
        $template->register_outputfilter('HTMLSmartyJEvolve_outputfilter');
        $contents = $template->fetch('tutorial.tpl');
        $a = '';
        if ($element->subpackage) $a = PATH_DELIMITER . $element->subpackage;
        phpDocumentor_out("\n");
        flush();
        $this->setTargetDir($this->base_dir . PATH_DELIMITER . $element->package . $a);
        $this->writeFile('tutorial_'.$element->name.'.html',$contents);
*/
        phpDocumentor_out("\n");
        flush();

        $this->package = $element->package;
        $this->subpackage = $element->subpackage;
        $content = $element->Convert($this);
        $template = &$this->newSmarty();
        $template->assign('contents',$content);
        $template->assign('title',$element->getTitle($this));
        $template->assign('nav',$element->parent || $element->prev || $element->next);
        if ($element->parent)
        {
            $template->assign('up',$this->getId($element->parent));
            $template->assign('uptitle',$element->parent->title);
        }
        if ($element->prev)
        {
            $template->assign('prev',$this->getId($element->prev));
            $template->assign('prevtitle',$element->prev->title);
        }
        if ($element->next)
        {
            $template->assign('next',$this->getId($element->next));
            $template->assign('nexttitle',$element->next->title);
        }

        $classleft = $this->getClassLeft();
        $template->assign("compiledfileindex",$this->getPageLeft());
        $template->assign("compiledclassindex",$classleft['class']);
        $template->assign("compiledinterfaceindex",$classleft['interface']);
        $template->assign("tutorials",$this->getTutorialList());
        $template->assign("title",strip_tags($element->getTitle($this)));
        $contents = $element->Convert($this);
        if ($element->name == $this->package . '.pkg')
        {
            $this->package_pages[$element->package] = $contents;
        }
        $a = '../';
        if (!empty($element->subpackage)) $a .= $a;
        $template->assign("subdir",$a);
        $a = '';
        if ($element->subpackage) $a = PATH_DELIMITER . $element->subpackage;
        $template->register_outputfilter('HTMLSmarty_outputfilter');
        $template->register_outputfilter('HTMLSmartyJEvolve_outputfilter');
        $this->setTargetDir($this->base_dir . PATH_DELIMITER . $element->package . $a);
        $this->writeFile('tutorial_'.$element->name.'.html',$template->fetch('tutorial.tpl'));
        if ($element->name == $element->package . '.pkg')
        {
            phpDocumentor_out("\n");
            flush();
            // package-level docs
            $this->setTargetDir($this->base_dir);
            $template->assign("subdir",'');
            $this->writeFile('li_'.$element->package.'.html',$template->fetch('tutorial.tpl'));
        }
        unset($template);
    }
    
    /**
     * Converts class for template output
     * @see prepareDocBlock(), generateChildClassList(), generateFormattedClassTree(), getFormattedConflicts()
     * @see getFormattedInheritedMethods(), getFormattedInheritedVars()
     * @param parserClass
     */
    function convertClass(&$element)  {
        parent::convertClass($element);
        $this->class_dir = $element->docblock->package;
        if (!empty($element->docblock->subpackage)) $this->class_dir .= PATH_DELIMITER . $element->docblock->subpackage;

        $this->class_data->assign('subdir', '');
        $this->class_data->assign("title","Docs For Class " . $element->getName());
        $this->class_data->assign("page",$element->getName() . '.html');
    }

    /**
     * Converts class variables for template output
     * @see prepareDocBlock(), getFormattedConflicts()
     * @param parserDefine
     */
    function convertVar(&$element) {
        parent::convertVar($element, array('var_dest' => $this->getId($element,false)));
    }

    /**
     * Converts class variables for template output
     * @see prepareDocBlock(), getFormattedConflicts()
     * @param parserDefine
     */
    function convertConst(&$element)  {
        parent::convertConst($element, array('const_dest' => $this->getId($element,false)));
    }

    /**
     * Converts class methods for template output
     * @see prepareDocBlock(), getFormattedConflicts()
     * @param parserDefine
     */
    function convertMethod(&$element) {
        parent::convertMethod($element, array('method_dest' => $this->getId($element,false)));
    }
    
    /**
     * Converts function for template output
     * @see prepareDocBlock(), parserFunction::getFunctionCall(), getFormattedConflicts()
     * @param parserFunction
     */
    function convertFunction(&$element) {
        $funcloc = $this->getId($this->addLink($element));
        parent::convertFunction($element,array('function_dest' => $this->getId($element,false)));
    }
    
    /**
     * Converts include elements for template output
     * @see prepareDocBlock()
     * @param parserInclude
     */
    function convertInclude(&$element) {
        parent::convertInclude($element, array('include_file'    => '_'.strtr($element->getValue(),array('"' => '', "'" => '','.' => '_'))));
    }
    
    /**
     * Converts defines for template output
     * @see prepareDocBlock(), getFormattedConflicts()
     * @param parserDefine
     */
    function convertDefine(&$element) {
        parent::convertDefine($element, array('define_link' => $this->getId($element,false)));
    }
    
    /**
     * Converts global variables for template output
     * @param parserGlobal
     */
    function convertGlobal(&$element) {
        parent::convertGlobal($element, array('global_link' => $this->getId($element,false)));
    }
    
    /**
     * converts procedural pages for template output
     * @see prepareDocBlock(), getClassesOnPage()
     * @param parserData
     */
    function convertPage(&$element) {
        parent::convertPage($element);
        $this->juststarted = true;
        $this->page_dir = $element->parent->package;
        if (!empty($element->parent->subpackage)) $this->page_dir .= PATH_DELIMITER . $element->parent->subpackage;
        // registering stuff on the template
        $this->page_data->assign("page",$this->getPageName($element) . '.html');
        $this->page_data->assign("title","Docs for page ".$element->parent->getFile());
    }
    
    function getPageName(&$element) {
        if (phpDocumentor_get_class($element) == 'parserpage') return '_'.$element->getName();
        return '_'.$element->parent->getName();
    }

    /**
     * returns an array containing the class inheritance tree from the root object to the class
     *
     * @param parserClass    class variable
     * @return array Format: array(root,child,child,child,...,$class)
     * @uses parserClass::getParentClassTree()
     */
    
    function generateFormattedClassTree($class) {
        $tree = $class->getParentClassTree($this);
        $out = '';
        if (count($tree) - 1) {
            $result = array($class->getName());
            $parent = $tree[$class->getName()];
            $distance[] = '';
            while ($parent)  {
                $x = $parent;
                if (is_object($parent))  {
                    $subpackage = $parent->docblock->subpackage;
                    $package = $parent->docblock->package;
                    $x = $parent;
                    $x = $parent->getLink($this);
                    if (!$x) $x = $parent->getName();
                }
                $result[] = $x;
                $distance[] = "\n%s|\n %s--";
                if (is_object($parent))
                $parent = $tree[$parent->getName()];
                elseif (isset($tree[$parent]))
                $parent = $tree[$parent];
            }
            $nbsp = '   ';
            for($i=count($result) - 1;$i>=0;$i--)
            {
                $my_nbsp = '';
                for($j=0;$j<count($result) - $i;$j++) $my_nbsp .= $nbsp;
                $distance[$i] = sprintf($distance[$i],$my_nbsp,$my_nbsp);
            }
            return array('classes'=>array_reverse($result),'distance'=>array_reverse($distance));
        } else
        {
            return array('classes'=>$class->getName(),'distance'=>array(''));
        }
    }
    
    /** @access private */
    function sortVar($a, $b) {
        return strnatcasecmp($a->getName(),$b->getName());
    }
    
    /** @access private */
    function sortMethod($a, $b)  {
        if ($a->isConstructor) return -1;
        if ($b->isConstructor) return 1;
        return strnatcasecmp($a->getName(),$b->getName());
    }

    /**
     * returns a template-enabled array of class trees
     * 
     * @param    string    $package    package to generate a class tree for
     * @see $roots, HTMLConverter::getRootTree()
     */
    function generateFormattedClassTrees($package)  {
        if (!isset($this->roots['normal'][$package]) &&
              !isset($this->roots['special'][$package])) {
            return array();
        }
        $trees = array();
        if (isset($this->roots['normal'][$package])) {
            $roots = $this->roots['normal'][$package];
            for($i=0;$i<count($roots);$i++)
            {
                $root = $this->classes->getClassByPackage($roots[$i], $package);
                if ($root && $root->isInterface()) {
                    continue;
                }
                $trees[] = array('class' => $roots[$i],'class_tree' => "<ul>\n".$this->getRootTree($this->getSortedClassTreeFromClass($roots[$i],$package,''),$package)."</ul>\n");
            }
        }
        if (isset($this->roots['special'][$package])) {
            $roots = $this->roots['special'][$package];
            foreach ($roots as $parent => $classes) {
                $thistree = '';
                foreach ($classes as $classinfo) {
                    $root = $this->classes->getClassByPackage($classinfo, $package);
                    if ($root && $root->isInterface()) {
                        continue;
                    }
                    $thistree .=
                        $this->getRootTree(
                            $this->getSortedClassTreeFromClass(
                                $classinfo,
                                $package,
                                ''),
                            $package,
                            true);
                }
                if (!$thistree) {
                    continue;
                }
                $trees[] = array(
                    'class' => $parent,
                    'class_tree' => "<ul>\n" . $thistree . "</ul>\n"
                );
            }
        }
        return $trees;
    }

    /**
     * returns a template-enabled array of interface inheritance trees
     * 
     * @param    string    $package    package to generate a class tree for
     * @see $roots, HTMLConverter::getRootTree()
     */
    function generateFormattedInterfaceTrees($package)
    {
        if (!isset($this->roots['normal'][$package]) &&
              !isset($this->roots['special'][$package])) {
            return array();
        }
        $trees = array();
        if (isset($this->roots['normal'][$package])) {
            $roots = $this->roots['normal'][$package];
            for($i=0;$i<count($roots);$i++)
            {
                $root = $this->classes->getClassByPackage($roots[$i], $package);
                if ($root && !$root->isInterface()) {
                    continue;
                }
                $trees[] = array('class' => $roots[$i],'class_tree' => "<ul>\n".$this->getRootTree($this->getSortedClassTreeFromClass($roots[$i],$package,''),$package)."</ul>\n");
            }
        }
        if (isset($this->roots['special'][$package])) {
            $roots = $this->roots['special'][$package];
            foreach ($roots as $parent => $classes) {
                $thistree = '';
                foreach ($classes as $classinfo) {
                    $root = $this->classes->getClassByPackage($classinfo, $package);
                    if ($root && !$root->isInterface()) {
                        continue;
                    }
                    $thistree .=
                        $this->getRootTree(
                            $this->getSortedClassTreeFromClass(
                                $classinfo,
                                $package,
                                ''),
                            $package,
                            true);
                }
                if (!$thistree) {
                    continue;
                }
                $trees[] = array(
                    'class' => $parent,
                    'class_tree' => "<ul>\n" . $thistree . "</ul>\n"
                );
            }
        }
        return $trees;
    }
    
    /**
     * return formatted class tree for the Class Trees page
     *
     * @param array $tree output from {@link getSortedClassTreeFromClass()}
     * @param string $package  package
     * @param boolean $nounknownparent if true, an object's parent will not be checked
     * @see Classes::$definitechild, generateFormattedClassTrees()
     * @return string
     */
    function getRootTree($tree, $package, $noparent = false)
    {
        if (!$tree) return '';
        $my_tree = '';
        $cur = '#root';
        $lastcur = array(false);
        $kids = array();
        $dopar = false;
        if (!$noparent && $tree[$cur]['parent'])
        {
            $dopar = true;
            if (!is_object($tree[$cur]['parent']))
            {
//                debug("parent ".$tree[$cur]['parent']." not found");
                $my_tree .= '<li>' . $tree[$cur]['parent'] .'<ul>';
            }
            else
            {
//                        debug("parent ".$this->returnSee($tree[$cur]['parent'])." in other package");
                $root = $this->classes->getClassByPackage($tree[$cur]['parent']->name,
                    $package);
                $my_tree .= '<li>' . $this->returnSee($tree[$cur]['parent']);
                if ($tree[$cur]['parent']->package != $package) $my_tree .= ' <b>(Different package)</b><ul>';
            }
        }
        do
        {
//            fancy_debug($cur,$lastcur,$kids);
            if (count($tree[$cur]['children']))
            {
//                debug("$cur has children");
                if (!isset($kids[$cur]))
                {
//                    debug("set $cur kids");
                    $kids[$cur] = 1;
                    $root = $this->classes->getClassByPackage(
                        $tree[$cur]['link']->name,
                        $tree[$cur]['link']->package);
                    if ($implements = $root->getImplements()) {
                        $my_tree .= '<li>'.$this->returnSee($tree[$cur]['link']) .
                            ' (implements ';
                        foreach ($implements as $i => $interface) {
                            if ($i && $i != count($implements) - 1) $my_tree .= ', ';
                            if ($link = $this->getLink('object ' .  $interface)) {
                                $my_tree .= $this->returnSee($link);
                            } else {
                                $my_tree .= $interface;
                            }
                        }
                        $my_tree .= ')';
                    } else {
                        $my_tree .= '<li>'.$this->returnSee($tree[$cur]['link']);
                    }
                    $my_tree .= '<ul>'."\n";
                }
                array_push($lastcur,$cur);
                list(,$cur) = each($tree[$cur]['children']);
//                var_dump('listed',$cur);
                if ($cur)
                {
                    $cur = $cur['package'] . '#' . $cur['class'];
//                    debug("set cur to child $cur");
//                    $my_tree .= '<li>'.$this->returnSee($tree[$cur]['link']);
                    continue;
                } else
                {
//                    debug("end of children for $cur");
                    $cur = array_pop($lastcur);
                    $cur = array_pop($lastcur);
                    $my_tree .= '</ul></li>'."\n";
                    if ($dopar && ($cur == '#root' || !$cur)) $my_tree .= '</ul></li>';
                }
            } else 
            {
//                debug("$cur has no children");
                $my_tree .= '<li>'.$this->returnSee($tree[$cur]['link'])."</li>";
                if ($dopar && $cur == '#root') $my_tree .= '</ul></li>';
                $cur = array_pop($lastcur);
            }
        } while ($cur);
        return $my_tree;
    }

    /**
     * Generate indexing information for given element
     * 
     * @param parserElement descendant of parserElement
     * @see generateElementIndex()
     * @return array
     */
    function getIndexInformation($elt)
    {
        $Result['type'] = $elt->type;
        $Result['file_name'] = $elt->file;
        $Result['path'] = $elt->getPath();
        
        if (isset($elt->docblock))
					{
						$Result['description'] = $elt->docblock->getSDesc($this);
						
						if ($elt->docblock->hasaccess)
							$Result['access'] = $elt->docblock->tags['access'][0]->value;
						else
							$Result['access'] = 'public';

						$Result['abstract'] = isset ($elt->docblock->tags['abstract'][0]);
					}
        else
            $Result['description'] = '';
        
        $aa = $Result['description'];
        if (!empty($aa)) $aa = "<br>&nbsp;&nbsp;&nbsp;&nbsp;$aa";

        switch($elt->type)
        {
                case 'class':
                        $Result['name'] = $elt->getName();
                        $Result['title'] = 'Class';
                        $Result['link'] = $this->getClassLink($elt->getName(),
                                                              $elt->docblock->package,
                                                              $elt->getPath(),
                                                              $elt->getName());
                        $Result['listing'] = 'in file '.$elt->file.', class '.$Result['link']."$aa";
                break;
                case 'define':
                        $Result['name'] = $elt->getName();
                        $Result['title'] = 'Constant';
                        $Result['link'] = $this->getDefineLink($elt->getName(),
                                                               $elt->docblock->package,
                                                               $elt->getPath(),
                                                               $elt->getName());
                        $Result['listing'] = 'in file '.$elt->file.', constant '.$Result['link']."$aa";
                break;
                case 'global':
                        $Result['name'] = $elt->getName();
                        $Result['title'] = 'Global';
                        $Result['link'] = $this->getGlobalLink($elt->getName(),
                                                               $elt->docblock->package,
                                                               $elt->getPath(),
                                                               $elt->getName());
                        $Result['listing'] = 'in file '.$elt->file.', global variable '.$Result['link']."$aa";
                break;
                case 'function':
                        $Result['name'] = $elt->getName();
                        $Result['title'] = 'Function';
                        $Result['link'] = $this->getFunctionLink($elt->getName(),
                                                                 $elt->docblock->package,
                                                                 $elt->getPath(),
                                                                 $elt->getName().'()');
                        $Result['listing'] = 'in file '.$elt->file.', function '.$Result['link']."$aa";
                break;
                case 'method':
                        $Result['name'] = $elt->getName();
                        $Result['title'] = 'Method';
                        $Result['link'] = $this->getMethodLink($elt->getName(),
                                                               $elt->class,
                                                               $elt->docblock->package,
                                                               $elt->getPath(),
                                                               $elt->class.'::'.$elt->getName().'()'
                                                                         );
													if ($elt->isConstructor) $Result['constructor'] = 1;
                        $Result['listing'] = 'in file '.$elt->file.', method '.$Result['link']."$aa";
                break;
                case 'var':
                        $Result['name'] = $elt->getName();
                        $Result['title'] = 'Variable';
                        $Result['link'] = $this->getVarLink($elt->getName(),
                                                            $elt->class,
                                                            $elt->docblock->package,
                                                            $elt->getPath(),
                                                            $elt->class.'::'.$elt->getName());
                        $Result['listing'] = 'in file '.$elt->file.', variable '.$Result['link']."$aa";
                break;
                case 'const':
                        $Result['name'] = $elt->getName();
                        $Result['title'] = 'Class Constant';
                        $Result['link'] = $this->getConstLink($elt->getName(),
                                                            $elt->class,
                                                            $elt->docblock->package,
                                                            $elt->getPath(),
                                                            $elt->class.'::'.$elt->getName());
                        $Result['listing'] = 'in file '.$elt->file.', class constant '.$Result['link']."$aa";
                break;
                case 'page':
                        $Result['name'] = $elt->getFile();
                        $Result['title'] = 'Page';
                        $Result['link'] = $this->getPageLink($elt->getFile(),
                                                             $elt->package,
                                                             $elt->getPath(),
                                                             $elt->getFile());
                        $Result['listing'] = 'procedural page '.$Result['link'];
                break;
                case 'include':
                        $Result['name'] = $elt->getName();
                        $Result['title'] = 'Include';
                        $Result['link'] = $elt->getValue();
                        $Result['listing'] = 'include '.$Result['name'];
                break;
        }

        return $Result;
    }
    /**
     * Generate alphabetical index of all elements
     *
     * @see $elements, walk()
     */
    function generateElementIndex() {
        $elementindex = array();
        $letters = array();
        $used = array();
        foreach($this->elements as $letter => $nutoh) {
            foreach($this->elements[$letter] as $i => $yuh) {
                if ($this->elements[$letter][$i]->type != 'include') {
                    if (!isset($used[$letter])) {
                        $letters[]['letter'] = $letter;
                        $elindex['letter'] = $letter;
                        $used[$letter] = 1;
                    }
                    $elindex['index'][] = $this->getIndexInformation($this->elements[$letter][$i]);
                }
            }
            if (isset($elindex['index'])) {
                $elementindex[] = $elindex;
            } else {
                unset($letters[count($letters) - 1]);
            }
            $elindex = array();
        }
        return array($elementindex,$letters);
    }
    
    function copyMediaRecursively($media,$targetdir,$subdir = '') {
        $versionControlDirectories = array ('CVS', 'media/CVS', 'media\\CVS', '.svn', 'media/.svn', 'media\\.svn');
        if (!is_array($media)) { return; }
        foreach($media as $dir => $files) {
            if ($dir === '/') {
                $this->copyMediaRecursively($files,$targetdir);
            } else {
                if (!is_numeric($dir)) {
                    if (in_array($dir, $versionControlDirectories)) {
                        // skip it entirely
                    } else {
                        // create the subdir
                        phpDocumentor_out("creating $targetdir" . PATH_DELIMITER . "$dir\n");
                        Converter::setTargetDir($targetdir . PATH_DELIMITER . $dir);
                        if (!empty($subdir)) 
                        {
                            $subdir .= PATH_DELIMITER;
                        }
                        $this->copyMediaRecursively($files,"$targetdir/$dir",$subdir . $dir);
                    }
                } else {
                    // copy the file
                    phpDocumentor_out("copying $targetdir" . PATH_DELIMITER . $files['file']."\n");
                    $this->copyFile($files['file'],$subdir);
                }
            }
        }
    }
    
    /**
     * calls the converter setTargetDir, and then copies any template images and the stylesheet if they haven't been copied
     * @see Converter::setTargetDir()
     */
    function setTargetDir($dir) {
        Converter::setTargetDir($dir);
        if ($this->wrote) return;
        $this->wrote = true;
        $template_images = array();
        $stylesheets = array();
        $tdir = $dir;
        $dir = $this->templateDir;
        $this->templateDir = $this->templateDir.'templates/';
        $info = new Io;
        $this->copyMediaRecursively($info->getDirTree($this->templateDir.'images',$this->templateDir),$tdir);
        $this->copyMediaRecursively($info->getDirTree($this->templateDir.'styles',$this->templateDir),$tdir);
        $this->copyMediaRecursively($info->getDirTree($this->templateDir.'scripts',$this->templateDir),$tdir);
    }
        
    /** 
     * 
     * Because of the added project tag that's not known by phpdocumentor we must 'hack' it a bit.
     * When classes are added, search for the tag 'project' and add it at the class link(same place as package/subpackage)
     * This allows us to later generate a nice treeview with the project name as root.
     *
     * @see Converter::addElement
     */
    function addElement(&$element,$pageel=false) {
        parent::addElement($element, $pageel);
        
        switch($element->type) {
            case 'class' :
                if(isset($element->docblock->tags['project'][0]->value[0]->value[0])) {
                    for($i=0; $i < count($this->class_elements[$element->docblock->package][$element->docblock->subpackage]) ; $i++) {
                        if($this->class_elements[$element->docblock->package][$element->docblock->subpackage][$i]->path == $element->path) {
                            $this->class_elements[$element->docblock->package][$element->docblock->subpackage][$i]->project = $element->docblock->tags['project'][0]->value[0]->value[0];
                        }
                    }
                }
                break;
        }
    }
    
    
    /** 
     * 
     * Adding new page lvl tags is a bitch, we must use the _createPkgElements to loop over the $this->page_elements 
     * already generated by the parent method and fill up the project value in the page link objects
     * Doing this allows us to later generate a nice treeview
     *
     * @see Converter::_createPkgElements
     */
    function _createPkgElements(&$pages) {
        parent::_createPkgElements($pages);
        
        foreach($pages as $file => $info) {
            if(!isset($info->docblock->tags['project'])) { continue; }
            
            $package = $info->parent->package;
            $subpackage = $info->parent->subpackage;
            for($i = 0; $i < count($this->page_elements[$package][$subpackage]); $i++) {
                if($this->page_elements[$package][$subpackage][$i]->path == $file) {
                    $this->page_elements[$package][$subpackage][$i]->project = $info->docblock->tags['project'][0]->value[0]->value[0];
                }
            }
        }
    }
}

/** @access private */
function HTMLSmartyJEvolve_outputfilter($src, &$smarty) { return str_replace('{$subdir}', '', $src); }
?>
