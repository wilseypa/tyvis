// Copyright (c) 2003-2004 The University of Cincinnati.  
// All rights reserved.
// 
// UC MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
// OR NON-INFRINGEMENT.  UC SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY
// LICENSEE AS A RESULT OF USING, RESULT OF USING, MODIFYING OR DISTRIBUTING
// THIS SOFTWARE OR ITS DERIVATIVES.
// 
// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the U.S.,
// and the terms of this license.
// 
// You may modify, distribute, and use the software contained in this
// package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
// version 2, June 1991. A copy of this license agreement can be found in
// the file "LGPL", distributed with this archive.
// 
// Authors: Philip A. Wilsey	philip.wilsey@ieee.org
// 	    Dale E. Martin	dmartin@cliftonlabs.com
// 
// --------------------------------------------------------------------------

// Classes we instantiate
#include "iir_headers.hh"
#include "TyvisStandardPackage.hh"
#include "savant/IRKind.hh"
#include "savant/error_func.hh"
#include "savant/ScramStandardPackage.hh"

// Implementation details
#include "tyvis_plugin_class_factory.hh"

tyvis_plugin_class_factory *
tyvis_plugin_class_factory::instance(){
  static tyvis_plugin_class_factory *_instance = new tyvis_plugin_class_factory();
  return _instance;
}

