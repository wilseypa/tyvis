
// Copyright (c) The University of Cincinnati.  All rights reserved.

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

#include "savant/savant_config.hh"
#include "TyvisArchitectureDeclaration.hh"
#include "TyvisCommentList.hh"
#include "TyvisDesignFile.hh"
#include "TyvisDesignFileList.hh"
#include "TyvisEntityDeclaration.hh"
#include "TyvisIdentifier.hh"
#include "TyvisLibraryDeclaration.hh"
#include "TyvisLibraryUnitList.hh"
#include <savant/generic_parser.hh>
#include "savant/set.hh"
#include "published_makefile.hh"
#include "published_cc_file.hh"
#include "savant/language_processing_control.hh"
#include "savant/library_manager.hh"
#include <sstream>

extern string design_library_name;
extern language_processing_control *lang_proc;

TyvisDesignFile::TyvisDesignFile() {
  set_comments(new TyvisCommentList());
  set_library_units(new TyvisLibraryUnitList());
}

TyvisDesignFile::~TyvisDesignFile() {
  //Release the list memory
  delete get_comments();
  delete get_library_units();
}

void 
TyvisDesignFile::_publish_cc( bool last_file_this_run ) {
  _set_currently_publishing_unit(Tyvis::NONE);

  _get_library_units()->_publish_cc();

  // We're assuming that the last file this run is the top level.
  // this procedure publish the makefile. Pointless for the moment.
  _publish_cc_makefile( last_file_this_run );

  _get_library_units()->_publish_cc_elaborate();
  if ( design_library_name == "" ){
    _publish_cc_main();
  }
}

void
TyvisDesignFile::_publish_cc_makefile( bool top_level ) {
  if( top_level == true ){
    published_makefile makefile( _get_work_library()->get_path_to_directory(),
				 "Makefile",
				 this );

    makefile << "# Tests with the g++ compiler show a substantial improvement in compile times\n"
	     << "# when multiple .cc files are combined and compiled with a single compiler\n"
	     << "# invocation.  However, we still want to support the possible use of DISTCC\n"
	     << "# and other conventional methods to compile the generated files.   Thus, \n"
	     << "# makefile output from tyvis now supports both the conventional method and the\n"
	     << "# creation of a single file containing includes to all the .cc source files.\n"
	     << "# Control of which option is used is determined by the environment variable\n"
	     << "# $(TYVIS_BUILDINDIVIDUALCCFILES).  If it is defined, all of the individual .cc\n"
	     << "# files are separately compiled; if it is not defined, then the individual .cc\n"
	     << "# files are compiled as one.\n\n\n";

    makefile << "# Begin publishing " << *_get_name() << "\n";

    string objectname;
    if( _get_current_architecture_name() != "" ){
      objectname =  _get_current_architecture_name();
    }
    else {
      objectname = "_savant_entity_elab";
    }
    string targetName = objectname;

    string backSlash = "\\";

    makefile << "SINGLEDOTCCFILE=" << targetName << "_allcc.cc\n"
	     << "SIMSRCS = $(filter-out $(SINGLEDOTCCFILE),$(wildcard *.cc))\n"
	     << "ALLINCLUDES=$(addprefix " << backSlash << "#include \",$(addsuffix \",$(SIMSRCS)))\n"
	     << "ifdef SAVANTROOT\n" 
	     << "  include $(SAVANTROOT)/lib/Makefile.common\n" 
	     << "else\n  include " << BUILD_SAVANTROOT << "/savant/lib/Makefile.common\n"
	     << "endif\n"
	     << "CPPFLAGS=$(SIMCPPFLAGS)\n"
	     << "CXXFLAGS=$(SIMCXXFLAGS)\n"
	     << "ALLSRCS=$(wildcard *.cc)\n"
	     << "# if the environment variable BUILDSINGLEFILE is defined, compile the single .cc\n"
	     << "# file (that includes all the other .cc files) to build the executable; otherwise\n"
	     << "# follow a conventional compiler of all the individual .cc files.\n"
	     << "ifdef TYVIS_BUILDINDIVIDUALCCFILES\n"
	     << "  ALLDOTOS=$(patsubst %.cc,%.o, $(SIMSRCS))\n"
	     <<	"  ALLDOTLOS=$(patsubst %.cc,%.lo, $(SIMSRCS))\n"
	     << "else\n"
	     << "  ALLDOTOS=$(patsubst %.cc,%.o, $(SINGLEDOTCCFILE))\n"
	     << "  ALLDOTLOS=$(patsubst %.cc,%.lo, $(SINGLEDOTCCFILE))\n"
	     << "endif\n";
    // include design libraries here...
    _publish_cc_library_data( makefile );

    makefile << "LIBS += $(LINK_THESE_LIBRARIES)\n\n";
    // Then we'll generate a magical "all" target.

    // Pick up all of the Makefile stubs that have been generated.
    makefile << "-include Makefile.*\n\n";

    makefile << "all: ";
      
    if( design_library_name == "" ){
      makefile << objectname << "\n\n";
    }
    else{
      makefile << design_library_name 
	      << library_manager::get_library_suffix() << ".la\n\n";
    }
      
    makefile << "depend: cleandep\n"
	     << "\t makedepend -p$(VHDLDIR)/ -f- -- $(CPPFLAGS) $(CXXFLAGS)"
	     << " -- $(DEPENDINC) $(SIMSRCS) > .depend\n"
	
	     << "cleandep:\n"
	     << "\t-rm -f .depend\n\n"

	     << "clean:\n"
	     << "\trm -rf *~ \\#*\\# .libs $(SINGLEDOTCCFILE);\n"
	     << "\trm -f *.a *.la *.lo *.so *.o\n"
      
	     << "-include .depend\n";
      
    // Now we'll generate how to build whatever this is we're talking about.
    if( design_library_name == "" ){
      // Needed for running benchmarks...
      makefile << targetName << ": $(ALLDOTOS)\n"
	       << "\t${LINK_exe}\n\n";

    }
    else{
      targetName = design_library_name + library_manager::get_library_suffix();
    }

    makefile << targetName << ".la: $(ALLDOTLOS)\n"
	     << "\t${LINK_lib}\n\n";
    
    makefile << "$(SINGLEDOTCCFILE): $(SIMSRCS)\n"
	     << "\trm -f $@\n"
	     << "\ttouch $@\n"
	     << "\tfind . -maxdepth 1 -type f " << backSlash << "( -name \"*.cc\" -and -not -name $(SINGLEDOTCCFILE) " << backSlash << ") -exec echo '#include \"{}\"' >>$@ \\;\n\n";


    makefile << targetName << ".so: " << targetName << ".la" << "\n"
	     << "\tlibtool --mode=install install -c $^ $(PWD)\n\n";

    makefile << "# End publishing " << *_get_name() << "\n";
  }
}

