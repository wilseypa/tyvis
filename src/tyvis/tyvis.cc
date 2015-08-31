// Copyright (c) Clifton Labs, Inc.  All Rights Reserved.

// CLIFTON LABS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE
// SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  CLIFTON LABS SHALL NOT BE
// LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, RESULT
// OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.

// By using or copying this Software, Licensee agrees to abide by the
// intellectual property laws, and all other applicable laws of the U.S.,
// and the terms of this license.

// You may modify, distribute, and use the software contained in this
// package under the terms of the "GNU LIBRARY GENERAL PUBLIC LICENSE"
// version 2, June 1991. A copy of this license agreement can be found in
// the file "LGPL", distributed with this archive.

#include <string>
#include <warped/WarpedMain.h>
#include <warped/warped.h>
#include "TyvisPluginLoader.hh"
#include "VHDLApplication.hh"

using std::string;

int
main( int argc, char **argv ){
  if( argc < 2 ){
    std::cout << "Usage: tyvis <tyvis module filename>" << std::endl;
    exit( -1 );
  }
  
  const string fileName( argv[1] );
  VHDLApplication *app = TyvisPluginLoader::instance().getVHDLApplication( fileName );
  ASSERT( app != 0 );

  WarpedMain wm( app );
  
  wm.main( argc, argv );
  
  return 0;
}