IIR *
tyvis_plugin_class_factory::get_new_class(IIR_Kind iir_type) {
  IIR *retv = NULL;
  switch (iir_type) {
  case IIR_ABSOLUTE_OPERATOR:
    retv = new TyvisAbsoluteOperator();
    break;     

  case IIR_ACCESS_SUBTYPE_DEFINITION:
    retv = new TyvisAccessSubtypeDefinition();
    break;     

  case IIR_ACCESS_TYPE_DEFINITION:
    retv = new TyvisAccessTypeDefinition();
    break;     

  case IIR_ACROSS_ATTRIBUTE:
    retv = new TyvisAcrossAttribute();
    break;     

  case IIR_ACTIVE_ATTRIBUTE:
    retv = new TyvisActiveAttribute();
    break;     

  case IIR_ADDITION_OPERATOR:
    retv = new TyvisAdditionOperator();
    break;     

  case IIR_AGGREGATE:
    retv = new TyvisAggregate();
    break;     

  case IIR_ALIAS_DECLARATION:
    retv = new TyvisAliasDeclaration();
    break;     

  case IIR_ALLOCATOR:
    retv = new TyvisAllocator();
    break;     

  case IIR_AND_OPERATOR:
    retv = new TyvisAndOperator();
    break;     

  case IIR_ARCHITECTURE_DECLARATION:
    retv = new TyvisArchitectureDeclaration();
    break;     

  case IIR_ARCHITECTURE_STATEMENT:
    retv = new TyvisArchitectureStatement();
    break;     

  case IIR_ARCHITECTURE_STATEMENT_LIST:
    retv = new TyvisArchitectureStatementList();
    break;     

  case IIR_ARRAY_NATURE_DEFINITION:
    retv = new TyvisArrayNatureDefinition();
    break;     

  case IIR_ARRAY_SUBNATURE_DEFINITION:
    retv = new TyvisArraySubnatureDefinition();
    break;     

  case IIR_ARRAY_SUBTYPE_DEFINITION:
    retv = new TyvisArraySubtypeDefinition();
    break;     

  case IIR_ARRAY_TYPE_DEFINITION:
    retv = new TyvisArrayTypeDefinition();
    break;     

  case IIR_ASCENDING_ATTRIBUTE:
    retv = new TyvisAscendingAttribute();
    break;     

  case IIR_ASSERTION_STATEMENT:
    retv = new TyvisAssertionStatement();
    break;     

  case IIR_ASSOCIATION_ELEMENT_BY_EXPRESSION:
    retv = new TyvisAssociationElementByExpression();
    break;     

  case IIR_ASSOCIATION_ELEMENT_BY_OPEN:
    retv = new TyvisAssociationElementOpen();
    break;     

  case IIR_ASSOCIATION_LIST:
    retv = new TyvisAssociationList();
    break;     

  case IIR_ATTRIBUTE_DECLARATION:
    retv = new TyvisAttributeDeclaration();
    break;     

  case IIR_ATTRIBUTE_SPECIFICATION:
    retv = new TyvisAttributeSpecification();
    break;     

  case IIR_ATTRIBUTE_SPECIFICATION_LIST:
    retv = new TyvisAttributeSpecificationList();
    break;     

  case IIR_BASE_ATTRIBUTE:
    retv = new TyvisBaseAttribute();
    break;     

  case IIR_BIT_STRING_LITERAL:
    retv = new TyvisBitStringLiteral();
    break;     

  case IIR_BLOCK_CONFIGURATION:
    retv = new TyvisBlockConfiguration();
    break;     

  case IIR_BLOCK_STATEMENT:
    retv = new TyvisBlockStatement();
    break;     

  case IIR_BRANCH_QUANTITY_DECLARATION:
    retv = new TyvisBranchQuantityDeclaration();
    break;     

  case IIR_BREAK_ELEMENT:
    retv = new TyvisBreakElement();
    break;     

  case IIR_BREAK_STATEMENT:
    retv = new TyvisBreakStatement();
    break;     

  case IIR_CASE_STATEMENT_ALTERNATIVE_BY_CHOICES:
    retv = new TyvisCaseStatementAlternativeByChoices();
    break;     

  case IIR_CASE_STATEMENT_ALTERNATIVE_BY_EXPRESSION:
    retv = new TyvisCaseStatementAlternativeByExpression();
    break;     

  case IIR_CASE_STATEMENT_ALTERNATIVE_BY_OTHERS:
    retv = new TyvisCaseStatementAlternativeByOthers();
    break;     

  case IIR_CASE_STATEMENT_ALTERNATIVE_LIST:
    retv = new TyvisCaseStatementAlternativeList();
    break;     

  case IIR_CASE_STATEMENT:
    retv = new TyvisCaseStatement();
    break;     

  case IIR_CHARACTER_LITERAL:
    retv = new TyvisCharacterLiteral();
    break;     

  case IIR_CHOICE:
    retv = new TyvisChoice();
    break;     

  case IIR_CHOICE_LIST:
    retv = new TyvisChoiceList();
    break;     

  case IIR_COMMENT:
    retv = new TyvisComment();
    break;     

  case IIR_COMMENT_LIST:
    retv = new TyvisCommentList();
    break;     

  case IIR_COMPONENT_CONFIGURATION:
    retv = new TyvisComponentConfiguration();
    break;     

  case IIR_COMPONENT_DECLARATION:
    retv = new TyvisComponentDeclaration();
    break;     

  case IIR_COMPONENT_INSTANTIATION_STATEMENT:
    retv = new TyvisComponentInstantiationStatement();
    break;     

  case IIR_CONCATENATION_OPERATOR:
    retv = new TyvisConcatenationOperator();
    break;     

  case IIR_CONCURRENT_ASSERTION_STATEMENT:
    retv = new TyvisConcurrentAssertionStatement();
    break;     

  case IIR_CONCURRENT_BREAK_STATEMENT:
    retv = new TyvisConcurrentBreakStatement();
    break;

  case IIR_CONCURRENT_CONDITIONAL_SIGNAL_ASSIGNMENT:
    retv = new TyvisConcurrentConditionalSignalAssignment();
    break;     

  case IIR_CONCURRENT_GENERATE_FOR_STATEMENT:
    retv = new TyvisConcurrentGenerateForStatement();
    break;     

  case IIR_CONCURRENT_GENERATE_IF_STATEMENT:
    retv = new TyvisConcurrentGenerateIfStatement();
    break;     

  case IIR_CONCURRENT_PROCEDURE_CALL_STATEMENT:
    retv = new TyvisConcurrentProcedureCallStatement();
    break;     

  case IIR_CONCURRENT_SELECTED_SIGNAL_ASSIGNMENT:
    retv = new TyvisConcurrentSelectedSignalAssignment();
    break;     

  case IIR_CONDITIONAL_WAVEFORM:
    retv = new TyvisConditionalWaveform();
    break;     

  case IIR_CONDITIONAL_WAVEFORM_LIST:
    retv = new TyvisConditionalWaveformList();
    break;     

  case IIR_CONFIGURATION_DECLARATION:
    retv = new TyvisConfigurationDeclaration();
    break;     

  case IIR_CONFIGURATION_ITEM:
    retv = new TyvisConfigurationItem();
    break;     

  case IIR_CONFIGURATION_ITEM_LIST:
    retv = new TyvisConfigurationItemList();
    break;     

  case IIR_CONFIGURATION_SPECIFICATION:
    retv = new TyvisConfigurationSpecification();
    break;     

  case IIR_CONTRIBUTION_ATTRIBUTE:
    retv = new TyvisContributionAttribute();
    break;     

  case IIR_CONSTANT_DECLARATION:
    retv = new TyvisConstantDeclaration();
    break;     

  case IIR_CONSTANT_INTERFACE_DECLARATION:
    retv = new TyvisConstantInterfaceDeclaration();
    break;     

  case IIR_DECLARATION_LIST:
    retv = new TyvisDeclarationList();
    break;     

  case IIR_DELAYED_ATTRIBUTE:
    retv = new TyvisDelayedAttribute();
    break;     

  case IIR_DESIGNATOR_BY_ALL:
    retv = new TyvisDesignatorByAll();
    break;     

  case IIR_DESIGNATOR_BY_OTHERS:
    retv = new TyvisDesignatorByOthers();
    break;     

  case IIR_DESIGNATOR_EXPLICIT:
    retv = new TyvisDesignatorExplicit();
    break;     

  case IIR_DESIGNATOR_LIST:
    retv = new TyvisDesignatorList();
    break;     

  case IIR_DESIGN_FILE:
    retv = new TyvisDesignFile();
    break;     

  case IIR_DESIGN_FILE_LIST:
    retv = new TyvisDesignFileList();
    break;     

  case IIR_DESIGN_UNIT_LIST:
    retv = new TyvisDesignUnitList();
    break;     

  case IIR_DISCONNECT_SPECIFICATION:
    retv = new TyvisDisconnectSpecification();
    break;     

  case IIR_DIVISION_OPERATOR:
    retv = new TyvisDivisionOperator();
    break;     

  case IIR_DOT_ATTRIBUTE:
    retv = new TyvisDotAttribute();
    break;     

  case IIR_DRIVING_ATTRIBUTE:
    retv = new TyvisDrivingAttribute();
    break;     

  case IIR_DRIVING_VALUE_ATTRIBUTE:
    retv = new TyvisDrivingValueAttribute();
    break;     

  case IIR_ELEMENT_DECLARATION:
    retv = new TyvisElementDeclaration();
    break;     

  case IIR_ELEMENT_DECLARATION_LIST:
    retv = new TyvisElementDeclarationList();
    break;     

  case IIR_ELSIF:
    retv = new TyvisElsif();
    break;     

  case IIR_ENTITY_CLASS_ENTRY:
    retv = new TyvisEntityClassEntry();
    break;     

  case IIR_ENTITY_CLASS_ENTRY_LIST:
    retv = new TyvisEntityClassEntryList();
    break;     

  case IIR_ENTITY_DECLARATION:
    retv = new TyvisEntityDeclaration();
    break;     

  case IIR_ENTITY_NAME_LIST:
    retv = new TyvisEntityNameList();
    break;     

  case IIR_ENUMERATION_LITERAL:
    retv = new TyvisEnumerationLiteral();
    break;     

  case IIR_ENUMERATION_LITERAL_LIST:
    retv = new TyvisEnumerationLiteralList();
    break;     

  case IIR_ENUMERATION_SUBTYPE_DEFINITION:
    retv = new TyvisEnumerationSubtypeDefinition();
    break;     

  case IIR_ENUMERATION_TYPE_DEFINITION:
    retv = new TyvisEnumerationTypeDefinition();
    break;     

  case IIR_EQUALITY_OPERATOR:
    retv = new TyvisEqualityOperator();
    break;     

  case IIR_EVENT_ATTRIBUTE:
    retv = new TyvisEventAttribute();
    break;     

  case IIR_EXIT_STATEMENT:
    retv = new TyvisExitStatement();
    break;     

  case IIR_EXPONENTIATION_OPERATOR:
    retv = new TyvisExponentiationOperator();
    break;     

  case IIR_FILE_DECLARATION:
    retv = new TyvisFileDeclaration();
    break;     

  case IIR_FILE_INTERFACE_DECLARATION:
    retv = new TyvisFileInterfaceDeclaration();
    break;     

  case IIR_FILE_TYPE_DEFINITION:
    retv = new TyvisFileTypeDefinition();
    break;     

  case IIR_FLOATING_POINT_LITERAL32:
    retv = new TyvisFloatingPointLiteral32();
    break;     

  case IIR_FLOATING_POINT_LITERAL64:
    retv = new TyvisFloatingPointLiteral64();
    break;     

  case IIR_FLOATING_POINT_LITERAL:
    retv = new TyvisFloatingPointLiteral();
    break;     

  case IIR_FLOATING_SUBTYPE_DEFINITION:
    retv = new TyvisFloatingSubtypeDefinition();
    break;     

  case IIR_FLOATING_TYPE_DEFINITION:
    retv = new TyvisFloatingTypeDefinition();
    break;     

  case IIR_FOR_LOOP_STATEMENT:
    retv = new TyvisForLoopStatement();
    break;     

  case IIR_FREE_QUANTITY_DECLARATION:
    retv = new TyvisFreeQuantityDeclaration();
    break;     

  case IIR_FUNCTION_CALL:
    retv = new TyvisFunctionCall();
    break;     

  case IIR_FUNCTION_DECLARATION:
    retv = new TyvisFunctionDeclaration();
    break;     

  case IIR_GENERIC_LIST:
    retv = new TyvisGenericList();
    break;     

  case IIR_GREATER_THAN_OPERATOR:
    retv = new TyvisGreaterThanOperator();
    break;     

  case IIR_GREATER_THAN_OR_EQUAL_OPERATOR:
    retv = new TyvisGreaterThanOrEqualOperator();
    break;     

  case IIR_GROUP_CONSTITUENT:
    retv = new TyvisGroupConstituent();
    break;     

  case IIR_GROUP_DECLARATION:
    retv = new TyvisGroupDeclaration();
    break;     

  case IIR_GROUP_TEMPLATE_DECLARATION:
    retv = new TyvisGroupTemplateDeclaration();
    break;     

  case IIR_HIGH_ATTRIBUTE:
    retv = new TyvisHighAttribute();
    break;     

  case IIR_IDENTIFIER:
    retv = new TyvisIdentifier();
    break;     

  case IIR_IDENTIFIER_LIST:
    retv = new TyvisIdentifierList();
    break;     

  case IIR_IDENTITY_OPERATOR:
    retv = new TyvisIdentityOperator();
    break;     

  case IIR_IF_STATEMENT:
    retv = new TyvisIfStatement();
    break;     

  case IIR_IMAGE_ATTRIBUTE:
    retv = new TyvisImageAttribute();
    break;     

  case IIR_INCOMPLETE_TYPE_DEFINITION:
    retv = new TyvisIncompleteTypeDefinition();
    break;     

  case IIR_INDEXED_NAME:
    retv = new TyvisIndexedName();
    break;     

  case IIR_INEQUALITY_OPERATOR:
    retv = new TyvisInequalityOperator();
    break;     

  case IIR_INSTANCE_NAME_ATTRIBUTE:
    retv = new TyvisInstanceNameAttribute();
    break;     

  case IIR_INTEGER_LITERAL32:
    retv = new TyvisIntegerLiteral32();
    break;     

  case IIR_INTEGER_LITERAL64:
    retv = new TyvisIntegerLiteral64();
    break;     

  case IIR_INTEGER_LITERAL:
    retv = new TyvisIntegerLiteral();
    break;     

  case IIR_INTEGER_SUBTYPE_DEFINITION:
    retv = new TyvisIntegerSubtypeDefinition();
    break;     

  case IIR_INTEGER_TYPE_DEFINITION:
    retv = new TyvisIntegerTypeDefinition();
    break;     

  case IIR_INTERFACE_LIST:
    retv = new TyvisInterfaceList();
    break;     

  case IIR_LABEL:
    retv = new TyvisLabel();
    break;     

  case IIR_LAST_ACTIVE_ATTRIBUTE:
    retv = new TyvisLastActiveAttribute();
    break;     

  case IIR_LAST_EVENT_ATTRIBUTE:
    retv = new TyvisLastEventAttribute();
    break;     

  case IIR_LAST_VALUE_ATTRIBUTE:
    retv = new TyvisLastValueAttribute();
    break;     

  case IIR_LEFT_ATTRIBUTE:
    retv = new TyvisLeftAttribute();
    break;     

  case IIR_LEFT_OF_ATTRIBUTE:
    retv = new TyvisLeftOfAttribute();
    break;     

  case IIR_LENGTH_ATTRIBUTE:
    retv = new TyvisLengthAttribute();
    break;     

  case IIR_LESS_THAN_OPERATOR:
    retv = new TyvisLessThanOperator();
    break;     

  case IIR_LESS_THAN_OR_EQUAL_OPERATOR:
    retv = new TyvisLessThanOrEqualOperator();
    break;     

  case IIR_LIBRARY_CLAUSE:
    retv = new TyvisLibraryClause();
    break;     

  case IIR_LIBRARY_DECLARATION:
    retv = new TyvisLibraryDeclaration();
    break;     

  case IIR_LIBRARY_UNIT_LIST:
    retv = new TyvisLibraryUnitList();
    break;     

  case IIR_LITERAL:
    retv = new TyvisLiteral();
    break;     

  case IIR_LTF_ATTRIBUTE:
    retv = new TyvisLTFAttribute();
    break;     

  case IIR_LOW_ATTRIBUTE:
    retv = new TyvisLowAttribute();
    break;     

  case IIR_MODULUS_OPERATOR:
    retv = new TyvisModulusOperator();
    break;     

  case IIR_MULTIPLICATION_OPERATOR:
    retv = new TyvisMultiplicationOperator();
    break;     

  case IIR_NAME:
    retv = new TyvisName();
    break;     

  case IIR_NAME_LIST:
    retv = new TyvisNameList();
    break;     

  case IIR_NAND_OPERATOR:
    retv = new TyvisNandOperator();
    break;     

  case IIR_NATURE_DECLARATION:
    retv = new TyvisNatureDeclaration();
    break;     

  case IIR_NATURE_DEFINITION:
    retv = new TyvisNatureDefinition();
    break;     

  case IIR_NEGATION_OPERATOR:
    retv = new TyvisNegationOperator();
    break;     

  case IIR_NEXT_STATEMENT:
    retv = new TyvisNextStatement();
    break;     

  case IIR_NOR_OPERATOR:
    retv = new TyvisNorOperator();
    break;     

  case IIR_NOT_OPERATOR:
    retv = new TyvisNotOperator();
    break;     

  case IIR_NULL_STATEMENT:
    retv = new TyvisNullStatement();
    break;     

  case IIR_OR_OPERATOR:
    retv = new TyvisOrOperator();
    break;     

  case IIR_OTHERS_INITIALIZATION:
    retv = new TyvisOthersInitialization();
    break;     

  case IIR_PACKAGE_BODY_DECLARATION:
    retv = new TyvisPackageBodyDeclaration();
    break;     

  case IIR_PACKAGE_DECLARATION:
    retv = new TyvisPackageDeclaration();
    break;     

  case IIR_PATH_NAME_ATTRIBUTE:
    retv = new TyvisPathNameAttribute();
    break;     

  case IIR_PHYSICAL_LITERAL:
    retv = new TyvisPhysicalLiteral();
    break;     

  case IIR_PHYSICAL_SUBTYPE_DEFINITION:
    retv = new TyvisPhysicalSubtypeDefinition();
    break;     

  case IIR_PHYSICAL_TYPE_DEFINITION:
    retv = new TyvisPhysicalTypeDefinition();
    break;     

  case IIR_PHYSICAL_UNIT:
    retv = new TyvisPhysicalUnit();
    break;     

  case IIR_PORT_LIST:
    retv = new TyvisPortList();
    break;     

  case IIR_POS_ATTRIBUTE:
    retv = new TyvisPosAttribute();
    break;     

  case IIR_PRED_ATTRIBUTE:
    retv = new TyvisPredAttribute();
    break;     

  case IIR_PROCEDURE_CALL_STATEMENT:
    retv = new TyvisProcedureCallStatement();
    break;     

  case IIR_PROCEDURE_DECLARATION:
    retv = new TyvisProcedureDeclaration();
    break;     

  case IIR_PROCESS_STATEMENT:
    retv = new TyvisProcessStatement();
    break;     

  case IIR_QUALIFIED_EXPRESSION:
    retv = new TyvisQualifiedExpression();
    break;     

  case IIR_QUANTITY_DECLARATION:
    retv = new TyvisQuantityDeclaration();
    break;     

  case IIR_QUIET_ATTRIBUTE:
    retv = new TyvisQuietAttribute();
    break;     

  case IIR_RANGE_ATTRIBUTE:
    retv = new TyvisRangeAttribute();
    break;     

  case IIR_RANGE_TYPE_DEFINITION:
    retv = new TyvisRangeTypeDefinition();
    break;     

  case IIR_RECORD_TYPE_DEFINITION:
    retv = new TyvisRecordTypeDefinition();
    break;     

  case IIR_REFERENCE_ATTRIBUTE:
    retv = new TyvisReferenceAttribute();
    break;     

  case IIR_REMAINDER_OPERATOR:
    retv = new TyvisRemainderOperator();
    break;     

  case IIR_REPORT_STATEMENT:
    retv = new TyvisReportStatement();
    break;     

  case IIR_RETURN_STATEMENT:
    retv = new TyvisReturnStatement();
    break;     

  case IIR_REVERSE_RANGE_ATTRIBUTE:
    retv = new TyvisReverseRangeAttribute();
    break;     

  case IIR_RIGHT_ATTRIBUTE:
    retv = new TyvisRightAttribute();
    break;     

  case IIR_RIGHT_OF_ATTRIBUTE:
    retv = new TyvisRightOfAttribute();
    break;     

  case IIR_ROL_OPERATOR:
    retv = new TyvisROLOperator();
    break;     

  case IIR_ROR_OPERATOR:
    retv = new TyvisROROperator();
    break;     

  case IIR_SCALAR_NATURE_DEFINITION:
    retv = new TyvisScalarNatureDefinition();
    break;     

  case IIR_SELECTED_NAME_BY_ALL:
    retv = new TyvisSelectedNameByAll();
    break;     

  case IIR_SELECTED_NAME:
    retv = new TyvisSelectedName();
    break;     

  case IIR_SELECTED_WAVEFORM:
    retv = new TyvisSelectedWaveform();
    break;     

  case IIR_SELECTED_WAVEFORM_LIST:
    retv = new TyvisSelectedWaveformList();
    break;     

  case IIR_SENSITIZED_PROCESS_STATEMENT:
    retv = new TyvisSensitizedProcessStatement();
    break;     

  case IIR_SEQUENTIAL_STATEMENT_LIST:
    retv = new TyvisSequentialStatementList();
    break;     

  case IIR_SHARED_VARIABLE_DECLARATION:
    retv = new TyvisSharedVariableDeclaration();
    break;     

  case IIR_SIGNAL_ASSIGNMENT_STATEMENT:
    retv = new TyvisSignalAssignmentStatement();
    break;     

  case IIR_SIGNAL_DECLARATION:
    retv = new TyvisSignalDeclaration();
    break;     

  case IIR_SIGNAL_INTERFACE_DECLARATION:
    retv = new TyvisSignalInterfaceDeclaration();
    break;     

  case IIR_SIGNATURE:
    retv = new TyvisSignature();
    break;     

  case IIR_SIMPLE_NAME_ATTRIBUTE:
    retv = new TyvisSimpleNameAttribute();
    break;     

  case IIR_SIMPLE_NAME:
    retv = new TyvisSimpleName();
    break;     

  case IIR_SIMPLE_SIMULTANEOUS_STATEMENT:
    retv = new TyvisSimpleSimultaneousStatement();
    break;     

  case IIR_SIMULTANEOUS_IF_STATEMENT:
    retv = new TyvisSimultaneousIfStatement();
    break;     

  case IIR_SLA_OPERATOR:
    retv = new TyvisSLAOperator();
    break;     

  case IIR_SLICE_NAME:
    retv = new TyvisSliceName();
    break;     

  case IIR_SLL_OPERATOR:
    retv = new TyvisSLLOperator();
    break;     

  case IIR_SOURCE_QUANTITY_DECLARATION:
    retv = new TyvisSourceQuantityDeclaration();
    break;     

  case IIR_SRA_OPERATOR:
    retv = new TyvisSRAOperator();
    break;     

  case IIR_SRL_OPERATOR:
    retv = new TyvisSRLOperator();
    break;     

  case IIR_STABLE_ATTRIBUTE:
    retv = new TyvisStableAttribute();
    break;     

  case IIR_STRING_LITERAL:
    retv = new TyvisStringLiteral();
    break;     

  case IIR_SUBPROGRAM_DECLARATION:
    retv = new TyvisSubprogramDeclaration();
    break;     

  case IIR_SUBTRACTION_OPERATOR:
    retv = new TyvisSubtractionOperator();
    break;     

  case IIR_SUBTYPE_DECLARATION:
    retv = new TyvisSubtypeDeclaration();
    break;     

  case IIR_SUCC_ATTRIBUTE:
    retv = new TyvisSuccAttribute();
    break;     

  case IIR_TERMINAL_DECLARATION:
    retv = new TyvisTerminalDeclaration();
    break;     

  case IIR_TERMINAL_INTERFACE_DECLARATION:
    retv = new TyvisTerminalInterfaceDeclaration();
    break;     

  case IIR_THROUGH_ATTRIBUTE:
    retv = new TyvisThroughAttribute();
    break;     

  case IIR_TRANSACTION_ATTRIBUTE:
    retv = new TyvisTransactionAttribute();
    break;     

  case IIR_TUPLE:
    retv = new TyvisTuple();
    break;     

  case IIR_TYPE_CONVERSION:
    retv = new TyvisTypeConversion();
    break;     

  case IIR_TYPE_DECLARATION:
    retv = new TyvisTypeDeclaration();
    break;     

  case IIR_UNIT_LIST:
    retv = new TyvisUnitList();
    break;     

  case IIR_USE_CLAUSE:
    retv = new TyvisUseClause();
    break;     

  case IIR_USER_ATTRIBUTE:
    retv = new TyvisUserAttribute();
    break;     

  case IIR_VAL_ATTRIBUTE:
    retv = new TyvisValAttribute();
    break;     

  case IIR_VALUE_ATTRIBUTE:
    retv = new TyvisValueAttribute();
    break;     

  case IIR_VARIABLE_ASSIGNMENT_STATEMENT:
    retv = new TyvisVariableAssignmentStatement();
    break;     

  case IIR_VARIABLE_DECLARATION:
    retv = new TyvisVariableDeclaration();
    break;     

  case IIR_VARIABLE_INTERFACE_DECLARATION:
    retv = new TyvisVariableInterfaceDeclaration();
    break;     

  case IIR_WAIT_STATEMENT:
    retv = new TyvisWaitStatement();
    break;     

  case IIR_WAVEFORM_ELEMENT:
    retv = new TyvisWaveformElement();
    break;     

  case IIR_WAVEFORM_LIST:
    retv = new TyvisWaveformList();
    break;     

  case IIR_WHILE_LOOP_STATEMENT:
    retv = new TyvisWhileLoopStatement();
    break;     

  case IIR_XNOR_OPERATOR:
    retv = new TyvisXnorOperator();
    break;     

  case IIR_XOR_OPERATOR:
    retv = new TyvisXorOperator();
    break;     

  case IIR_ZTF_ATTRIBUTE:
    retv = new TyvisZTFAttribute();
    break;     

  default:
    // Attempted to create an unknown type of node
    string error;
    error = "Error creating node";;
    report_error(error);
  }

  return retv;
}

