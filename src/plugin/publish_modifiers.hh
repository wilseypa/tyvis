#ifndef PUBLISH_MODIFIERS_HH
#define PUBLISH_MODIFIERS_HH

#include <string>
using std::string;

class publish_modifier {
protected:
  publish_modifier(){}
};

class new_line : public publish_modifier {
public:
  new_line(){}
};

extern new_line &NL();

class prefix_modifier : public publish_modifier {
protected:
  prefix_modifier( string init_prefix ) :
    prefix( init_prefix ){}

  prefix_modifier( const prefix_modifier &toCopy ) : 
    publish_modifier(),
    prefix( toCopy.get_prefix() ){}

public:
  const string &get_prefix() const {
    return prefix;
  }

private:
  const string prefix;
};

class open_scope : public prefix_modifier {
public:
  open_scope( string init_prefix ) : prefix_modifier( init_prefix ){}

  open_scope( const open_scope &toCopy ) : prefix_modifier( toCopy.get_prefix() ){}
  
private:
};

class close_scope : public prefix_modifier {
public:
  close_scope( string init_prefix ) : prefix_modifier( init_prefix ){}

  close_scope( const close_scope &toCopy ) : prefix_modifier( toCopy.get_prefix() ){}
  
private:
};

extern open_scope OS( const string &prefix );
extern close_scope CS( const string &prefix );

#endif
