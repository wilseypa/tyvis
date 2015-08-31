#include "publish_modifiers.hh"

new_line &
NL(){
  static new_line nl;
  return nl;
}

open_scope
OS( const std::string &prefix ){
  return open_scope( prefix );
}

close_scope
CS( const std::string &prefix ){
  return close_scope( prefix );
}