StandardPackage *
tyvis_plugin_class_factory::get_standard_package(){
  static StandardPackage *instance = 
    dynamic_cast<StandardPackage *>(ScramStandardPackage::instance()->convert_tree( this ));
  return instance;
}

IIR_AbsoluteOperator *
tyvis_plugin_class_factory::new_IIR_AbsoluteOperator() {
  return new TyvisAbsoluteOperator();
}

IIR_AccessSubtypeDefinition *
tyvis_plugin_class_factory::new_IIR_AccessSubtypeDefinition() {
  return new TyvisAccessSubtypeDefinition();
}

IIR_AccessTypeDefinition *
tyvis_plugin_class_factory::new_IIR_AccessTypeDefinition() {
  return new TyvisAccessTypeDefinition();
}

IIR_AcrossAttribute *
tyvis_plugin_class_factory::new_IIR_AcrossAttribute() {
  return new TyvisAcrossAttribute();
}

IIR_ActiveAttribute *
tyvis_plugin_class_factory::new_IIR_ActiveAttribute() {
  return new TyvisActiveAttribute();
}

IIR_AdditionOperator *
tyvis_plugin_class_factory::new_IIR_AdditionOperator() {
  return new TyvisAdditionOperator();
}

IIR_Aggregate *
tyvis_plugin_class_factory::new_IIR_Aggregate(){
  return new TyvisAggregate();
}

