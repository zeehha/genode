#include <tester.h>


using Vmm::Tester;

void Tester::attach_page(Genode::addr_t fault_addr) {

//    Genode::uint64_t bm = ~(SZ_SUPERPAGE - 1);
//    fault_addr &= bm;
//
//    Genode::log("attach page ", Genode::Hex(fault_addr));
//
//    Genode::uint64_t offset = fault_addr - BASE_RAM;
//    Genode::Vm_session::Attach_attr attr = {
//            .offset = offset,
//            .size = SZ_SUPERPAGE,
//            .writeable = true
//    };
//    _vm.attach(_vm_ram.cap(), BASE_RAM + offset, attr);
//
//    _page_attached[offset / SZ_SUPERPAGE] = true;
//    _num_attached_pages++;
//    if(_num_attached_pages > 4) {
//        _attach_remaining();
//    }
}

void Tester::_attach_remaining() {

    Genode::Vm_session::Attach_attr attr = {
            .offset = 0,
            .size = SZ_SUPERPAGE,
            .writeable = true
    };

    for (int i = 0; i < NUM_SUPERPAGES; i++) {
        if (!_page_attached[i]) {
            attr.offset = i * SZ_SUPERPAGE;
            _vm.attach(_vm_ram.cap(), BASE_RAM + attr.offset, attr);
            _page_attached[i] = true;
        }
    }

    _rdy_for_test = true;
    _num_attached_pages = NUM_SUPERPAGES;
    Genode::log("all pages attached");
}

void Tester::_detach_all_pages () {

    _cpu.pause();
    _rdy_for_test = false;
    _num_attached_pages = 0;

    for (int i = 0; i < NUM_SUPERPAGES; i++ ) {
        _page_attached[i] = false;
    }

    _vm.detach(BASE_RAM, SZ_RAM);
    Genode::log("all pages detached");
    _cpu.run();

}

void Tester::_detach_partially() {


}

void Tester::_start_test(Genode::Duration) {

    /* setup environment for testing */
//    if (!_rdy_for_test) {
//        _cpu.pause();
//        _attach_remaining();
//        _cpu.run();
//    }


    /* Test 1: (1) detach entire guest ram,
     *         (2) attach single page on fault
     *         (3) after 5 pages are attached, attach rest
     *         */
    if (_rdy_for_test) {
        Genode::log("Start test 1");
       _detach_all_pages();
    }


    /* Test 2: (1) detach entire guest ram,
     *         (2) attach single page on fault
     *         (3) after 16 pages are attached, attach rest
     *         */


    _timeout.schedule(Genode::Microseconds(TEN_SECS));


}


Tester::Tester (Genode::Env & env,
                Genode::Vm_connection & vm,
                Cpu & cpu,
                Genode::Attached_ram_dataspace & vm_ram)
        :    _env(env),
             _vm(vm),
             _cpu(cpu),
             _vm_ram(vm_ram),
             _timeout(_timer, *this, &Tester::_start_test)
{
    _timeout.schedule(Genode::Microseconds(TEN_SECS));
}