void
TyvisDesignFile::_publish_cc_main(){
  
  //Lot of work needed in this function
  published_cc_file cc_file( _get_work_library()->get_path_to_directory(),
			     "main",
			     this );
  CC_REF( cc_file, "TyvisDesignFile::_publish_cc_main" );

  _publish_cc_include( cc_file, _get_top_level_design_unit_name() + ".hh" );
  _publish_cc_include( cc_file, "warped.hpp", true );
  //_publish_cc_include( cc_file, "tyvis/VHDLApplication.hh", true );
  //if (lang_proc->processing_vhdl_ams()) {
  //  _publish_cc_include( cc_file, "tyvis/AMSApplication.hh", true);
  //}
  
  cc_file << "int" << NL()
	  << OS("main( int argc, char **argv ){")
     << "// In this vector you can add custom parameters" << NL()
	  << "std::vector<TCLAP::Arg*> args;" << NL()
	  << OS("warped::Simulation sim {")
     << "\"Digital Simulation\", argc, argv, args"
     // TODO: circuit constructor
	  << CS("};")
	  << "sim.simulate();"
	  << CS("}");
}

void
TyvisDesignFile::_publish_cc_library_data( published_file &_cc_out ){
  savant::set<IIR_LibraryDeclaration> *libraries = library_manager::instance()->get_libraries();
  
  ostringstream include_stream;
  ostringstream library_stream;

  IIRBase_LibraryDeclaration *curLibrary =
    dynamic_cast<IIRBase_LibraryDeclaration *>(libraries->getElement());

  TyvisLibraryDeclaration *current_library =
    dynamic_cast<TyvisLibraryDeclaration *>(curLibrary->convert_tree(get_design_file()->get_class_factory()));
  include_stream << "LIBRARY_INCLUDES=";
  library_stream << "LINK_THESE_LIBRARIES=";
  while( current_library != NULL ){
    include_stream << " -I";
    library_stream << " ";
    current_library->_publish_cc_include_path( include_stream );
    if(	current_library != _get_work_library() ){
      current_library->_publish_cc_library_file_name( library_stream );
    }
    
    current_library = dynamic_cast<TyvisLibraryDeclaration *>(libraries->getNextElement());
  }
  include_stream << "\n";
  library_stream << " -lVHDL -lltdl ";
  if ( lang_proc->processing_vhdl_ams() ){
    library_stream << "-lAMS -lsparse -lad";
  }
  _cc_out << include_stream.str() << "\n";
  _cc_out << library_stream.str() << "\n";

  _cc_out << "\n\n";
  _cc_out << "CPPFLAGS += $(LIBRARY_INCLUDES)\n";

  delete libraries;
}

TyvisCommentList *
TyvisDesignFile::_get_comments() {
  return dynamic_cast<TyvisCommentList *>(get_comments());
}

TyvisLibraryUnitList *
TyvisDesignFile::_get_library_units() {
  return dynamic_cast<TyvisLibraryUnitList *>(get_library_units());
}

Tyvis *
TyvisDesignFile::_get_name() {
  return dynamic_cast<Tyvis *>(get_name());
}

TyvisLibraryDeclaration *
TyvisDesignFile::_get_work_library(){
  ASSERT( my_parser != NULL );
  return dynamic_cast<TyvisLibraryDeclaration *>(my_parser->get_work_library());
}

plugin_class_factory *
TyvisDesignFile::_get_class_factory(){
  ASSERT( my_parser != NULL );
  ASSERT( my_parser->get_class_factory() != NULL );
  return my_parser->get_class_factory();
}

const string
TyvisDesignFile::_get_top_level_design_unit_name() {
  string objectname;
  if( _get_current_configuration_name() != "" ){
    objectname = "SCFG" 
      + _get_current_configuration_name() + "_"
      + _get_current_entity_name() + "_" 
      + _get_current_architecture_name() + "_elab";
  }
  else if( _get_current_entity_name() != "" && _get_current_architecture_name() != "" ){
    objectname = _get_current_architecture_name() + "_elab";
  }
  else {
    objectname = "_savant_entity_elab";
  }
  return objectname;
}