IIR_AliasDeclaration *
tyvis_plugin_class_factory::new_IIR_AliasDeclaration(){
  return new TyvisAliasDeclaration();
}

IIR_Allocator *
tyvis_plugin_class_factory::new_IIR_Allocator(){
  return new TyvisAllocator();
}

IIR_AndOperator *
tyvis_plugin_class_factory::new_IIR_AndOperator(){
  return new TyvisAndOperator();
}

IIR_ArchitectureDeclaration *
tyvis_plugin_class_factory::new_IIR_ArchitectureDeclaration(){
  return new TyvisArchitectureDeclaration();
}

IIR_ArchitectureStatement *
tyvis_plugin_class_factory::new_IIR_ArchitectureStatement(){
  return new TyvisArchitectureStatement();
}

IIR_ArchitectureStatementList *
tyvis_plugin_class_factory::new_IIR_ArchitectureStatementList(){
  return new TyvisArchitectureStatementList();
}

IIR_ArrayNatureDefinition *
tyvis_plugin_class_factory::new_IIR_ArrayNatureDefinition(){
  return new TyvisArrayNatureDefinition();
}

IIR_ArraySubnatureDefinition *
tyvis_plugin_class_factory::new_IIR_ArraySubnatureDefinition(){
  return new TyvisArraySubnatureDefinition();
}

IIR_ArraySubtypeDefinition *
tyvis_plugin_class_factory::new_IIR_ArraySubtypeDefinition(){
  return new TyvisArraySubtypeDefinition();
}

IIR_ArrayTypeDefinition *
tyvis_plugin_class_factory::new_IIR_ArrayTypeDefinition(){
  return new TyvisArrayTypeDefinition();
}

IIR_AscendingAttribute *
tyvis_plugin_class_factory::new_IIR_AscendingAttribute(){
  return new TyvisAscendingAttribute();
}

IIR_AssertionStatement *
tyvis_plugin_class_factory::new_IIR_AssertionStatement(){
  return new TyvisAssertionStatement();
}

IIR_AssociationElementByExpression *
tyvis_plugin_class_factory::new_IIR_AssociationElementByExpression(){
  return new TyvisAssociationElementByExpression();
}

IIR_AssociationElementOpen *
tyvis_plugin_class_factory::new_IIR_AssociationElementOpen(){
  return new TyvisAssociationElementOpen();
}

IIR_AssociationList *
tyvis_plugin_class_factory::new_IIR_AssociationList(){
  return new TyvisAssociationList();
}

IIR_AttributeDeclaration *
tyvis_plugin_class_factory::new_IIR_AttributeDeclaration(){
  return new TyvisAttributeDeclaration();
}

IIR_AttributeSpecification *
tyvis_plugin_class_factory::new_IIR_AttributeSpecification(){
  return new TyvisAttributeSpecification();
}

IIR_AttributeSpecificationList *
tyvis_plugin_class_factory::new_IIR_AttributeSpecificationList(){
  return new TyvisAttributeSpecificationList();
}

IIR_BaseAttribute *
tyvis_plugin_class_factory::new_IIR_BaseAttribute(){
  return new TyvisBaseAttribute();
}

IIR_BitStringLiteral *
tyvis_plugin_class_factory::new_IIR_BitStringLiteral(){
  return new TyvisBitStringLiteral();
}

IIR_BlockConfiguration *
tyvis_plugin_class_factory::new_IIR_BlockConfiguration(){
  return new TyvisBlockConfiguration();
}

