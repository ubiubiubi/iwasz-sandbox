	.syntax unified
	.arch armv7-m
	.eabi_attribute 27, 3
	.eabi_attribute 28, 1
	.fpu fpv4-sp-d16
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 2
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.thumb
	.file	"stm32fxxx_it.c"
	.text
	.align	2
	.global	NMI_Handler
	.thumb
	.thumb_func
	.type	NMI_Handler, %function
NMI_Handler:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	bx	lr
	.size	NMI_Handler, .-NMI_Handler
	.align	2
	.global	HardFault_Handler
	.thumb
	.thumb_func
	.type	HardFault_Handler, %function
HardFault_Handler:
	@ Volatile: function does not return.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	ldr	r0, .L5
	bl	puts
.L3:
	b	.L3
.L6:
	.align	2
.L5:
	.word	.LC0
	.size	HardFault_Handler, .-HardFault_Handler
	.align	2
	.global	MemManage_Handler
	.thumb
	.thumb_func
	.type	MemManage_Handler, %function
MemManage_Handler:
	@ Volatile: function does not return.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
.L8:
	b	.L8
	.size	MemManage_Handler, .-MemManage_Handler
	.align	2
	.global	BusFault_Handler
	.thumb
	.thumb_func
	.type	BusFault_Handler, %function
BusFault_Handler:
	@ Volatile: function does not return.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
.L10:
	b	.L10
	.size	BusFault_Handler, .-BusFault_Handler
	.align	2
	.global	UsageFault_Handler
	.thumb
	.thumb_func
	.type	UsageFault_Handler, %function
UsageFault_Handler:
	@ Volatile: function does not return.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
.L12:
	b	.L12
	.size	UsageFault_Handler, .-UsageFault_Handler
	.align	2
	.global	SVC_Handler
	.thumb
	.thumb_func
	.type	SVC_Handler, %function
SVC_Handler:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	bx	lr
	.size	SVC_Handler, .-SVC_Handler
	.align	2
	.global	DebugMon_Handler
	.thumb
	.thumb_func
	.type	DebugMon_Handler, %function
DebugMon_Handler:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	bx	lr
	.size	DebugMon_Handler, .-DebugMon_Handler
	.align	2
	.global	PendSV_Handler
	.thumb
	.thumb_func
	.type	PendSV_Handler, %function
PendSV_Handler:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	bx	lr
	.size	PendSV_Handler, .-PendSV_Handler
	.align	2
	.global	SysTick_Handler
	.thumb
	.thumb_func
	.type	SysTick_Handler, %function
