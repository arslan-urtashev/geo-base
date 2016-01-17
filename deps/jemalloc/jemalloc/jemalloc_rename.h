/*
 * Name mangling for public symbols is controlled by --with-mangling and
 * --with-jemalloc-prefix.  With default settings the je_ prefix is stripped by
 * these macro definitions.
 */
#ifndef JEMALLOC_NO_RENAME
#  define je_malloc_conf yesmalloc_conf
#  define je_malloc_message yesmalloc_message
#  define je_malloc yesmalloc
#  define je_calloc yescalloc
#  define je_posix_memalign yesposix_memalign
#  define je_aligned_alloc yesaligned_alloc
#  define je_realloc yesrealloc
#  define je_free yesfree
#  define je_mallocx yesmallocx
#  define je_rallocx yesrallocx
#  define je_xallocx yesxallocx
#  define je_sallocx yessallocx
#  define je_dallocx yesdallocx
#  define je_sdallocx yessdallocx
#  define je_nallocx yesnallocx
#  define je_mallctl yesmallctl
#  define je_mallctlnametomib yesmallctlnametomib
#  define je_mallctlbymib yesmallctlbymib
#  define je_malloc_stats_print yesmalloc_stats_print
#  define je_malloc_usable_size yesmalloc_usable_size
#  define je_valloc yesvalloc
#endif