IIR_BlockStatement *
tyvis_plugin_class_factory::new_IIR_BlockStatement(){
  return new TyvisBlockStatement();
}

IIR_BranchQuantityDeclaration *
tyvis_plugin_class_factory::new_IIR_BranchQuantityDeclaration(){
  return new TyvisBranchQuantityDeclaration();
}

IIR_BreakElement *
tyvis_plugin_class_factory::new_IIR_BreakElement(){
  return new TyvisBreakElement();
}

IIR_BreakStatement *
tyvis_plugin_class_factory::new_IIR_BreakStatement(){
  return new TyvisBreakStatement();
}

IIR_CaseStatementAlternativeByChoices *
tyvis_plugin_class_factory::new_IIR_CaseStatementAlternativeByChoices() {
  return new TyvisCaseStatementAlternativeByChoices();
}

IIR_CaseStatementAlternativeByExpression *
tyvis_plugin_class_factory::new_IIR_CaseStatementAlternativeByExpression() {
  return new TyvisCaseStatementAlternativeByExpression();
}

IIR_CaseStatementAlternativeByOthers *
tyvis_plugin_class_factory::new_IIR_CaseStatementAlternativeByOthers() {
  return new TyvisCaseStatementAlternativeByOthers();
}

IIR_CaseStatementAlternativeList *
tyvis_plugin_class_factory::new_IIR_CaseStatementAlternativeList() {
  return new TyvisCaseStatementAlternativeList();
}

IIR_CaseStatement *
tyvis_plugin_class_factory::new_IIR_CaseStatement() {
  return new TyvisCaseStatement();
}

IIR_CharacterLiteral *
tyvis_plugin_class_factory::new_IIR_CharacterLiteral() {
  return new TyvisCharacterLiteral();
}

IIR_Choice *
tyvis_plugin_class_factory::new_IIR_Choice() {
  return new TyvisChoice();
}

IIR_ChoiceList *
tyvis_plugin_class_factory::new_IIR_ChoiceList() {
  return new TyvisChoiceList();
}

IIR_Comment *
tyvis_plugin_class_factory::new_IIR_Comment() {
  return new TyvisComment();
}

IIR_CommentList *
tyvis_plugin_class_factory::new_IIR_CommentList() {
  return new TyvisCommentList();
}

IIR_ComponentConfiguration *
tyvis_plugin_class_factory::new_IIR_ComponentConfiguration() {
  return new TyvisComponentConfiguration();
}

IIR_ComponentDeclaration *
tyvis_plugin_class_factory::new_IIR_ComponentDeclaration() {
  return new TyvisComponentDeclaration();
}

IIR_ComponentInstantiationStatement *
tyvis_plugin_class_factory::new_IIR_ComponentInstantiationStatement() {
  return new TyvisComponentInstantiationStatement();
}

IIR_ConcatenationOperator *
tyvis_plugin_class_factory::new_IIR_ConcatenationOperator() {
  return new TyvisConcatenationOperator();
}

IIR_ConcurrentAssertionStatement *
tyvis_plugin_class_factory::new_IIR_ConcurrentAssertionStatement() {
  return new TyvisConcurrentAssertionStatement();
}

IIR_ConcurrentBreakStatement *
tyvis_plugin_class_factory::new_IIR_ConcurrentBreakStatement() {
  return new TyvisConcurrentBreakStatement();
}

IIR_ConcurrentConditionalSignalAssignment *
tyvis_plugin_class_factory::new_IIR_ConcurrentConditionalSignalAssignment() {
  return new TyvisConcurrentConditionalSignalAssignment();
}

IIR_ConcurrentGenerateForStatement *
tyvis_plugin_class_factory::new_IIR_ConcurrentGenerateForStatement() {
  return new TyvisConcurrentGenerateForStatement();
}

IIR_ConcurrentGenerateIfStatement *
tyvis_plugin_class_factory::new_IIR_ConcurrentGenerateIfStatement() {
  return new TyvisConcurrentGenerateIfStatement();
}

IIR_ConcurrentProcedureCallStatement *
tyvis_plugin_class_factory::new_IIR_ConcurrentProcedureCallStatement() {
  return new TyvisConcurrentProcedureCallStatement();
}

IIR_ConcurrentSelectedSignalAssignment *
tyvis_plugin_class_factory::new_IIR_ConcurrentSelectedSignalAssignment() {
  return new TyvisConcurrentSelectedSignalAssignment();
}

IIR_ConditionalWaveform *
tyvis_plugin_class_factory::new_IIR_ConditionalWaveform() {
  return new TyvisConditionalWaveform();
}

IIR_ConditionalWaveformList *
tyvis_plugin_class_factory::new_IIR_ConditionalWaveformList() {
  return new TyvisConditionalWaveformList();
}

IIR_ConfigurationDeclaration *
tyvis_plugin_class_factory::new_IIR_ConfigurationDeclaration() {
  return new TyvisConfigurationDeclaration();
}

IIR_ConfigurationItem *
tyvis_plugin_class_factory::new_IIR_ConfigurationItem() {
  return new TyvisConfigurationItem();
}

IIR_ConfigurationItemList *
tyvis_plugin_class_factory::new_IIR_ConfigurationItemList() {
  return new TyvisConfigurationItemList();
}

IIR_ConfigurationSpecification *
tyvis_plugin_class_factory::new_IIR_ConfigurationSpecification() {
  return new TyvisConfigurationSpecification();
}

IIR_ContributionAttribute *
tyvis_plugin_class_factory::new_IIR_ContributionAttribute() {
  return new TyvisContributionAttribute();
}

IIR_ConstantDeclaration *
tyvis_plugin_class_factory::new_IIR_ConstantDeclaration() {
  return new TyvisConstantDeclaration();
}

IIR_ConstantInterfaceDeclaration *
tyvis_plugin_class_factory::new_IIR_ConstantInterfaceDeclaration() {
  return new TyvisConstantInterfaceDeclaration();
}

IIR_DeclarationList *
tyvis_plugin_class_factory::new_IIR_DeclarationList() {
  return new TyvisDeclarationList();
}

IIR_DelayedAttribute *
tyvis_plugin_class_factory::new_IIR_DelayedAttribute() {
  return new TyvisDelayedAttribute();
}

IIR_DesignatorByAll *
tyvis_plugin_class_factory::new_IIR_DesignatorByAll() {
  return new TyvisDesignatorByAll();
}

IIR_DesignatorByOthers *
tyvis_plugin_class_factory::new_IIR_DesignatorByOthers() {
  return new TyvisDesignatorByOthers();
}

IIR_DesignatorExplicit *
tyvis_plugin_class_factory::new_IIR_DesignatorExplicit() {
  return new TyvisDesignatorExplicit();
}

IIR_Designator *
tyvis_plugin_class_factory::new_IIR_Designator() {
  return new TyvisDesignator();
}

IIR_DesignatorList *
tyvis_plugin_class_factory::new_IIR_DesignatorList() {
  return new TyvisDesignatorList();
}

IIR_DesignFile *
tyvis_plugin_class_factory::new_IIR_DesignFile() {
  return new TyvisDesignFile();
}

IIR_DesignFileList *
tyvis_plugin_class_factory::new_IIR_DesignFileList() {
  return new TyvisDesignFileList();
}

IIR_DesignUnitList *
tyvis_plugin_class_factory::new_IIR_DesignUnitList() {
  return new TyvisDesignUnitList();
}

IIR_DisconnectSpecification *
tyvis_plugin_class_factory::new_IIR_DisconnectSpecification() {
  return new TyvisDisconnectSpecification();
}

IIR_DivisionOperator *
tyvis_plugin_class_factory::new_IIR_DivisionOperator() {
  return new TyvisDivisionOperator();
}

IIR_DotAttribute *
tyvis_plugin_class_factory::new_IIR_DotAttribute() {
  return new TyvisDotAttribute();
}

IIR_DrivingAttribute *
tyvis_plugin_class_factory::new_IIR_DrivingAttribute() {
  return new TyvisDrivingAttribute();
}

IIR_DrivingValueAttribute *
tyvis_plugin_class_factory::new_IIR_DrivingValueAttribute() {
  return new TyvisDrivingValueAttribute();
}

IIR_ElementDeclaration *
tyvis_plugin_class_factory::new_IIR_ElementDeclaration() {
  return new TyvisElementDeclaration();
}

