#include "Notification.h"
#include <typeinfo>

Notification::Notification() {

}

Notification::~Notification() {

}

std::string Notification::name() const {
  return typeid(*this).name();
}
