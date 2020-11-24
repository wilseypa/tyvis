
// Copyright (c) 1996-2003 The University of Cincinnati.
// All rights reserved.

// UC MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
// OR NON-INFRINGEMENT.  UC SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY
// LICENSEE AS A RESULT OF USING, RESULT OF USING, MODIFYING OR
// DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the U.S.,
// and the terms of this license.

// You may modify, distribute, and use the software contained in this
// package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
// version 2, June 1991. A copy of this license agreement can be found in
// the file "LGPL", distributed with this archive.

// Authors: Philip A. Wilsey    philip.wilsey@ieee.org
//          Dale E. Martin      dmartin@cliftonlabs.com

//---------------------------------------------------------------------------

#include "TyvisXnorOperator.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisSignalInterfaceDeclaration.hh"
#include "savant/IIR_TextLiteral.hh"
#include "published_cc_file.hh"
#include "published_header_file.hh"

void
TyvisXnorOperator::_publish_cc_main(published_file& main_printer) {
   static unsigned num = 0;
   if( num == 0 ) {
        main_printer.add_include("XNor.hpp", false);
   }
   std::string myname = *_get_full_current_publish_name() + "logicGate" + std::to_string(num++);
   main_printer << "XNor * " << myname << " = new XNor( \"" << myname << "\" );" << NL();
   main_printer << "object_pointers.push_back( " << myname << " );" << NL();
   const std::string left_operand = dynamic_cast<TyvisSignalInterfaceDeclaration*>(get_left_operand())->get_declarator()->get_text();
   const std::string right_operand = dynamic_cast<TyvisSignalInterfaceDeclaration*>(get_right_operand())->get_declarator()->get_text();
   main_printer << myname << "->addSignal( \"" << left_operand << "\");" << NL();
   main_printer << myname << "->addSignal( \"" << right_operand << "\");" << NL();
   num++;
}

void
TyvisXnorOperator::_publish_cc() {
   static bool done = false;
   if( done )
      return;
   done = true;
   // start fie creation
   published_header_file _cc_out( _get_work_library()->get_path_to_directory(), 
         "XNor",
         this );
   _cc_out.add_include("tyvis/VHDLKernel.hh", true);
   CC_REF( _cc_out, 
         "TyvisXNorOperator::_publish_cc()" );
   _cc_out << "class XNor : public VHDLKernel {\n"
      << OS("public:");

   // Publish the constructor
   _cc_out << "XNor( const std::string& name );" << NL();

   // The destructor.
   _cc_out << "~XNor();\n" << NL()
      << "virtual std::vector<std::shared_ptr<warped::Event>> receiveEvent( const warped::Event& ) override;" << NL()
      << "virtual std::vector<std::shared_ptr<warped::Event>> initializeLP() override;\n" << NL()
      << "virtual std::vector<std::shared_ptr<warped::Event>> assignSignal( const std::string& name, int value, unsigned int delay, const VTime& timestamp ) override;\n" << NL()
      << CS("};");
   //Tyvis::_publish_cc_include( _cc_out, "header.hh" );

   published_cc_file _imp_out( _get_work_library()->get_path_to_directory(), 
         "XNor",
         this );
   CC_REF( _imp_out, 
         "TyvisConcurrentConditionalSignalAssignment::_publish_cc()" );

   _imp_out.add_include("XNor.hpp", false);
   _imp_out.add_include("tyvis/SigEvent.hh", true);
   _imp_out.add_include("cassert", true);
   // Publish the constructor
   _imp_out << "XNor::XNor( const std::string& name ) : VHDLKernel(name) {}" << NL();

   // The destructor.
   _imp_out << "XNor::~XNor() {}\n" << NL()
      << "std::vector<std::shared_ptr<warped::Event>>\n"
      << OS("XNor::receiveEvent( const warped::Event& event ) {")
      << "std::cout << name_ << \": received a message from \" << event.sender_name_" << NL()
      << "<< \" at time \" << event.timestamp() << \".\" << std::endl;" << NL()
      << "const SigEvent sign_event = static_cast<const SigEvent&>(event);" << NL()
      << "std::vector<std::shared_ptr<warped::Event>> response_events = assignSignal(sign_event.signalName(), sign_event.getValue(), 0, sign_event.timestamp());" << NL()
      << "int retval = 0;" << NL()
      << OS("for ( auto it = signals_.begin(); it != signals_.end(); it++) {")
      << "retval++;" << NL()
      << CS("}\n")
      << "response_events.emplace_back(new SigEvent { (*hierarchy_.begin()->second.begin()).first, (retval % 2) ? 0 : 1, (*hierarchy_.begin()->second.begin()).second, sign_event.timestamp() });" << NL()
      << "return response_events;" << NL()
      << CS("};\n")
      << "std::vector<std::shared_ptr<warped::Event>>\n"
      << OS("XNor::initializeLP() {")
      << "std::cout << name_ << \": initialization\" << std::endl;" << NL()
      << "std::vector<std::shared_ptr<warped::Event>> response_events;" << NL()
      << "return response_events;"
      << CS("};\n")
      << "std::vector<std::shared_ptr<warped::Event>>\n"
      << OS("XNor::assignSignal( const std::string& name, int value, unsigned int delay, const VTime& timestamp ) {")
      << "std::vector<std::shared_ptr<warped::Event>> response_events;" << NL()
      << "assert( signals_.find(name) != signals_.end() );" << NL()
      << "signals_[name] = value;" << NL()
      << "return response_events;"
      << CS("};");
   assert(done == true);
}
const string &
TyvisXnorOperator::_get_cc_operator_name() const {
   static const string operator_name = "vhdlXnor";
   return operator_name;
}
