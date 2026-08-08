#include <hal/cpu.h>
#include <boot/limine.h>
#include <stdint.h>
#include <stddef.h>

#define LIMINE_REQUEST __attribute__((used, section(".limine_requests")))

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_start_marker[] = LIMINE_REQUESTS_START_MARKER;

LIMINE_REQUEST
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(3);

LIMINE_REQUEST
volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_end_marker[] = LIMINE_REQUESTS_END_MARKER;

void kmain(void) {
    if (limine_base_revision[2] != 0) {
        cpu_halt();
    }

    if (framebuffer_request.response == NULL) {
        cpu_halt();
    }

    uint32_t *addr = (uint32_t *)framebuffer_request.response->framebuffers[0]->address;

    for (int i = 0; i < 100; i++) {
        addr[i] = 0x0000FF00;
    }

    while (1) {
        cpu_halt();
    }
}