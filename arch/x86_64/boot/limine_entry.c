#include <boot/limine.h>
#include <boot/boot_info.h>
#include <stddef.h>

#define LIMINE_REQUEST __attribute__((used, section(".limine_requests")))

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_start_marker[] = LIMINE_REQUESTS_START_MARKER;

LIMINE_REQUEST
static volatile uint64_t limine_base_revision[3] = LIMINE_BASE_REVISION(3);

LIMINE_REQUEST
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

LIMINE_REQUEST
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST_ID,
    .revision = 0
};

LIMINE_REQUEST
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_end_marker[] = LIMINE_REQUESTS_END_MARKER;

static struct sysitn_boot_info boot_info;
static struct sysitn_framebuffer framebuffer;

void _start(void) {
    if (limine_base_revision[2] != 0) {
        __asm__ volatile ("cli; hlt");
    }

    if (framebuffer_request.response != NULL && framebuffer_request.response->framebuffer_count > 0) {
        struct limine_framebuffer *fb = framebuffer_request.response->framebuffers[0];
        framebuffer.address = (void*) fb->address;
        framebuffer.width = fb->width;
        framebuffer.height = fb->height;
        framebuffer.pitch = fb->pitch;
        framebuffer.bpp = fb->bpp;
        boot_info.framebuffer = &framebuffer;
    } else {
        boot_info.framebuffer = NULL;
    }

    if (hhdm_request.response != NULL) {
        boot_info.hhdm_offset = hhdm_request.response->offset;
    } else {
        boot_info.hhdm_offset = 0;
    }

    kmain(&boot_info);

    __asm__ volatile ("cli; hlt");
}