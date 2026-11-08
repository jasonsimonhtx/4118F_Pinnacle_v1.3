#ifndef _SUBSYSTEMS_SUBSYSTEM_HPP_
#define _SUBSYSTEMS_SUBSYSTEM_HPP_

#include "main.h"

namespace subsys {

class SubsystemController {
public:
    SubsystemController(const str32_t identifier);
    ~SubsystemController();
protected:
    const str32_t identifier;
};

} // namespace subsys

#endif // _SUBSYSTEMS_SUBSYSTEM_HPP_