#include <base/env.h>
#include <base/attached_ram_dataspace.h>
#include <vm_session/connection.h>
#include <exception.h>

#define RM_ADDR 0x40000000
#define RAM_SZ 0x8000000
#define ACTIVE 1

namespace Vmm {
    class Snapshotter;
}

class Vmm::Snapshotter {

    private:
        Genode::Env                     & _env;
        Genode::Vm_connection           & _vm;
        Genode::Attached_ram_dataspace  & _vm_ram;
        bool                            _called = false;

    public:
        Snapshotter (Genode::Env    & env,
                     Genode::Vm_connection      & vm,
                     Genode::Attached_ram_dataspace & vm_ram);

        void copy_on_write();
};