SysTick_Handler:
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	mov	r2, #2048
	movt	r2, 16386
	ldr	r2, [r2, #16]
	push	{r4, r5, r6, r7, r8, r9, sl, fp}
	movw	r3, #:lower16:.LANCHOR0
	movw	r7, #13107
	movt	r3, #:upper16:.LANCHOR0
	and	r1, r2, r7
	bic	r2, r2, #13056
	ldr	r0, [r3, #0]
	bic	r2, r2, #51
	lsls	r2, r2, #16
	lsrs	r2, r2, #16
	lsls	r1, r1, #16
	lsls	r0, r0, #2
	orr	r1, r1, r2, lsr #2
	and	r0, r0, #-858993460
	orrs	r1, r1, r0
	and	r5, r1, #15
	subs	r6, r5, #4
	and	r4, r1, #240
	rsbs	sl, r6, #0
	adcs	sl, sl, r6
	subs	fp, r4, #64
	rsbs	r9, fp, #0
	and	r0, r1, #3840
	adcs	r9, r9, fp
	cmp	r4, #32
	it	eq
	addeq	r9, r9, #1
	cmp	r5, #2
	it	eq
	addeq	sl, sl, #1
	subs	fp, r0, #1024
	ldrh	r8, [r3, #4]
	rsbs	ip, fp, #0
	and	r2, r1, #61440
	ldrh	r6, [r3, #6]
	adcs	ip, ip, fp
	cmp	r0, #512
	it	eq
	addeq	ip, ip, #1
	add	sl, sl, r8
	subs	fp, r2, #16384
	ldrh	r8, [r3, #8]
	add	r9, r9, r6
	rsbs	r6, fp, #0
	adcs	r6, r6, fp
	add	ip, ip, r8
	ldrh	r8, [r3, #10]
	cmp	r2, #8192
	it	eq
	addeq	r6, r6, #1
	cmp	r5, #11
	it	eq
	addeq	sl, sl, #1
	add	r6, r6, r8
	cmp	r4, #176
	it	eq
	addeq	r9, r9, #1
	cmp	r0, #2816
	it	eq
	addeq	ip, ip, #1
	cmp	r5, #13
	it	eq
	addeq	sl, sl, #1
	cmp	r2, #45056
	it	eq
	addeq	r6, r6, #1
	cmp	r4, #208
	it	eq
	addeq	r9, r9, #1
	cmp	r0, #3328
	it	eq
	addeq	ip, ip, #1
	cmp	r5, #1
	it	eq
	subeq	sl, sl, #1
	cmp	r2, #53248
	it	eq
	addeq	r6, r6, #1
	cmp	r4, #16
	it	eq
	subeq	r9, r9, #1
	cmp	r0, #256
	it	eq
	subeq	ip, ip, #1
	cmp	r5, #7
	it	eq
	subeq	sl, sl, #1
	cmp	r2, #4096
	it	eq
	subeq	r6, r6, #1
	cmp	r4, #112
	it	eq
	subeq	r9, r9, #1
	cmp	r0, #1792
	it	eq
	subeq	ip, ip, #1
	cmp	r5, #8
	it	eq
	subeq	sl, sl, #1
	cmp	r2, #28672
	it	eq
	subeq	r6, r6, #1
	cmp	r5, #14
	it	eq
	subeq	sl, sl, #1
	cmp	r4, #128
	it	eq
	subeq	r9, r9, #1
	and	r5, r1, #983040
	cmp	r0, #2048
	it	eq
	subeq	ip, ip, #1
	cmp	r2, #32768
	it	eq
	subeq	r6, r6, #1
	mov	r8, #3072
	sub	sp, sp, #32
	cmp	r4, #224
	it	eq
	subeq	r9, r9, #1
	cmp	r0, #3584
	it	eq
	subeq	ip, ip, #1
	cmp	r2, #57344
	it	eq
	subeq	r6, r6, #1
	movt	r8, 16386
	subs	r2, r5, #262144
	ldr	r8, [r8, #16]
	str	r9, [sp, #12]
	rsbs	r9, r2, #0
	adcs	r9, r9, r2
	ldrh	fp, [r3, #12]
	cmp	r5, #131072
	it	eq
	addeq	r9, r9, #1
	add	fp, r9, fp
	cmp	r5, #720896
	it	eq
	addeq	fp, fp, #1
	and	r4, r1, #15728640
	cmp	r5, #851968
	it	eq
	addeq	fp, fp, #1
	cmp	r5, #65536
	it	eq
	subeq	fp, fp, #1
	subs	r9, r4, #4194304
	str	sl, [sp, #4]
	rsbs	sl, r9, #0
	adcs	sl, sl, r9
	ldrh	r9, [r3, #14]
	cmp	r4, #2097152
	it	eq
	addeq	sl, sl, #1
	add	sl, sl, r9
	cmp	r4, #11534336
	it	eq
	addeq	sl, sl, #1
	cmp	r4, #13631488
	it	eq
	addeq	sl, sl, #1
	and	r0, r1, #251658240
	cmp	r5, #458752
	it	eq
	subeq	fp, fp, #1
	cmp	r4, #1048576
	it	eq
	subeq	sl, sl, #1
	cmp	r5, #524288
	it	eq
	subeq	fp, fp, #1
	cmp	r4, #7340032
	it	eq
	subeq	sl, sl, #1
	cmp	r0, #67108864
	ite	ne
	movne	r9, #0
	moveq	r9, #1
	cmp	r5, #917504
	it	eq
	subeq	fp, fp, #1
	ldrh	r5, [r3, #16]
	cmp	r0, #33554432
	it	eq
	addeq	r9, r9, #1
	cmp	r4, #8388608
	it	eq
	subeq	sl, sl, #1
	cmp	r4, #14680064
	it	eq
	subeq	sl, sl, #1
	add	r4, r9, r5
	cmp	r0, #184549376
	it	eq
	addeq	r4, r4, #1
	cmp	r0, #218103808
	it	eq
	addeq	r4, r4, #1
	and	r2, r1, #-268435456
	cmp	r0, #16777216
	it	eq
	subeq	r4, r4, #1
	cmp	r0, #117440512
	it	eq
	subeq	r4, r4, #1
	subs	r9, r2, #1073741824
	str	fp, [sp, #20]
	rsbs	fp, r9, #0
	adcs	fp, fp, r9
	ldrh	r5, [r3, #18]
	cmp	r2, #536870912
	it	eq
	addeq	fp, fp, #1
	add	fp, fp, r5
	and	r5, r8, r7
	bic	r8, r8, #13056
	cmp	r2, #-1342177280
	it	eq
	addeq	fp, fp, #1
	str	r6, [sp, #8]
	bic	r8, r8, #51
	ldr	r6, [r3, #68]
	cmp	r2, #-805306368
	it	eq
	addeq	fp, fp, #1
	lsl	r8, r8, #16
	cmp	r2, #268435456
	it	eq
	subeq	fp, fp, #1
	lsls	r5, r5, #16
	lsls	r6, r6, #2
	lsr	r8, r8, #16
	cmp	r2, #1879048192
	it	eq
	subeq	fp, fp, #1
	orr	r8, r5, r8, lsr #2
	and	r6, r6, #-858993460
	orr	r6, r8, r6
	cmp	r2, #-2147483648
	it	eq
	subeq	fp, fp, #1
	cmp	r0, #134217728
	it	eq
	subeq	r4, r4, #1
	cmp	r2, #-536870912
	it	eq
	subeq	fp, fp, #1
	and	r2, r6, #15
	cmp	r0, #234881024
	it	eq
	subeq	r4, r4, #1
	str	fp, [sp, #0]
	subs	fp, r2, #4
	rsbs	r8, fp, #0
	adcs	r8, r8, fp
	ldrh	r0, [r3, #20]
	cmp	r2, #2
	it	eq
	addeq	r8, r8, #1
	add	r8, r8, r0
	cmp	r2, #11
	it	eq
	addeq	r8, r8, #1
	cmp	r2, #13
	it	eq
	addeq	r8, r8, #1
	cmp	r2, #1
	it	eq
	subeq	r8, r8, #1
	cmp	r2, #7
	it	eq
	subeq	r8, r8, #1
	and	r5, r6, #240
	cmp	r2, #8
	it	eq
	subeq	r8, r8, #1
	cmp	r2, #14
	it	eq
	subeq	r8, r8, #1
	subs	r2, r5, #64
	str	sl, [sp, #24]
	rsbs	sl, r2, #0
	adcs	sl, sl, r2
	str	ip, [sp, #16]
	ldrh	fp, [r3, #22]
	mov	ip, #4096
	cmp	r5, #32
	it	eq
	addeq	sl, sl, #1
	movt	ip, 16386
	ldr	r2, [ip, #16]
	add	ip, sl, fp
	cmp	r5, #176
	it	eq
	addeq	ip, ip, #1
	cmp	r5, #208
	it	eq
	addeq	ip, ip, #1
	cmp	r5, #16
	it	eq
	subeq	ip, ip, #1
	cmp	r5, #112
	it	eq
	subeq	ip, ip, #1
	str	r4, [sp, #28]
	cmp	r5, #128
	it	eq
	subeq	ip, ip, #1
	and	r4, r6, #3840
	cmp	r5, #224
	ite	ne
	movne	r5, ip
	subeq	r5, ip, #1
	subs	ip, r4, #1024
	rsbs	sl, ip, #0
	adcs	sl, sl, ip
	ldrh	ip, [r3, #24]
	cmp	r4, #512
	it	eq
	addeq	sl, sl, #1
	add	ip, sl, ip
	cmp	r4, #2816
	it	eq
	addeq	ip, ip, #1
	cmp	r4, #3328
	it	eq
	addeq	ip, ip, #1
	cmp	r4, #256
	it	eq
	subeq	ip, ip, #1
	cmp	r4, #1792
	it	eq
	subeq	ip, ip, #1
	and	r0, r6, #61440
	cmp	r4, #2048
	it	eq
	subeq	ip, ip, #1
	cmp	r4, #3584
	ite	ne
	movne	r4, ip
	subeq	r4, ip, #1
	subs	sl, r0, #16384
	rsbs	ip, sl, #0
	adcs	ip, ip, sl
	ldrh	sl, [r3, #26]
	cmp	r0, #8192
	it	eq
	addeq	ip, ip, #1
	add	ip, ip, sl
	cmp	r0, #45056
	it	eq
	addeq	ip, ip, #1
	cmp	r0, #53248
	it	eq
	addeq	ip, ip, #1
	cmp	r0, #4096
	it	eq
	subeq	ip, ip, #1
	cmp	r0, #28672
	it	eq
	subeq	ip, ip, #1
	cmp	r0, #32768
	it	eq
	subeq	ip, ip, #1
	cmp	r0, #57344
	it	eq
	subeq	ip, ip, #1
	ldr	sl, [sp, #4]
	and	r0, r6, #983040
	mov	r9, #5120
	subs	fp, r0, #262144
	movt	r9, 16386
	ldr	r9, [r9, #16]
	strh	sl, [r3, #4]	@ movhi
	rsbs	sl, fp, #0
	adcs	sl, sl, fp
	str	r1, [r3, #0]
	ldrh	r1, [r3, #28]
	cmp	r0, #131072
	it	eq
	addeq	sl, sl, #1
	add	r1, sl, r1
	cmp	r0, #720896
	it	eq
	addeq	r1, r1, #1
	cmp	r0, #851968
	it	eq
	addeq	r1, r1, #1
	cmp	r0, #65536
	it	eq
	subeq	r1, r1, #1
	cmp	r0, #458752
	it	eq
	subeq	r1, r1, #1
	cmp	r0, #524288
	it	eq
	subeq	r1, r1, #1
	cmp	r0, #917504
	ite	ne
	movne	r0, r1
	subeq	r0, r1, #1
	ldr	fp, [sp, #12]
	ldr	r1, [sp, #16]
	strh	fp, [r3, #6]	@ movhi
	strh	r1, [r3, #8]	@ movhi
	ldr	sl, [sp, #8]
	strh	r5, [r3, #22]	@ movhi
	strh	sl, [r3, #10]	@ movhi
	and	r5, r2, r7
	ldr	sl, [sp, #28]
	bic	r2, r2, #13056
	strh	r4, [r3, #24]	@ movhi
	bic	r2, r2, #51
	and	r4, r6, #15728640
	ldr	r1, [sp, #24]
	strh	sl, [r3, #16]	@ movhi
	lsls	r2, r2, #16
	subs	sl, r4, #4194304
	str	r6, [r3, #68]
	strh	r8, [r3, #20]	@ movhi
	strh	r0, [r3, #28]	@ movhi
	and	r8, r6, #-268435456
	and	r0, r6, #251658240
	lsrs	r2, r2, #16
	lsls	r6, r5, #16
	rsbs	r5, sl, #0
	ldr	fp, [sp, #20]
	adcs	r5, r5, sl
	strh	r1, [r3, #14]	@ movhi
	orr	r2, r6, r2, lsr #2
	ldr	r1, [r3, #72]
	subs	r6, r0, #67108864
	strh	ip, [r3, #26]	@ movhi
	rsbs	ip, r6, #0
	adcs	ip, ip, r6
	strh	fp, [r3, #12]	@ movhi
	lsls	r1, r1, #2
	ldr	fp, [sp, #0]
	and	r1, r1, #-858993460
	cmp	r4, #2097152
	it	eq
	addeq	r5, r5, #1
	cmp	r0, #33554432
	it	eq
	addeq	ip, ip, #1
	cmp	r8, #1073741824
	orr	r2, r2, r1
	ldrh	sl, [r3, #32]
	ite	ne
	movne	r1, #0
	moveq	r1, #1
	ldrh	r6, [r3, #34]
	strh	fp, [r3, #18]	@ movhi
	ldrh	fp, [r3, #30]
	cmp	r8, #536870912
	it	eq
	addeq	r1, r1, #1
	add	r5, r5, fp
	add	ip, ip, sl
	adds	r1, r1, r6
	and	r6, r2, #15
	cmp	r4, #11534336
	it	eq
	addeq	r5, r5, #1
	cmp	r0, #184549376
	it	eq
	addeq	ip, ip, #1
	cmp	r8, #-1342177280
	it	eq
	addeq	r1, r1, #1
	subs	sl, r6, #4
	rsbs	fp, sl, #0
	adcs	fp, fp, sl
	cmp	r4, #13631488
	it	eq
	addeq	r5, r5, #1
	ldrh	sl, [r3, #36]
	cmp	r4, #1048576
	it	eq
	subeq	r5, r5, #1
	cmp	r0, #218103808
	it	eq
	addeq	ip, ip, #1
	cmp	r8, #-805306368
	it	eq
	addeq	r1, r1, #1
	cmp	r6, #2
	it	eq
	addeq	fp, fp, #1
	cmp	r4, #7340032
	it	eq
	subeq	r5, r5, #1
	add	fp, fp, sl
	cmp	r0, #16777216
	it	eq
	subeq	ip, ip, #1
	cmp	r8, #268435456
	it	eq
	subeq	r1, r1, #1
	cmp	r4, #8388608
	it	eq
	subeq	r5, r5, #1
	cmp	r4, #14680064
	it	eq
	subeq	r5, r5, #1
	cmp	r0, #117440512
	it	eq
	subeq	ip, ip, #1
	cmp	r8, #1879048192
	it	eq
	subeq	r1, r1, #1
	cmp	r6, #11
	it	eq
	addeq	fp, fp, #1
	cmp	r0, #134217728
	it	eq
	subeq	ip, ip, #1
	cmp	r8, #-2147483648
	it	eq
	subeq	r1, r1, #1
	cmp	r6, #13
	it	eq
	addeq	fp, fp, #1
	str	r5, [sp, #4]
	and	r5, r2, #240
	cmp	r0, #234881024
	it	eq
	subeq	ip, ip, #1
	cmp	r8, #-536870912
	it	eq
	subeq	r1, r1, #1
	cmp	r6, #1
	it	eq
	subeq	fp, fp, #1
	cmp	r5, #64
	str	r1, [sp, #16]
	it	ne
	movne	sl, #0
	ldrh	r1, [r3, #38]
	it	eq
	moveq	sl, #1
	and	r4, r2, #3840
	cmp	r5, #32
	it	eq
	addeq	sl, sl, #1
	add	sl, sl, r1
	subs	r1, r4, #1024
	rsbs	r8, r1, #0
	adcs	r8, r8, r1
	ldrh	r1, [r3, #40]
	cmp	r4, #512
	it	eq
	addeq	r8, r8, #1
	and	r0, r2, #61440
	add	r8, r8, r1
	cmp	r5, #176
	it	eq
	addeq	sl, sl, #1
	cmp	r4, #2816
	it	eq
	addeq	r8, r8, #1
	subs	r1, r0, #16384
	str	ip, [sp, #12]
	rsbs	ip, r1, #0
	adcs	ip, ip, r1
	ldrh	r1, [r3, #42]
	cmp	r0, #8192
	it	eq
	addeq	ip, ip, #1
	and	r7, r9, r7
	bic	r9, r9, #13056
	add	ip, ip, r1
	bic	r9, r9, #51
	cmp	r0, #45056
	it	eq
	addeq	ip, ip, #1
	lsl	r9, r9, #16
	and	r1, r2, #983040
	cmp	r5, #208
	it	eq
	addeq	sl, sl, #1
	cmp	r4, #3328
	it	eq
	addeq	r8, r8, #1
	cmp	r0, #53248
	it	eq
	addeq	ip, ip, #1
	lsr	r9, r9, #16
	lsls	r7, r7, #16
	cmp	r5, #16
	it	eq
	subeq	sl, sl, #1
	cmp	r4, #256
	it	eq
	subeq	r8, r8, #1
	cmp	r0, #4096
	it	eq
	subeq	ip, ip, #1
	orr	r7, r7, r9, lsr #2
	subs	r9, r1, #262144
	str	r7, [sp, #24]
	rsbs	r7, r9, #0
	adcs	r7, r7, r9
	ldrh	r9, [r3, #44]
	cmp	r1, #131072
	it	eq
	addeq	r7, r7, #1
	cmp	r6, #7
	it	eq
	subeq	fp, fp, #1
	cmp	r5, #112
	it	eq
	subeq	sl, sl, #1
	cmp	r6, #8
	it	eq
	subeq	fp, fp, #1
	add	r7, r7, r9
	cmp	r1, #720896
	it	eq
	addeq	r7, r7, #1
	cmp	r5, #128
	it	eq
	subeq	sl, sl, #1
	cmp	r6, #14
	it	eq
	subeq	fp, fp, #1
	ldr	r6, [r3, #76]
	cmp	r5, #224
	it	eq
	subeq	sl, sl, #1
	cmp	r4, #1792
	it	eq
	subeq	r8, r8, #1
	cmp	r0, #28672
	it	eq
	subeq	ip, ip, #1
	cmp	r1, #851968
	it	eq
	addeq	r7, r7, #1
	and	r5, r2, #15728640
	cmp	r4, #2048
	it	eq
	subeq	r8, r8, #1
	cmp	r0, #32768
	it	eq
	subeq	ip, ip, #1
	cmp	r1, #65536
	it	eq
	subeq	r7, r7, #1
	str	sl, [sp, #20]
	lsls	r6, r6, #2
	ldr	sl, [sp, #24]
	cmp	r4, #3584
	it	eq
	subeq	r8, r8, #1
	cmp	r0, #57344
	it	eq
	subeq	ip, ip, #1
	cmp	r1, #458752
	it	eq
	subeq	r7, r7, #1
	str	fp, [sp, #8]
	and	r6, r6, #-858993460
	subs	fp, r5, #4194304
	orr	r6, sl, r6
	rsbs	sl, fp, #0
	str	r8, [sp, #24]
	adcs	sl, sl, fp
	str	ip, [sp, #28]
	ldrh	ip, [r3, #46]
	cmp	r5, #2097152
	it	eq
	addeq	sl, sl, #1
	add	sl, sl, ip
	and	r4, r2, #251658240
	cmp	r5, #11534336
	it	eq
	addeq	sl, sl, #1
	cmp	r1, #524288
	it	eq
	subeq	r7, r7, #1
	cmp	r5, #13631488
	it	eq
	addeq	sl, sl, #1
	subs	ip, r4, #67108864
	rsbs	r9, ip, #0
	adcs	r9, r9, ip
	ldrh	ip, [r3, #48]
	cmp	r4, #33554432
	it	eq
	addeq	r9, r9, #1
	add	r9, r9, ip
	cmp	r4, #184549376
	it	eq
	addeq	r9, r9, #1
	cmp	r4, #218103808
	it	eq
	addeq	r9, r9, #1
	and	r0, r2, #-268435456
	cmp	r5, #1048576
	it	eq
	subeq	sl, sl, #1
	cmp	r4, #16777216
	it	eq
	subeq	r9, r9, #1
	cmp	r5, #7340032
	it	eq
	subeq	sl, sl, #1
	cmp	r4, #117440512
	it	eq
	subeq	r9, r9, #1
	subs	fp, r0, #1073741824
	rsbs	r8, fp, #0
	adcs	r8, r8, fp
	ldrh	ip, [r3, #50]
	cmp	r0, #536870912
	it	eq
	addeq	r8, r8, #1
	add	r8, r8, ip
	cmp	r0, #-1342177280
	it	eq
	addeq	r8, r8, #1
	cmp	r0, #-805306368
	it	eq
	addeq	r8, r8, #1
	cmp	r0, #268435456
	it	eq
	subeq	r8, r8, #1
	and	fp, r6, #15
	cmp	r0, #1879048192
	it	eq
	subeq	r8, r8, #1
	cmp	r5, #8388608
	it	eq
	subeq	sl, sl, #1
	cmp	r4, #134217728
	it	eq
	subeq	r9, r9, #1
	cmp	r0, #-2147483648
	it	eq
	subeq	r8, r8, #1
	cmp	r1, #917504
	it	eq
	subeq	r7, r7, #1
	cmp	r5, #14680064
	it	eq
	subeq	sl, sl, #1
	subs	r1, fp, #4
	rsbs	r5, r1, #0
	adcs	r5, r5, r1
	ldrh	ip, [r3, #52]
	cmp	fp, #2
	it	eq
	addeq	r5, r5, #1
	add	ip, r5, ip
	cmp	fp, #11
	it	eq
	addeq	ip, ip, #1
	cmp	fp, #13
	it	eq
	addeq	ip, ip, #1
	cmp	fp, #1
	it	eq
	subeq	ip, ip, #1
	cmp	fp, #7
	it	eq
	subeq	ip, ip, #1
	and	r5, r6, #240
	cmp	fp, #8
	it	eq
	subeq	ip, ip, #1
	cmp	r4, #234881024
	it	eq
	subeq	r9, r9, #1
	cmp	r0, #-536870912
	it	eq
	subeq	r8, r8, #1
	cmp	fp, #14
	it	eq
	subeq	ip, ip, #1
	str	r7, [sp, #0]
	subs	r7, r5, #64
	rsbs	fp, r7, #0
	adcs	fp, fp, r7
	ldrh	r1, [r3, #54]
	cmp	r5, #32
	it	eq
	addeq	fp, fp, #1
	add	fp, fp, r1
	cmp	r5, #176
	it	eq
	addeq	fp, fp, #1
	cmp	r5, #208
	it	eq
	addeq	fp, fp, #1
	and	r4, r6, #3840
	ldr	r7, [sp, #4]
	cmp	r5, #16
	it	eq
	subeq	fp, fp, #1
	cmp	r5, #112
	it	eq
	subeq	fp, fp, #1
	subs	r1, r4, #1024
	strh	r7, [r3, #30]	@ movhi
	rsbs	r7, r1, #0
	adcs	r7, r7, r1
	cmp	r5, #128
	it	eq
	subeq	fp, fp, #1
	cmp	r5, #224
	ite	ne
	movne	r5, fp
	subeq	r5, fp, #1
	ldrh	r1, [r3, #56]
	ldr	fp, [sp, #12]
	cmp	r4, #512
	it	eq
	addeq	r7, r7, #1
	strh	fp, [r3, #32]	@ movhi
	add	fp, r7, r1
	cmp	r4, #2816
	it	eq
	addeq	fp, fp, #1
	and	r0, r6, #61440
	cmp	r4, #3328
	it	eq
	addeq	fp, fp, #1
	cmp	r4, #256
	it	eq
	subeq	fp, fp, #1
	subs	r1, r0, #16384
	rsbs	r7, r1, #0
	adcs	r7, r7, r1
	ldrh	r1, [r3, #58]
	cmp	r0, #8192
	it	eq
	addeq	r7, r7, #1
	adds	r7, r7, r1
	cmp	r0, #45056
	it	eq
	addeq	r7, r7, #1
	cmp	r0, #53248
	it	eq
	addeq	r7, r7, #1
	cmp	r0, #4096
	it	eq
	subeq	r7, r7, #1
	cmp	r0, #28672
	it	eq
	subeq	r7, r7, #1
	cmp	r0, #32768
	it	eq
	subeq	r7, r7, #1
	cmp	r0, #57344
	ite	ne
	movne	r0, r7
	subeq	r0, r7, #1
	cmp	r4, #1792
	it	eq
	subeq	fp, fp, #1
	ldr	r7, [sp, #16]
	cmp	r4, #2048
	it	eq
	subeq	fp, fp, #1
	cmp	r4, #3584
	ite	ne
	movne	r4, fp
	subeq	r4, fp, #1
	ldr	r1, [sp, #20]
	ldr	fp, [sp, #8]
	strh	r7, [r3, #34]	@ movhi
	ldr	r7, [sp, #24]
	str	r2, [r3, #72]
	and	r2, r6, #983040
	strh	fp, [r3, #36]	@ movhi
	strh	r1, [r3, #38]	@ movhi
	strh	r7, [r3, #40]	@ movhi
	ldr	r1, [sp, #0]
	subs	r7, r2, #262144
	ldr	fp, [sp, #28]
	strh	r4, [r3, #56]	@ movhi
	rsbs	r4, r7, #0
	adcs	r4, r4, r7
	strh	r9, [r3, #48]	@ movhi
	str	r6, [r3, #76]
	strh	r1, [r3, #44]	@ movhi
	strh	r8, [r3, #50]	@ movhi
	strh	ip, [r3, #52]	@ movhi
	strh	r5, [r3, #54]	@ movhi
	strh	fp, [r3, #42]	@ movhi
	strh	sl, [r3, #46]	@ movhi
	strh	r0, [r3, #58]	@ movhi
	ldrh	r1, [r3, #60]
	cmp	r2, #131072
	it	eq
	addeq	r4, r4, #1
	adds	r4, r4, r1
	cmp	r2, #720896
	it	eq
	addeq	r4, r4, #1
	cmp	r2, #851968
	it	eq
	addeq	r4, r4, #1
	cmp	r2, #65536
	it	eq
	subeq	r4, r4, #1
	cmp	r2, #458752
	it	eq
	subeq	r4, r4, #1
	and	r0, r6, #15728640
	cmp	r2, #524288
	it	eq
	subeq	r4, r4, #1
	cmp	r2, #917504
	ite	ne
	movne	r2, r4
	subeq	r2, r4, #1
	subs	ip, r0, #4194304
	and	r1, r6, #251658240
	rsbs	r5, ip, #0
	adcs	r5, r5, ip
	subs	r7, r1, #67108864
	and	r6, r6, #-268435456
	rsbs	r4, r7, #0
	adcs	r4, r4, r7
	cmp	r6, #1073741824
	ite	ne
	movne	r9, #0
	moveq	r9, #1
	ldrh	r8, [r3, #62]
	ldrh	ip, [r3, #64]
	ldrh	r7, [r3, #66]
	cmp	r0, #2097152
	it	eq
	addeq	r5, r5, #1
	cmp	r1, #33554432
	it	eq
	addeq	r4, r4, #1
	cmp	r6, #536870912
	it	eq
	addeq	r9, r9, #1
	add	r5, r5, r8
	add	r4, r4, ip
	add	r9, r9, r7
	cmp	r0, #11534336
	it	eq
	addeq	r5, r5, #1
	cmp	r1, #184549376
	it	eq
	addeq	r4, r4, #1
	cmp	r6, #-1342177280
	it	eq
	addeq	r9, r9, #1
	cmp	r0, #13631488
	it	eq
	addeq	r5, r5, #1
	cmp	r1, #218103808
	it	eq
	addeq	r4, r4, #1
	cmp	r6, #-805306368
	it	eq
	addeq	r9, r9, #1
	cmp	r0, #1048576
	it	eq
	subeq	r5, r5, #1
	cmp	r1, #16777216
	it	eq
	subeq	r4, r4, #1
	cmp	r6, #268435456
	it	eq
	subeq	r9, r9, #1
	cmp	r0, #7340032
	it	eq
	subeq	r5, r5, #1
	cmp	r1, #117440512
	it	eq
	subeq	r4, r4, #1
	cmp	r6, #1879048192
	it	eq
	subeq	r9, r9, #1
	cmp	r0, #8388608
	it	eq
	subeq	r5, r5, #1
	cmp	r1, #134217728
	it	eq
	subeq	r4, r4, #1
	cmp	r6, #-2147483648
	it	eq
	subeq	r9, r9, #1
	cmp	r0, #14680064
	ite	ne
	movne	r0, r5
	subeq	r0, r5, #1
	cmp	r1, #234881024
	ite	ne
	movne	r1, r4
	subeq	r1, r4, #1
	cmp	r6, #-536870912
	ite	ne
	movne	r6, r9
	subeq	r6, r9, #1
	strh	r2, [r3, #60]	@ movhi
	strh	r0, [r3, #62]	@ movhi
	strh	r1, [r3, #64]	@ movhi
	strh	r6, [r3, #66]	@ movhi
	add	sp, sp, #32
	pop	{r4, r5, r6, r7, r8, r9, sl, fp}
	bx	lr
	.size	SysTick_Handler, .-SysTick_Handler
	.align	2
	.global	setCountersToZero
	.thumb
	.thumb_func
	.type	setCountersToZero, %function
setCountersToZero:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movw	r3, #:lower16:.LANCHOR0
	movt	r3, #:upper16:.LANCHOR0
	movs	r2, #0
	str	r2, [r3, #4]
	str	r2, [r3, #8]
	str	r2, [r3, #12]
	str	r2, [r3, #16]
	str	r2, [r3, #20]
	str	r2, [r3, #24]
	str	r2, [r3, #28]
	str	r2, [r3, #32]
	str	r2, [r3, #36]
	str	r2, [r3, #40]
	str	r2, [r3, #44]
	str	r2, [r3, #48]
	str	r2, [r3, #52]
	str	r2, [r3, #56]
	str	r2, [r3, #60]
	str	r2, [r3, #64]
	str	r2, [r3, #80]
	str	r2, [r3, #84]
	str	r2, [r3, #88]
	str	r2, [r3, #92]
	str	r2, [r3, #96]
	str	r2, [r3, #100]
	str	r2, [r3, #104]
	str	r2, [r3, #108]
	bx	lr
	.size	setCountersToZero, .-setCountersToZero
	.align	2
	.global	EXTI0_IRQHandler
	.thumb
	.thumb_func
	.type	EXTI0_IRQHandler, %function
EXTI0_IRQHandler:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #1
	b	EXTI_ClearITPendingBit
	.size	EXTI0_IRQHandler, .-EXTI0_IRQHandler
	.align	2
	.global	EXTI1_IRQHandler
	.thumb
	.thumb_func
	.type	EXTI1_IRQHandler, %function
EXTI1_IRQHandler:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #2
	b	EXTI_ClearITPendingBit
	.size	EXTI1_IRQHandler, .-EXTI1_IRQHandler
	.align	2
	.global	OTG_FS_WKUP_IRQHandler
	.thumb
	.thumb_func
	.type	OTG_FS_WKUP_IRQHandler, %function
OTG_FS_WKUP_IRQHandler:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	ldr	r4, .L25
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cbz	r3, .L21
	mov	r3, #60672
	movt	r3, 57344
	ldr	r2, [r3, #16]
	bic	r2, r2, #6
	str	r2, [r3, #16]
	bl	SystemInit
	mov	r0, r4
	bl	USB_OTG_UngateClock
.L21:
	mov	r0, #262144
	pop	{r4, lr}
	b	EXTI_ClearITPendingBit
.L26:
	.align	2
.L25:
	.word	USB_OTG_dev
	.size	OTG_FS_WKUP_IRQHandler, .-OTG_FS_WKUP_IRQHandler
	.align	2
	.global	OTG_FS_IRQHandler
	.thumb
	.thumb_func
	.type	OTG_FS_IRQHandler, %function
OTG_FS_IRQHandler:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movw	r0, #:lower16:USB_OTG_dev
	movt	r0, #:upper16:USB_OTG_dev
	b	USBD_OTG_ISR_Handler
	.size	OTG_FS_IRQHandler, .-OTG_FS_IRQHandler
	.global	chunkF
	.global	chunkE
	.global	chunkD
	.global	chunkC
	.global	angle
	.global	remote_wakeup
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"HardFault_Handler\015\000"
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	chunkC, %object
	.size	chunkC, 4
chunkC:
	.space	4
	.type	angle, %object
	.size	angle, 64
angle:
	.space	64
	.type	chunkD, %object
	.size	chunkD, 4
chunkD:
	.space	4
	.type	chunkE, %object
	.size	chunkE, 4
chunkE:
	.space	4
	.type	chunkF, %object
	.size	chunkF, 4
chunkF:
	.space	4
	.type	abCe, %object
	.size	abCe, 4
abCe:
	.space	4
	.type	abCo, %object
	.size	abCo, 4
abCo:
	.space	4
	.type	abDe, %object
	.size	abDe, 4
abDe:
	.space	4
	.type	abDo, %object
	.size	abDo, 4
abDo:
	.space	4
	.type	abEe, %object
	.size	abEe, 4
abEe:
	.space	4
	.type	abEo, %object
	.size	abEo, 4
abEo:
	.space	4
	.type	abFe, %object
	.size	abFe, 4
abFe:
	.space	4
	.type	abFo, %object
	.size	abFo, 4
abFo:
	.space	4
	.type	remote_wakeup, %object
	.size	remote_wakeup, 4
remote_wakeup:
	.space	4
	.ident	"GCC: (crosstool-NG 1.18.0 - iwasz.pl) 4.7.3 20130102 (prerelease)"