IIR_ElementDeclarationList *
tyvis_plugin_class_factory::new_IIR_ElementDeclarationList() {
  return new TyvisElementDeclarationList();
}

IIR_Elsif *
tyvis_plugin_class_factory::new_IIR_Elsif() {
  return new TyvisElsif();
}

IIR_EntityClassEntry *
tyvis_plugin_class_factory::new_IIR_EntityClassEntry() {
  return new TyvisEntityClassEntry();
}

IIR_EntityClassEntryList *
tyvis_plugin_class_factory::new_IIR_EntityClassEntryList() {
  return new TyvisEntityClassEntryList();
}

IIR_EntityDeclaration *
tyvis_plugin_class_factory::new_IIR_EntityDeclaration() {
  return new TyvisEntityDeclaration();
}

IIR_EntityNameList *
tyvis_plugin_class_factory::new_IIR_EntityNameList() {
  return new TyvisEntityNameList();
}

IIR_EnumerationLiteral *
tyvis_plugin_class_factory::new_IIR_EnumerationLiteral() {
  return new TyvisEnumerationLiteral();
}

IIR_EnumerationLiteralList *
tyvis_plugin_class_factory::new_IIR_EnumerationLiteralList() {
  return new TyvisEnumerationLiteralList();
}

IIR_EnumerationSubtypeDefinition *
tyvis_plugin_class_factory::new_IIR_EnumerationSubtypeDefinition() {
  return new TyvisEnumerationSubtypeDefinition();
}

IIR_EnumerationTypeDefinition *
tyvis_plugin_class_factory::new_IIR_EnumerationTypeDefinition() {
  return new TyvisEnumerationTypeDefinition();
}

IIR_EqualityOperator *
tyvis_plugin_class_factory::new_IIR_EqualityOperator() {
  return new TyvisEqualityOperator();
}

IIR_EventAttribute *
tyvis_plugin_class_factory::new_IIR_EventAttribute() {
  return new TyvisEventAttribute();
}

IIR_ExitStatement *
tyvis_plugin_class_factory::new_IIR_ExitStatement() {
  return new TyvisExitStatement();
}

IIR_ExponentiationOperator *
tyvis_plugin_class_factory::new_IIR_ExponentiationOperator() {
  return new TyvisExponentiationOperator();
}

IIR_FileDeclaration *
tyvis_plugin_class_factory::new_IIR_FileDeclaration() {
  return new TyvisFileDeclaration();
}

IIR_FileInterfaceDeclaration *
tyvis_plugin_class_factory::new_IIR_FileInterfaceDeclaration() {
  return new TyvisFileInterfaceDeclaration();
}

IIR_FileTypeDefinition *
tyvis_plugin_class_factory::new_IIR_FileTypeDefinition() {
  return new TyvisFileTypeDefinition();
}

IIR_FloatingPointLiteral32 *
tyvis_plugin_class_factory::new_IIR_FloatingPointLiteral32() {
  return new TyvisFloatingPointLiteral32();
}

IIR_FloatingPointLiteral64 *
tyvis_plugin_class_factory::new_IIR_FloatingPointLiteral64() {
  return new TyvisFloatingPointLiteral64();
}

IIR_FloatingPointLiteral *
tyvis_plugin_class_factory::new_IIR_FloatingPointLiteral() {
  return new TyvisFloatingPointLiteral();
}

IIR_FloatingSubtypeDefinition *
tyvis_plugin_class_factory::new_IIR_FloatingSubtypeDefinition() {
  return new TyvisFloatingSubtypeDefinition();
}

IIR_FloatingTypeDefinition *
tyvis_plugin_class_factory::new_IIR_FloatingTypeDefinition() {
  return new TyvisFloatingTypeDefinition();
}

IIR_ForLoopStatement *
tyvis_plugin_class_factory::new_IIR_ForLoopStatement() {
  return new TyvisForLoopStatement();
}

IIR_FreeQuantityDeclaration *
tyvis_plugin_class_factory::new_IIR_FreeQuantityDeclaration() {
  return new TyvisFreeQuantityDeclaration();
}

IIR_FunctionCall *
tyvis_plugin_class_factory::new_IIR_FunctionCall() {
  return new TyvisFunctionCall();
}

IIR_FunctionDeclaration *
tyvis_plugin_class_factory::new_IIR_FunctionDeclaration() {
  return new TyvisFunctionDeclaration();
}

IIR_GenericList *
tyvis_plugin_class_factory::new_IIR_GenericList() {
  return new TyvisGenericList();
}

IIR_GreaterThanOperator *
tyvis_plugin_class_factory::new_IIR_GreaterThanOperator() {
  return new TyvisGreaterThanOperator();
}

IIR_GreaterThanOrEqualOperator *
tyvis_plugin_class_factory::new_IIR_GreaterThanOrEqualOperator() {
  return new TyvisGreaterThanOrEqualOperator();
}

IIR_GroupConstituent *
tyvis_plugin_class_factory::new_IIR_GroupConstituent() {
  return new TyvisGroupConstituent();
}

IIR_GroupDeclaration *
tyvis_plugin_class_factory::new_IIR_GroupDeclaration() {
  return new TyvisGroupDeclaration();
}

IIR_GroupTemplateDeclaration *
tyvis_plugin_class_factory::new_IIR_GroupTemplateDeclaration() {
  return new TyvisGroupTemplateDeclaration();
}

IIR_HighAttribute *
tyvis_plugin_class_factory::new_IIR_HighAttribute() {
  return new TyvisHighAttribute();
}

IIR_Identifier *
tyvis_plugin_class_factory::new_IIR_Identifier() {
  return new TyvisIdentifier();
}

IIR_IdentifierList *
tyvis_plugin_class_factory::new_IIR_IdentifierList() {
  return new TyvisIdentifierList();
}

IIR_IdentityOperator *
tyvis_plugin_class_factory::new_IIR_IdentityOperator() {
  return new TyvisIdentityOperator();
}

IIR_IfStatement *
tyvis_plugin_class_factory::new_IIR_IfStatement() {
  return new TyvisIfStatement();
}

IIR_ImageAttribute *
tyvis_plugin_class_factory::new_IIR_ImageAttribute() {
  return new TyvisImageAttribute();
}

IIR_IncompleteTypeDefinition *
tyvis_plugin_class_factory::new_IIR_IncompleteTypeDefinition() {
  return new TyvisIncompleteTypeDefinition();
}

IIR_IndexedName *
tyvis_plugin_class_factory::new_IIR_IndexedName() {
  return new TyvisIndexedName();
}

IIR_InequalityOperator *
tyvis_plugin_class_factory::new_IIR_InequalityOperator() {
  return new TyvisInequalityOperator();
}

IIR_InstanceNameAttribute *
tyvis_plugin_class_factory::new_IIR_InstanceNameAttribute() {
  return new TyvisInstanceNameAttribute();
}

IIR_IntegerLiteral32 *
tyvis_plugin_class_factory::new_IIR_IntegerLiteral32() {
  return new TyvisIntegerLiteral32();
}

IIR_IntegerLiteral64 *
tyvis_plugin_class_factory::new_IIR_IntegerLiteral64() {
  return new TyvisIntegerLiteral64();
}

IIR_IntegerLiteral *
tyvis_plugin_class_factory::new_IIR_IntegerLiteral() {
  return new TyvisIntegerLiteral();
}

IIR_IntegerSubtypeDefinition *
tyvis_plugin_class_factory::new_IIR_IntegerSubtypeDefinition() {
  return new TyvisIntegerSubtypeDefinition();
}

IIR_IntegerTypeDefinition *
tyvis_plugin_class_factory::new_IIR_IntegerTypeDefinition() {
  return new TyvisIntegerTypeDefinition();
}

IIR_InterfaceList *
tyvis_plugin_class_factory::new_IIR_InterfaceList() {
  return new TyvisInterfaceList();
}

IIR_Label *
tyvis_plugin_class_factory::new_IIR_Label() {
  return new TyvisLabel();
}

IIR_LastActiveAttribute *
tyvis_plugin_class_factory::new_IIR_LastActiveAttribute() {
  return new TyvisLastActiveAttribute();
}

IIR_LastEventAttribute *
tyvis_plugin_class_factory::new_IIR_LastEventAttribute() {
  return new TyvisLastEventAttribute();
}

