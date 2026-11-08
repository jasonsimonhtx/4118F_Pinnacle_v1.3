#include "main.h"

namespace subsys {

class InertialController : public SubsystemController {
public:
    InertialController(const str32_t identifier);
    ~InertialController();
private:
    const str32_t identifier;
};

} // namespace subsys