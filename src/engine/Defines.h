#ifndef _MYENGINE_DEFINES_H
#define _MYENGINE_DEFINES_H

#include <memory>
#include <iostream>

/**
* A weak reference of 'type'
**/
#define weak(type) std::weak_ptr<type>

/**
* A shared reference of 'type'
**/
#define shared(type) std::shared_ptr<type>

/**
* A new shared reference of 'type'
**/
#define make_shared(type) std::make_shared<type>


#define NOTE(x) std::cout << "[ - ] " << x << std::endl
#define WARN(x) std::cout << "[ ! ] " << x << std::endl
#define ERR(x) std::cout << "[ X ] " << x << std::endl

//#define weak(type) type*
//#define shared(type) type*
//#define make_shared(type) new type

#endif
