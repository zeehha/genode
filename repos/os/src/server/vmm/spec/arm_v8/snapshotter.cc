#include <snapshotter.h>


using Vmm::Snapshotter;

void Snapshotter::copy_on_write() {
    // TODO
    if ( _called) {
        throw Exception("STOP");
    }

    Genode::error("Snapshotter::copy_on_write");
    _vm.attach(_vm_ram.cap(), RM_ADDR);
    _called = true;

}

Snapshotter::Snapshotter (Genode::Env & env,
                          Genode::Vm_connection & vm,
                          Genode::Attached_ram_dataspace & vm_ram)
        :    _env(env),
             _vm(vm),
             _vm_ram(vm_ram)
{

}