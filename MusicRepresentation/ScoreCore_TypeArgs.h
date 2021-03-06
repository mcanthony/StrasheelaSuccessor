//
//  ScoreCore_TypeArgs.h
//  MusicRepresentation
//
//  Created by torsten on 26/12/2013.
//  Copyright (c) 2013 torsten. All rights reserved.
//

/*! \file ScoreCore_TypeArgs.h
 Defines types and classes which allow to quasi hand optional and named parameters to score object constructors. These parameters are wrapped in a map called Args.
 */


/* // Musing on design
 
 In Strasheela the score object constructors are a large number of named arguments, and the constructor definition is spread over multiple classes. How can I implement something like that in C++?
 
 It is essential I get constructors with something like named arguments, even if they are not supported by bindings to other languages, because the complexity of creating score data gets unmanagable otherwise;
 If necessary, I may have additional simpler constructors for bindings to other langs;
 
 - Specify a single nested data structure (e.g., vector) as argument, which contains all args, and which can be created in-place, e.g., the following (note: such constructor does not work with map, unfortunately). Is this too lengthy? Likely...
 vector<string> args {"arg1", "x", "arg2", "y"};
 
 map<string,string> args2 {{"arg1", "x"}, {"arg2", "y"}};
 for(auto x: args2)
 std::cout << x.first << ": " << x.second << '\n';
 
 Lengthy type declaration can be reduced:
 typedef map<string,string> args;
 args myArgs {{"arg1", "x"}, {"arg2", "y"}};
 
 Constructor can be used in an expression, e.g., I can call an init method with
 init(args {{"arg1", "x"}, {"arg2", "y"}})
 init(arg1: "x", arg2: "y")  // preferred syntax comparions -- much shorter, but the other is not unreasonable
 
 Problem: args are only captured at run-time, and so the compiler cannot help much.
 
 Arg values are boost::variant instances. boost::apply_visitor cares for compile-time error checking of types.
 
 
 - Similar approach
 http://www.reddit.com/r/programming/comments/19bul0/c11_named_parameters_using_operator_suffixes_and/
 
 
 
 - method chaining: http://www.parashift.com/c++-faq-lite/named-parameter-idiom.html
 
 - The Boost Parameter Library: http://www.boost.org/doc/libs/1_55_0/libs/parameter/doc/html/index.html
 
 - Variadic function:
 https://en.wikipedia.org/wiki/Variadic_function#Example_in_C
 
  */


#ifndef __MusicRepresentation__ScoreCore_TypeArgs__
#define __MusicRepresentation__ScoreCore_TypeArgs__

class ScoreObject;

#include <map>
#include <vector>
#include <boost/variant.hpp>
#include "ScoreCore_ScoreObject.h"

/*******************************************************************************************************/
//
// Defining Args typ for accessing optional and named arguments for constructors of ScoreObject and subclasses.
//
/*******************************************************************************************************/

// typedef boost::variant<int,std::string> Arg;


// boost::variant doc: http://www.boost.org/doc/libs/1_55_0/doc/html/variant.html
typedef std::map<std::string, boost::variant<int,std::string,ScoreObject,std::vector<ScoreObject>>> Args;
/*!< Shorthand type for argument maps for score object constructors. This type effectively allows for optional and named arguments for constructors of ScoreObject and subclasses. You can conveniently create such named arguments like
 
 SomeScoreObjectClass x = {Args {{"arg1", 42}, {"arg2", "test"}}}
 
 \sa The constructors of ScoreObject and its subclasses.
 */
// Doc after typedef, because doxygen otherwise ignored it -- strange.


/*! Defines compile-time checked accessors for every type given to Args (i.e. the values in the map type called Args).
 */
// TODO: consider rewriting with type template
class getStringArg : public boost::static_visitor<std::string> {
public:
    std::string operator()(std::string& str) const;
    std::string operator()(int& i) const;
    std::string operator()(ScoreObject& x) const;
    std::string operator()(std::vector<ScoreObject>& xs) const;
};
class getIntArg : public boost::static_visitor<int> {
public:
    int operator()(std::string& str) const;
    int operator()(int& i) const;
    int operator()(ScoreObject& x) const;
    int operator()(std::vector<ScoreObject>& xs) const;
};
class getScoreObjectArg : public boost::static_visitor<ScoreObject> {
public:
    ScoreObject operator()(std::string& str) const;
    ScoreObject operator()(int& i) const;
    ScoreObject operator()(ScoreObject& x) const;
    ScoreObject operator()(std::vector<ScoreObject>& xs) const;
};
class getVectorOfScoreObjectsArg : public boost::static_visitor<std::vector<ScoreObject>> {
public:
    std::vector<ScoreObject> operator()(std::string& str) const;
    std::vector<ScoreObject> operator()(int& i) const;
    std::vector<ScoreObject> operator()(ScoreObject& x) const;
    std::vector<ScoreObject> operator()(std::vector<ScoreObject>& xs) const;
};


/*! Defines compile-time checked accessors for (i.e. the values in the map type called Args).
 */
// TODO: test and revise
template <typename T>
class getArg : public boost::static_visitor<T> {
public:
    T operator()(const T& t) { return t; };
    
    template <typename U>
    T operator()(const U& u) {
        // input of arbitrary type U instead of given T
        throw std::invalid_argument{"invalid type"};
    }
};

/*******************************************************************************************************/
//
// Aux functions
//
/*******************************************************************************************************/

/*! Returns a copy of Args map as, reduced by the keys in keys
 */
Args reduceArgsBy(Args as, std::vector<std::string> keys);

/*! Extracts arg named argName from Args map as. If not contained in as, then defaultVal is return instead.
 */
int extractIntArg(Args as, std::string argName, int defaultVal);
std::string extractStringArg(Args as, std::string argName, std::string defaultVal);
//ScoreObject extractScoreObjectArg(Args as, std::string argName);
std::vector<ScoreObject> extractVectorOfScoreObjectsArg(Args as, std::string argName);



#endif /* defined(__MusicRepresentation__ScoreCore_TypeArgs__) */
