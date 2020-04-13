#include <tester.h>


using Vmm::Tester;

void Tester::attach_page(Genode::addr_t fault_addr) {

    if (!_attach_pages_on_fault) {
        return;
    }

    Genode::uint64_t bm = ~(SZ_PAGE - 1);
    fault_addr &= bm;

    Genode::uint64_t offset = fault_addr - BASE_RAM;
    Genode::Vm_session::Attach_attr attr = {
            .offset = offset,
            .size = SZ_PAGE,
            .writeable = true
    };
    _vm.attach(_vm_ram.cap(), BASE_RAM + offset, attr);

    _page_attached[offset / SZ_PAGE] = true;
    _num_attached_pages++;
    if(_num_attached_pages > 9 && _attach_pages_remaining) {
        _attach_remaining();
    }
}

void Tester::_attach_remaining() {

    Genode::Vm_session::Attach_attr attr = {
            .offset = 0,
            .size = SZ_PAGE,
            .writeable = true
    };

    for (int i = 0; i < NUM_PAGES; i++) {
        if (!_page_attached[i]) {
            attr.offset = i * SZ_PAGE;
            _vm.attach(_vm_ram.cap(), BASE_RAM + attr.offset, attr);
            _page_attached[i] = true;
        }
    }

    _rdy_for_test = true;
    _num_attached_pages = NUM_PAGES;
    Genode::log("all pages attached");
}

void Tester::_detach_entirely () {

    _vm.detach(BASE_RAM, SZ_RAM);
    Genode::log("entire detach");
}

void Tester::_detach_individually() {

    _rdy_for_test = false;
    _num_attached_pages = 0;

    for (int i = 0; i < NUM_PAGES; i++) {
        _vm.detach(BASE_RAM + i*SZ_PAGE, SZ_PAGE);
    }

    Genode::log("individual detach");
}

void Tester::_prepare_test_env() {

    if (!_rdy_for_test) {
        _attach_remaining();
    }

    _rdy_for_test = false;
    _num_attached_pages = 0;

    for (int i = 0; i < NUM_PAGES; i++ ) {
        _page_attached[i] = false;
    }
}

void Tester::_start_test(Genode::Duration) {

    _cpu.pause();
    _prepare_test_env();


    /* Test 1: (1) detach entire guest ram,
     *             this causes constant page faults
     *         */
    if (TEST_CASE == 1) {
        Genode::log("Start test 1");
        _detach_entirely();
        _attach_pages_on_fault = false;
        _attach_pages_remaining = false;
    }

    /* Test 2: (1) detach entire guest ram,
     *         (2) attach single page on fault
     *         (3) after 10 pages are attached, attach rest
     *         */
    if (TEST_CASE == 2) {
        Genode::log("Start test 2");
        _detach_entirely();
        _attach_pages_on_fault = true;
        _attach_pages_remaining = true;
        _timeout.schedule(Genode::Microseconds(TEN_SECS));
    }

    /* Test 3: (1) detach all pages individually,
     *             this causes constant page faults
     *         */
    if (TEST_CASE == 3) {
        Genode::log("Start test 3");
        _detach_individually();
        _attach_pages_on_fault = false;
        _attach_pages_remaining = false;
    }


    /* Test 4: (1) detach all pages individually,
     *         (2) attach single page on fault
     *         (3) after 10 pages are attached, attach rest
     *         */
    if (TEST_CASE == 4) {
        Genode::log("Start test 4");
        _detach_individually();
        _attach_pages_on_fault = true;
        _attach_pages_remaining = true;
        _timeout.schedule(Genode::Microseconds(TEN_SECS));
    }

    /* Test 5: (1) detach entire guest ram,
     *         (2) attach single page on fault
     *         */
    if (TEST_CASE == 5) {
        Genode::log("Start test 5");
        _detach_entirely();
        _attach_pages_on_fault = true;
        _attach_pages_remaining = false;
        _timeout.schedule(Genode::Microseconds(TEN_SECS));
    }

    /* Test 6: (1) detach all pages individually,
     *         (2) attach single page on fault
     *         */
    if (TEST_CASE == 6) {
        Genode::log("Start test 6");
        _detach_individually();
        _attach_pages_on_fault = true;
        _attach_pages_remaining = false;
        _timeout.schedule(Genode::Microseconds(TEN_SECS));
    }

    /* Test 7-12: set USE_SUERPAGE to 0 and
     *            do tests 1-6 again
     *         */

    _cpu.run();
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