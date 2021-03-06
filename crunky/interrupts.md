# Interrupts

According to [Timer interrupt example](https://www.raspberrypi.org/forums/viewtopic.php?f=72&t=286072),
interrupts are enabled on aarch32 architectures as follows:

```
cpsie i ; enable interrupts
cpsie f; enable fast interrupts (FIQ)
```

On aarch64:

```
msr daifclr, #2; enable interrupts
msr daifclr, #1; enable fast interrupts (FIQ)
```

It further states: "in both cases the enabling instruction should be followed by an instruction sync barrier assembly instruction", like so:

```
isb
```

As of 2020-12-06, `enable_irq()` is implemented in `vectors.S` by the assembly code:

```
.globl enable_irq
enable_irq:
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr
```

## Writing an interrupt

Define an interrupt like so:
```
void my_interrupt () __attribute__ ((interrupt ("IRQ")));
```

Permissible value for this parameter are: IRQ, FIQ, SWI, ABORT amd UNDEF 
([xlink](https://gcc.gnu.org/onlinedocs/gcc/ARM-Function-Attributes.html)).

Alternatively, use the form:
```
void __attribute__ ((interrupt ("IRQ"))) IRQ_handler()
{
	// do something
}
```

## Links to other sites

* [ARM system calls](https://wiki.osdev.org/ARM_System_Calls) (swi)
* [pi baremetal interrupts](https://github.com/brianwiddas/pi-baremetal/blob/master/interrupts.c)
