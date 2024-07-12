#include <proc.h>
#include <elf.h>
#include <fs.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
# define Elf_Shdr Elf64_Shdr
# define Elf_Sym  Elf64_Sym
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
# define Elf_Shdr Elf32_Shdr
# define Elf_Sym  Elf32_Sym
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

uintptr_t loader(PCB *pcb, const char *filename) {
  int fd = fs_open(filename, 0, 0);
  assert(fd != -1);
  
  size_t r;
  Elf_Ehdr elfHeader;
  r = fs_read(fd, &elfHeader, sizeof(elfHeader));
  assert(r == sizeof(elfHeader));
  assert(memcmp(elfHeader.e_ident, ELF_MAGIC_NUMBER, 4) == 0);
  assert(elfHeader.e_ident[EI_CLASS] == ELFCLASS32);
  assert(elfHeader.e_machine == EXPECT_ISA);

  Elf_Phdr phdrArray[elfHeader.e_phnum];
  fs_lseek(fd, elfHeader.e_phoff, SEEK_SET);
  r = fs_read(fd, phdrArray, sizeof(phdrArray));
  assert(r == sizeof(phdrArray));
  
  for (int i = 0; i < elfHeader.e_phnum; i++) {
    Elf_Phdr phdr = phdrArray[i];
    if (phdr.p_type == PT_LOAD) {
      fs_lseek(fd, phdr.p_offset, SEEK_SET);
      fs_read(fd, (void*)(phdr.p_vaddr), phdr.p_filesz);
      memset((void*)(phdr.p_vaddr + phdr.p_filesz), 0, phdr.p_memsz - phdr.p_filesz); 
    }
  }

  fs_close(fd);
  return elfHeader.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}
