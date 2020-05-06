#include <tester.h>
#include <exception.h>

using Vmm::Tester;

/* Setup S2 translation, VM should then work as expected.
 * However the created translation does not reach Point of Unification
 * and the Page walker does not see it. Hence, exceptions are created
 * continously. Causes denial of service.
 * */
void Tester::attach_ram() {
	Genode::log("Setup S2 translation");
	_vm.attach(_vm_ram.cap(), RAM_START);
}


Tester::Tester (Genode::Env & env,
                Genode::Vm_connection & vm,
                Genode::Attached_ram_dataspace & vm_ram)
      :    _env(env),
           _vm(vm),
           _vm_ram(vm_ram)
{ }