#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

#if defined(__ISA_AM_NATIVE__)
# define EXPECT_ISA EM_X86_64
#elif defined(__ISA_X86__)
# define EXPECT_ISA EM_386
#elif defined(__ISA_RISCV32__)
# define EXPECT_ISA EM_RISCV
#endif

static const char ELF_MAGIC_NUMBER[] = {0x7f, 'E', 'L', 'F'};

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t get_ramdisk_size();

static uintptr_t loader(PCB *pcb, const char *filename) {
  size_t r;
  Elf_Ehdr elfHeader;
  r = ramdisk_read(&elfHeader, 0, sizeof(elfHeader));
  assert(r == sizeof(elfHeader));
  assert(memcmp(elfHeader.e_ident, ELF_MAGIC_NUMBER, 4) == 0);
  assert(elfHeader.e_ident[EI_CLASS] == ELFCLASS32);
  assert(elfHeader.e_machine == EXPECT_ISA);

  Elf_Phdr phdrArray[elfHeader.e_phnum];
  r = ramdisk_read(phdrArray, elfHeader.e_phoff, sizeof(phdrArray));
  assert(r == sizeof(phdrArray));
  
  for (int i = 0; i < elfHeader.e_phnum; i++) {
    Elf_Phdr phdr = phdrArray[i];
    if (phdr.p_type == PT_LOAD) {
      ramdisk_read((void*)(phdr.p_vaddr), phdr.p_offset, phdr.p_filesz);
      memset((void*)(phdr.p_vaddr + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz); 
    }
  }
  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

