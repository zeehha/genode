/*
 * \brief  testing cache-maintenance of page-table updates
 * \author Chris hofer
 */


#ifndef _SRC__SERVER__VMM__TESTER_H_
#define _SRC__SERVER__VMM__TESTER_H_

#include <base/env.h>
#include <base/attached_ram_dataspace.h>
#include <vm_session/connection.h>
#include <timer_session/connection.h>
#include <board.h>

namespace Vmm {
    class Tester;
}

class Vmm::Tester {

    private:
        Genode::Env                     & _env;
        Genode::Vm_connection           & _vm;
        Genode::Attached_ram_dataspace  & _vm_ram;

    public:
        Tester (Genode::Env  & env,
                Genode::Vm_connection  & vm,
                Genode::Attached_ram_dataspace  & vm_ram);

        void attach_ram();
};


#endif /* _SRC__SERVER__VMM__TESTER_H_ */