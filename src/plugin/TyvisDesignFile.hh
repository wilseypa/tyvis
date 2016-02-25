#ifndef TYVIS_DESIGNFILE_HH
#define TYVIS_DESIGNFILE_HH

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

#include "Tyvis.hh"
#include "savant/IIRBase_DesignFile.hh"

class TyvisLibraryUnitList;
class TyvisCommentList;
class scram;

class TyvisDesignFile : public virtual Tyvis, public virtual IIRBase_DesignFile {
public:
  TyvisDesignFile();
  ~TyvisDesignFile();

  /** Publishes the units in this file.  Also publishes a Makefile.  
     
      @param last_file_this_run True if this is the last file to published
      on this run of scram or not.  The last file is assumed to be the top
      level of the design. */
  void _publish_cc( bool last_file_this_run );

  /** Publishes a makefile for this design file.  Currently all
      Makefiles will be named "Makefile" and therefore successive code
      generator runs will overwrite this file.  Due to the
      implementation of the publishing code, multiple runs within one
      program execution will cause code to be appended to the Makefile.
      Therefore, to avoid duplicated rules, the parameter was
      introduced.
      
      @param top_level If this parameter is true, an "all" rule will be
      published, along with other rules that apply to all design files.
      If it's false, only code specific to this file will be published.

      In general, when publishing an TyvisDesignFileList, this method
      should be called with "false" until the last unit is being
      published. */
  void _publish_cc_makefile( bool top_level );
  void _publish_cc_library_data( published_file &_cc_out );

  // This is a temporary function, eventually will be chaned to _publish_cc_main
  void _publish_cc_main(published_file&) override;

  const string _get_top_level_design_unit_name();

  plugin_class_factory                  *_get_class_factory();  

  // Helper Functions
  TyvisLibraryDeclaration        *_get_work_library();
  Tyvis                           *_get_name();
  TyvisLibraryUnitList           *_get_library_units();
  TyvisCommentList               *_get_comments();

protected:
private:
};
#endif
