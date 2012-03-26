#include "RefCountedObject.h"

RefCountedObject::RefCountedObject() : counter_(1) {

}

RefCountedObject::~RefCountedObject() {
}