IIR_LastValueAttribute *
tyvis_plugin_class_factory::new_IIR_LastValueAttribute() {
  return new TyvisLastValueAttribute();
}

IIR_LeftAttribute *
tyvis_plugin_class_factory::new_IIR_LeftAttribute() {
  return new TyvisLeftAttribute();
}

IIR_LeftOfAttribute *
tyvis_plugin_class_factory::new_IIR_LeftOfAttribute() {
  return new TyvisLeftOfAttribute();
}

IIR_LengthAttribute *
tyvis_plugin_class_factory::new_IIR_LengthAttribute() {
  return new TyvisLengthAttribute();
}

IIR_LessThanOperator *
tyvis_plugin_class_factory::new_IIR_LessThanOperator() {
  return new TyvisLessThanOperator();
}

IIR_LessThanOrEqualOperator *
tyvis_plugin_class_factory::new_IIR_LessThanOrEqualOperator() {
  return new TyvisLessThanOrEqualOperator();
}

IIR_LibraryClause *
tyvis_plugin_class_factory::new_IIR_LibraryClause() {
  return new TyvisLibraryClause();
}

IIR_LibraryDeclaration *
tyvis_plugin_class_factory::new_IIR_LibraryDeclaration() {
  return new TyvisLibraryDeclaration();
}

IIR_LibraryUnitList *
tyvis_plugin_class_factory::new_IIR_LibraryUnitList() {
  return new TyvisLibraryUnitList();
}

IIR_Literal *
tyvis_plugin_class_factory::new_IIR_Literal() {
  return new TyvisLiteral();
}

IIR_LTFAttribute *
tyvis_plugin_class_factory::new_IIR_LTFAttribute() {
  return new TyvisLTFAttribute();
}

IIR_LowAttribute *
tyvis_plugin_class_factory::new_IIR_LowAttribute() {
  return new TyvisLowAttribute();
}

IIR_ModulusOperator *
tyvis_plugin_class_factory::new_IIR_ModulusOperator() {
  return new TyvisModulusOperator();
}

IIR_MultiplicationOperator *
tyvis_plugin_class_factory::new_IIR_MultiplicationOperator() {
  return new TyvisMultiplicationOperator();
}

IIR_Name *
tyvis_plugin_class_factory::new_IIR_Name() {
  return new TyvisName();
}

IIR_NameList *
tyvis_plugin_class_factory::new_IIR_NameList() {
  return new TyvisNameList();
}

IIR_NandOperator *
tyvis_plugin_class_factory::new_IIR_NandOperator() {
  return new TyvisNandOperator();
}

IIR_NatureDeclaration *
tyvis_plugin_class_factory::new_IIR_NatureDeclaration() {
  return new TyvisNatureDeclaration();
}

IIR_NatureDefinition *
tyvis_plugin_class_factory::new_IIR_NatureDefinition() {
  return new TyvisNatureDefinition();
}

IIR_NegationOperator *
tyvis_plugin_class_factory::new_IIR_NegationOperator() {
  return new TyvisNegationOperator();
}

IIR_NextStatement *
tyvis_plugin_class_factory::new_IIR_NextStatement() {
  return new TyvisNextStatement();
}

IIR_NorOperator *
tyvis_plugin_class_factory::new_IIR_NorOperator() {
  return new TyvisNorOperator();
}

IIR_NotOperator *
tyvis_plugin_class_factory::new_IIR_NotOperator() {
  return new TyvisNotOperator();
}

IIR_NullStatement *
tyvis_plugin_class_factory::new_IIR_NullStatement() {
  return new TyvisNullStatement();
}

IIR_OrOperator *
tyvis_plugin_class_factory::new_IIR_OrOperator() {
  return new TyvisOrOperator();
}

IIR_OthersInitialization *
tyvis_plugin_class_factory::new_IIR_OthersInitialization() {
  return new TyvisOthersInitialization();
}

IIR_PackageBodyDeclaration *
tyvis_plugin_class_factory::new_IIR_PackageBodyDeclaration() {
  return new TyvisPackageBodyDeclaration();
}

IIR_PackageDeclaration *
tyvis_plugin_class_factory::new_IIR_PackageDeclaration() {
  return new TyvisPackageDeclaration();
}

IIR_PathNameAttribute *
tyvis_plugin_class_factory::new_IIR_PathNameAttribute() {
  return new TyvisPathNameAttribute();
}

IIR_PhysicalLiteral *
tyvis_plugin_class_factory::new_IIR_PhysicalLiteral() {
  return new TyvisPhysicalLiteral();
}

IIR_PhysicalSubtypeDefinition *
tyvis_plugin_class_factory::new_IIR_PhysicalSubtypeDefinition() {
  return new TyvisPhysicalSubtypeDefinition();
}

IIR_PhysicalTypeDefinition *
tyvis_plugin_class_factory::new_IIR_PhysicalTypeDefinition() {
  return new TyvisPhysicalTypeDefinition();
}

IIR_PhysicalUnit *
tyvis_plugin_class_factory::new_IIR_PhysicalUnit() {
  return new TyvisPhysicalUnit();
}

IIR_PortList *
tyvis_plugin_class_factory::new_IIR_PortList() {
  return new TyvisPortList();
}

IIR_PosAttribute *
tyvis_plugin_class_factory::new_IIR_PosAttribute() {
  return new TyvisPosAttribute();
}

IIR_PredAttribute *
tyvis_plugin_class_factory::new_IIR_PredAttribute() {
  return new TyvisPredAttribute();
}

IIR_ProcedureCallStatement *
tyvis_plugin_class_factory::new_IIR_ProcedureCallStatement() {
  return new TyvisProcedureCallStatement();
}

IIR_ProcedureDeclaration *
tyvis_plugin_class_factory::new_IIR_ProcedureDeclaration() {
  return new TyvisProcedureDeclaration();
}

IIR_ProcessStatement *
tyvis_plugin_class_factory::new_IIR_ProcessStatement() {
  return new TyvisProcessStatement();
}

IIR_QualifiedExpression *
tyvis_plugin_class_factory::new_IIR_QualifiedExpression() {
  return new TyvisQualifiedExpression();
}

IIR_QuantityDeclaration *
tyvis_plugin_class_factory::new_IIR_QuantityDeclaration() {
  return new TyvisQuantityDeclaration();
}

IIR_QuietAttribute *
tyvis_plugin_class_factory::new_IIR_QuietAttribute() {
  return new TyvisQuietAttribute();
}

IIR_RangeAttribute *
tyvis_plugin_class_factory::new_IIR_RangeAttribute() {
  return new TyvisRangeAttribute();
}

IIR_RangeTypeDefinition *
tyvis_plugin_class_factory::new_IIR_RangeTypeDefinition() {
  return new TyvisRangeTypeDefinition();
}

IIR_RecordTypeDefinition *
tyvis_plugin_class_factory::new_IIR_RecordTypeDefinition() {
  return new TyvisRecordTypeDefinition();
}

IIR_ReferenceAttribute *
tyvis_plugin_class_factory::new_IIR_ReferenceAttribute() {
  return new TyvisReferenceAttribute();
}

IIR_RemainderOperator *
tyvis_plugin_class_factory::new_IIR_RemainderOperator() {
  return new TyvisRemainderOperator();
}

IIR_ReportStatement *
tyvis_plugin_class_factory::new_IIR_ReportStatement() {
  return new TyvisReportStatement();
}

IIR_ReturnStatement *
tyvis_plugin_class_factory::new_IIR_ReturnStatement() {
  return new TyvisReturnStatement();
}

IIR_ReverseRangeAttribute *
tyvis_plugin_class_factory::new_IIR_ReverseRangeAttribute() {
  return new TyvisReverseRangeAttribute();
}

IIR_RightAttribute *
tyvis_plugin_class_factory::new_IIR_RightAttribute() {
  return new TyvisRightAttribute();
}

IIR_RightOfAttribute *
tyvis_plugin_class_factory::new_IIR_RightOfAttribute() {
  return new TyvisRightOfAttribute();
}

IIR_ROLOperator *
tyvis_plugin_class_factory::new_IIR_ROLOperator() {
  return new TyvisROLOperator();
}

IIR_ROROperator *
tyvis_plugin_class_factory::new_IIR_ROROperator() {
  return new TyvisROROperator();
}

IIR_ScalarNatureDefinition *
tyvis_plugin_class_factory::new_IIR_ScalarNatureDefinition() {
  return new TyvisScalarNatureDefinition();
}

