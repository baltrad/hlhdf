#!/usr/bin/perl

my%FUNCTIONMAP=(
   # Node list
   "newHL_NodeList" => "HLNodeList_new",
   "freeHL_NodeList" => "HLNodeList_free",
   "setHL_NodeListFileName" => "HLNodeList_setFileName",
   "getHL_NodeListFileName" => "HLNodeList_getFileName",
   "getHL_NodeListNumberOfNodes" => "HLNodeList_getNumberOfNodes",
   "getHL_NodeListNodeByIndex" => "HLNodeList_getNodeByIndex",
   "markHL_NodeListNodes" => "HLNodeList_markNodes",
   "addHL_Node" => "HLNodeList_addNode",
   "getHL_Node" => "HLNodeList_getNodeByName",
   "findCompoundTypeDescription" => "HLNodeList_findCompoundDescription",
   "findHL_CompoundTypeDescription" => "HLNodeList_findCompoundDescription",
   
   # Node
   "newHL_Node" => "HLNode_new",
   "freeHL_Node" => "HLNode_free",
   "newHL_Group" => "HLNode_newGroup",
   "newHL_Attribute" => "HLNode_newAttribute",
   "newHL_Dataset" => "HLNode_newDataset",
   "newHL_Datatype" => "HLNode_newDatatype",
   "newHL_Reference" => "HLNode_newReference",
   "copyHL_Node" => "HLNode_copy",
   "setHL_NodeScalarValue" => "HLNode_setScalarValue",
   "setHL_NodeArrayValue" => "HLNode_setArrayValue",
   "getHL_NodeName" => "HLNode_getName",
   "getHL_NodeDataSize" => "HLNode_getDataSize",
   "getHL_NodeRawdataSize" => "HLNode_getRawdataSize",
   "equalsHL_NodeName" => "HLNode_nameEquals",
   "setHL_NodeMark" => "HLNode_setMark",
   "getHL_NodeMark" => "HLNode_getMark",
   "getHL_NodeType" => "HLNode_getType",
   "setHL_NodeDataType" => "HLNode_setDataType",
   "getHL_NodeDataType" => "HLNode_getDataType",
   "setHL_NodeDimensions" => "HLNode_setDimensions",
   "getHL_NodeDimensions" => "HLNode_getDimensions",
   "getHL_NodeRank" => "HLNode_getRank",
   "getHL_NodeDimension" => "HLNode_getDimension",
   "getHL_NodeNumberOfPoints" => "HLNode_getNumberOfPoints",
   "setHL_NodeCompoundDescription" => "HLNode_setCompoundDescription",
   "getHL_NodeCompoundDescription" => "HLNode_getCompoundDescription",
   "setHL_NodeCompression" => "HLNode_setCompression",
   "getHL_NodeCompression" => "HLNode_getCompression",
   "getHL_NodeData" => "HLNode_getData",       # Needs to be down here since otherwise, getHL_NodeData will be executed before getHL_NodeDataSize, etc.
   "getHL_NodeRawdata" => "HLNode_getRawdata", # Needs to be down here since otherwise, getHL_NodeRawdata will be executed before getHL_NodeRawdataSize, etc.
   "commitHL_Datatype" => "HLNode_commitType",
   
   # Write
   "writeHL_NodeList" => "HLNodeList_write",
   "writeNodeList" => "HLNodeList_write",
   "updateHL_NodeList" => "HLNodeList_update",
   "updateNodeList" => "HLNodeList_update",

   # Read
   "readHL_NodeListFrom" => "HLNodeList_readFrom",
   "readHL_NodeList" => "HLNodeList_read",
   "deselectNode" => "HLNodeList_deselectNode",
   "selectNode" => "HLNodeList_selectNode",
   "selectAllNodes" => "HLNodeList_selectAllNodes",
   "selectMetadataNodes" => "HLNodeList_selectMetadataNodes",
   "fetchMarkedNodes" => "HLNodeList_fetchMarkedNodes",
   "fetchNode" => "HLNodeList_fetchNode",   
   
   # Base
   "disableErrorReporting" => "HL_disableErrorReporting",
   "enableErrorReporting" => "HL_enableErrorReporting",
   "initHlHdf" => "HL_init",
   "debugHlHdf" => "HL_setDebugMode",
   "isHdf5File" => "HL_isHDF5File",
   "whatSizeIsHdfFormat" => "HL_sizeOfFormat",
   "isFormatSupported" => "HL_isFormatSupported",
   
   # File creation property
   "createHlHdfFileCreationProperty" => "HLFileCreationProperty_new",
   "freeHL_fileCreationProperty" => "HLFileCreationProperty_free",
   
   # Compression
   "newHL_Compression" => "HLCompression_new",
   "dupHL_Compression" => "HLCompression_clone",
   "initHL_Compression" => "HLCompression_init",
   "freeHL_Compression" => "HLCompression_free"
   
   );

if ($#ARGV != 0) {
  print "Usage rename_functions.pl <filename>\n";
  exit(255);
}

die unless open (FILEFP, "<$ARGV[0]");

while (<FILEFP>) {
   while (($k, $v) = each(%FUNCTIONMAP)) {
      $_ =~ s/([^A-Za-z0-9_])$k(\s*[\\$|\\(])/$1$v$2/g;
   }
   print $_;
}
