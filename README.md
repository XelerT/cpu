# CPU
<p align="center">
<img
  src="imgs/cpu.png"
  alt="cpu"
  style="display: block; margin: auto; width: 150px">
</p>

This is stack soft-cpu learning project made during the 1st semester of the university.

## Install & Use

    $ git clone https://github.com/XelerT/cpu.git
    $ cd cpu
    $ make cpu/asm/dis_asm
    $ make run_cpu/run_asm/run_dis_asm

## Technical information
There are 2 types of memory:
- registers
- RAM

2 variable types:
- int
- float (with static dot)

## Assembly
This processor support intel-like assembly.
### Commands
#### Push/Pop
        push rax
        push [rbx + 5]
        push 5
        pushf rax

        pop rcx
        pop [rdx]
        pop
        popf
#### Math
        add
        sub
        mul
        mulf
        div
        divf
        sqrt
#### IO
        in
        out
        inf
        outf
#### Control
        label:
        jmp label
        call label
        je/ja/jbe/jae/jne label