IIR_SelectedNameByAll *
tyvis_plugin_class_factory::new_IIR_SelectedNameByAll() {
  return new TyvisSelectedNameByAll();
}

IIR_SelectedName *
tyvis_plugin_class_factory::new_IIR_SelectedName() {
  return new TyvisSelectedName();
}

IIR_SelectedWaveform *
tyvis_plugin_class_factory::new_IIR_SelectedWaveform() {
  return new TyvisSelectedWaveform();
}

IIR_SelectedWaveformList *
tyvis_plugin_class_factory::new_IIR_SelectedWaveformList() {
  return new TyvisSelectedWaveformList();
}

IIR_SensitizedProcessStatement *
tyvis_plugin_class_factory::new_IIR_SensitizedProcessStatement() {
  return new TyvisSensitizedProcessStatement();
}

IIR_SequentialStatementList *
tyvis_plugin_class_factory::new_IIR_SequentialStatementList() {
  return new TyvisSequentialStatementList();
}

IIR_SharedVariableDeclaration *
tyvis_plugin_class_factory::new_IIR_SharedVariableDeclaration() {
  return new TyvisSharedVariableDeclaration();
}

IIR_SignalAssignmentStatement *
tyvis_plugin_class_factory::new_IIR_SignalAssignmentStatement() {
  return new TyvisSignalAssignmentStatement();
}

IIR_SignalDeclaration *
tyvis_plugin_class_factory::new_IIR_SignalDeclaration() {
  return new TyvisSignalDeclaration();
}

IIR_SignalInterfaceDeclaration *
tyvis_plugin_class_factory::new_IIR_SignalInterfaceDeclaration() {
  return new TyvisSignalInterfaceDeclaration();
}

IIR_Signature *
tyvis_plugin_class_factory::new_IIR_Signature() {
  return new TyvisSignature();
}

IIR_SimpleNameAttribute *
tyvis_plugin_class_factory::new_IIR_SimpleNameAttribute() {
  return new TyvisSimpleNameAttribute();
}

IIR_SimpleName *
tyvis_plugin_class_factory::new_IIR_SimpleName() {
  return new TyvisSimpleName();
}

IIR_SimpleSimultaneousStatement *
tyvis_plugin_class_factory::new_IIR_SimpleSimultaneousStatement() {
  return new TyvisSimpleSimultaneousStatement();
}

IIR_SimultaneousElsif *
tyvis_plugin_class_factory::new_IIR_SimultaneousElsif() {
  return new TyvisSimultaneousElsif();
}

IIR_SimultaneousIfStatement *
tyvis_plugin_class_factory::new_IIR_SimultaneousIfStatement() {
  return new TyvisSimultaneousIfStatement();
}

IIR_SLAOperator *
tyvis_plugin_class_factory::new_IIR_SLAOperator() {
  return new TyvisSLAOperator();
}

IIR_SliceName *
tyvis_plugin_class_factory::new_IIR_SliceName() {
  return new TyvisSliceName();
}

IIR_SLLOperator *
tyvis_plugin_class_factory::new_IIR_SLLOperator() {
  return new TyvisSLLOperator();
}

IIR_SourceQuantityDeclaration *
tyvis_plugin_class_factory::new_IIR_SourceQuantityDeclaration() {
  return new TyvisSourceQuantityDeclaration();
}

IIR_SRAOperator *
tyvis_plugin_class_factory::new_IIR_SRAOperator() {
  return new TyvisSRAOperator();
}

IIR_SRLOperator *
tyvis_plugin_class_factory::new_IIR_SRLOperator() {
  return new TyvisSRLOperator();
}

IIR_StableAttribute *
tyvis_plugin_class_factory::new_IIR_StableAttribute() {
  return new TyvisStableAttribute();
}

IIR_StringLiteral *
tyvis_plugin_class_factory::new_IIR_StringLiteral() {
  return new TyvisStringLiteral();
}

IIR_SubprogramDeclaration *
tyvis_plugin_class_factory::new_IIR_SubprogramDeclaration() {
  return new TyvisSubprogramDeclaration();
}

IIR_SubtractionOperator *
tyvis_plugin_class_factory::new_IIR_SubtractionOperator() {
  return new TyvisSubtractionOperator();
}

IIR_SubtypeDeclaration *
tyvis_plugin_class_factory::new_IIR_SubtypeDeclaration() {
  return new TyvisSubtypeDeclaration();
}

IIR_SuccAttribute *
tyvis_plugin_class_factory::new_IIR_SuccAttribute() {
  return new TyvisSuccAttribute();
}

IIR_TerminalDeclaration *
tyvis_plugin_class_factory::new_IIR_TerminalDeclaration() {
  return new TyvisTerminalDeclaration();
}

IIR_TerminalInterfaceDeclaration *
tyvis_plugin_class_factory::new_IIR_TerminalInterfaceDeclaration() {
  return new TyvisTerminalInterfaceDeclaration();
}

IIR_TextLiteral *
tyvis_plugin_class_factory::new_IIR_TextLiteral() {
  return new TyvisTextLiteral();
}

IIR_ThroughAttribute *
tyvis_plugin_class_factory::new_IIR_ThroughAttribute() {
  return new TyvisThroughAttribute();
}

IIR_TransactionAttribute *
tyvis_plugin_class_factory::new_IIR_TransactionAttribute() {
  return new TyvisTransactionAttribute();
}

IIR_Tuple *
tyvis_plugin_class_factory::new_IIR_Tuple() {
  return new TyvisTuple();
}

IIR_TypeConversion *
tyvis_plugin_class_factory::new_IIR_TypeConversion() {
  return new TyvisTypeConversion();
}

IIR_TypeDeclaration *
tyvis_plugin_class_factory::new_IIR_TypeDeclaration() {
  return new TyvisTypeDeclaration();
}

IIR_UnitList *
tyvis_plugin_class_factory::new_IIR_UnitList() {
  return new TyvisUnitList();
}

IIR_UseClause *
tyvis_plugin_class_factory::new_IIR_UseClause() {
  return new TyvisUseClause();
}

IIR_UserAttribute *
tyvis_plugin_class_factory::new_IIR_UserAttribute() {
  return new TyvisUserAttribute();
}

IIR_ValAttribute *
tyvis_plugin_class_factory::new_IIR_ValAttribute() {
  return new TyvisValAttribute();
}

IIR_ValueAttribute *
tyvis_plugin_class_factory::new_IIR_ValueAttribute() {
  return new TyvisValueAttribute();
}

IIR_VariableAssignmentStatement *
tyvis_plugin_class_factory::new_IIR_VariableAssignmentStatement() {
  return new TyvisVariableAssignmentStatement();
}

IIR_VariableDeclaration *
tyvis_plugin_class_factory::new_IIR_VariableDeclaration() {
  return new TyvisVariableDeclaration();
}

IIR_VariableInterfaceDeclaration *
tyvis_plugin_class_factory::new_IIR_VariableInterfaceDeclaration() {
  return new TyvisVariableInterfaceDeclaration();
}

IIR_WaitStatement *
tyvis_plugin_class_factory::new_IIR_WaitStatement() {
  return new TyvisWaitStatement();
}

IIR_WaveformElement *
tyvis_plugin_class_factory::new_IIR_WaveformElement() {
  return new TyvisWaveformElement();
}

IIR_WaveformList *
tyvis_plugin_class_factory::new_IIR_WaveformList() {
  return new TyvisWaveformList();
}

IIR_WhileLoopStatement *
tyvis_plugin_class_factory::new_IIR_WhileLoopStatement() {
  return new TyvisWhileLoopStatement();
}

IIR_XnorOperator *
tyvis_plugin_class_factory::new_IIR_XnorOperator() {
  return new TyvisXnorOperator();
}

IIR_XorOperator *
tyvis_plugin_class_factory::new_IIR_XorOperator() {
  return new TyvisXorOperator();
}

IIR_ZTFAttribute *
tyvis_plugin_class_factory::new_IIR_ZTFAttribute() {
  return new TyvisZTFAttribute();
}

StandardPackage *
tyvis_plugin_class_factory::new_StandardPackage(){
  return new TyvisStandardPackage(this);
}

tyvis_plugin_class_factory::tyvis_plugin_class_factory(){}

tyvis_plugin_class_factory::~tyvis_plugin_class_factory(){}
