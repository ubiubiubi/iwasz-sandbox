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
	movw	r0, #:lower16:.LC0
	push	{r3, lr}
	movt	r0, #:upper16:.LC0
	bl	puts
.L3:
	b	.L3
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
.L7:
	b	.L7
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
.L9:
	b	.L9
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
.L11:
	b	.L11
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
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, r7, r8, r9, r10, fp, lr}
	mov	r2, #2048
	movt	r2, 16386
	ldr	r5, [r2, #16]
	movw	r3, #:lower16:.LANCHOR0
	mov	r2, #3072
	movt	r3, #:upper16:.LANCHOR0
	movt	r2, 16386
	ldr	r2, [r2, #16]
	ldr	r0, [r3]
	sub	sp, sp, #28
	movw	r1, #52428
	ldr	r6, [r3, #68]
	movw	r7, #13107
	str	r2, [sp, #4]
	ands	r7, r7, r5
	and	r4, r1, r0, lsl #2
	orrs	r4, r4, r7
	ldr	r0, [sp, #4]
	and	r6, r1, r6, lsl #2
	ands	r5, r5, r1
	movw	r2, #:lower16:.LANCHOR1
	movt	r2, #:upper16:.LANCHOR1
	orr	r5, r6, r5, lsr #2
	lsr	lr, r4, #12
	ldr	r6, [r3, #72]
	movw	r7, #13107
	ldrh	r10, [r3, #10]
	ldrsb	lr, [r2, lr]
	ands	r7, r7, r0
	orr	r0, r7, r6, lsl #2
	lsrs	r6, r5, #12
	add	lr, lr, r10
	ldrsb	r10, [r2, r6]
	and	r6, r0, #15
	ldrsb	r6, [r2, r6]
	ubfx	r7, r5, #8, #4
	ldrsb	fp, [r2, r7]
	str	r6, [sp, #8]
	mov	r7, #5120
	mov	r6, #4096
	ubfx	r8, r4, #8, #4
	movt	r6, 16386
	movt	r7, 16386
	ldr	r6, [r6, #16]
	ldrsb	r9, [r2, r8]
	ldr	r7, [r7, #16]
	ldrh	r8, [r3, #12]
	ubfx	ip, r5, #4, #4
	ldrsb	ip, [r2, ip]
	add	r9, r9, r8
	str	r7, [sp]
	ldrh	r8, [r3, #14]
	ldrh	r7, [r3, #18]
	uxth	lr, lr
	add	lr, lr, ip
	add	fp, fp, r8
	add	r10, r10, r7
	ldr	r8, [sp, #8]
	ldrh	r7, [r3, #20]
	str	r4, [r3]
	str	r5, [r3, #68]
	strh	lr, [r3, #10]	@ movhi
	ubfx	ip, r0, #4, #4
	and	lr, r4, #15
	and	r5, r5, #15
	ubfx	r4, r4, #4, #4
	ldrsb	ip, [r2, ip]
	strh	r9, [r3, #12]	@ movhi
	add	r8, r8, r7
	ldrsb	r9, [r2, r5]
	ldrh	r7, [r3, #24]
	strh	fp, [r3, #14]	@ movhi
	strh	r10, [r3, #18]	@ movhi
	str	r0, [r3, #72]
	ldrsb	r4, [r2, r4]
	add	ip, ip, r7
	str	r4, [sp, #12]
	ldr	r7, [r3, #76]
	ldr	r4, [sp, #4]
	ldrsb	r10, [r2, lr]
	ldr	r5, [r3, #80]
	and	lr, r4, r1
	lsls	r4, r7, #2
	orr	r4, r4, lr, lsr #2
	movw	lr, #13107
	and	lr, r6, lr
	and	r5, r1, r5, lsl #2
	orr	r5, r5, lr
	ubfx	lr, r5, #4, #4
	strh	r8, [r3, #20]	@ movhi
	ldrsb	lr, [r2, lr]
	ldrh	r8, [r3, #6]
	uxth	r7, r9
	add	lr, lr, r8
	add	lr, lr, r7
	ubfx	r7, r0, #8, #4
	ubfx	r0, r0, #12, #4
	ldrsb	r0, [r2, r0]
	ldrsb	r8, [r2, r7]
	str	r0, [sp, #16]
	ldr	r0, [r3, #84]
	strh	ip, [r3, #24]	@ movhi
	and	r7, r1, r0, lsl #2
	and	ip, r6, r1
	and	r0, r4, #15
	orr	r7, r7, ip, lsr #2
	ldrsb	ip, [r2, r0]
	ubfx	r0, r4, #4, #4
	ldrsb	r0, [r2, r0]
	strh	lr, [r3, #6]	@ movhi
	str	r0, [sp, #20]
	ubfx	r0, r4, #8, #4
	ldrsb	lr, [r2, r0]
	ubfx	r0, r4, #12, #4
	ldrsb	r6, [r2, r0]
	and	r0, r5, #15
	ldrsb	r9, [r2, r0]
	str	r5, [r3, #80]
	ubfx	r0, r5, #8, #4
	lsrs	r5, r5, #12
	str	r4, [r3, #76]
	ldrsb	r4, [r2, r5]
	ldr	r5, [sp]
	ldrsb	fp, [r2, r0]
	movw	r0, #13107
	ands	r0, r0, r5
	ands	r1, r1, r5
	ldrh	r5, [r3, #4]
	add	r5, r5, r10
	ldrh	r10, [r3, #28]
	str	r5, [sp, #4]
	add	r8, r8, r10
	ldr	r5, [sp, #12]
	ldrh	r10, [r3, #8]
	strh	r8, [r3, #28]	@ movhi
	add	r5, r5, r10
	ldrh	r8, [r3, #32]
	str	r5, [sp, #12]
	ldr	r5, [sp, #16]
	ldrh	r10, [r3, #30]
	add	r8, r8, r5
	ldrh	r5, [r3, #22]
	strh	r8, [r3, #32]	@ movhi
	add	ip, ip, r5
	ldr	r8, [sp, #20]
	ldrh	r5, [r3, #26]
	add	lr, lr, r10
	add	r5, r5, r8
	ldrh	r8, [r3, #48]
	strh	r5, [r3, #26]	@ movhi
	ldr	r5, [r3, #92]
	add	r4, r4, r8
	ldrh	r10, [r3, #34]
	strh	r4, [r3, #48]	@ movhi
	lsls	r5, r5, #2
	ldr	r4, [r3, #88]
	orr	r1, r5, r1, lsr #2
	orr	r0, r0, r4, lsl #2
	add	r6, r6, r10
	and	r8, r1, #15
	ldrh	r10, [r3, #36]
	ldrsb	r8, [r2, r8]
	ubfx	r4, r0, #12, #4
	ldrsb	r4, [r2, r4]
	add	r9, r9, r10
	ldrh	r10, [r3, #44]
	str	r8, [sp]
	ubfx	r8, r1, #4, #4
	ldrsb	r8, [r2, r8]
	add	fp, fp, r10
	strh	lr, [r3, #30]	@ movhi
	ldr	r10, [sp, #4]
	str	r4, [sp, #16]
	and	r5, r7, #15
	ldr	r4, [sp, #12]
	ubfx	lr, r7, #4, #4
	ldrsb	r5, [r2, r5]
	ldrsb	lr, [r2, lr]
	str	r8, [sp, #8]
	uxth	r8, r10
	uxth	r10, r4
	strh	ip, [r3, #22]	@ movhi
	strh	r6, [r3, #34]	@ movhi
	strh	r9, [r3, #36]	@ movhi
	strh	fp, [r3, #44]	@ movhi
	str	r7, [r3, #84]
	add	r8, r8, r5
	add	lr, r10, lr
	ubfx	fp, r7, #8, #4
	and	r9, r0, #15
	ubfx	r6, r0, #4, #4
	ubfx	ip, r0, #8, #4
	ubfx	r5, r1, #8, #4
	lsrs	r7, r7, #12
	ldrsb	fp, [r2, fp]
	ldrsb	r7, [r2, r7]
	ldrsb	r6, [r2, r6]
	ldrsb	r9, [r2, r9]
	ldrsb	ip, [r2, ip]
	ldrsb	r5, [r2, r5]
	str	lr, [sp, #4]
	ldrh	lr, [r3, #46]
	ldrh	r10, [r3, #50]
	add	lr, fp, lr
	str	lr, [sp, #12]
	strh	r8, [r3, #4]	@ movhi
	ldrh	lr, [r3, #52]
	ldrh	r8, [r3, #56]
	add	r10, r10, r7
	add	r9, r9, lr
	ldrh	r7, [r3, #60]
	add	r8, r8, r6
	ldrh	lr, [r3, #64]
	ldr	r6, [sp, #16]
	add	ip, ip, r7
	add	lr, lr, r6
	ldrh	r7, [r3, #54]
	ldr	fp, [sp]
	ldrh	r6, [r3, #58]
	ldr	r4, [sp, #8]
	add	r7, r7, fp
	add	r6, r6, r4
	ldrh	fp, [r3, #62]
	ubfx	r4, r1, #12, #4
	str	r1, [r3, #92]
	ldrsb	r1, [r2, r4]
	ldrh	r2, [r3, #66]
	add	r5, r5, fp
	str	r0, [r3, #88]
	ldr	fp, [sp, #4]
	ldr	r0, [sp, #12]
	add	r2, r2, r1
	strh	fp, [r3, #8]	@ movhi
	strh	r0, [r3, #46]	@ movhi
	strh	r10, [r3, #50]	@ movhi
	strh	r9, [r3, #52]	@ movhi
	strh	r8, [r3, #56]	@ movhi
	strh	ip, [r3, #60]	@ movhi
	strh	lr, [r3, #64]	@ movhi
	strh	r7, [r3, #54]	@ movhi
	strh	r6, [r3, #58]	@ movhi
	strh	r5, [r3, #62]	@ movhi
	strh	r2, [r3, #66]	@ movhi
	add	sp, sp, #28
	@ sp needed
	pop	{r4, r5, r6, r7, r8, r9, r10, fp, pc}
	.size	SysTick_Handler, .-SysTick_Handler
	.align	2
	.global	setCountersToZero
	.thumb
	.thumb_func
	.type	setCountersToZero, %function
setCountersToZero:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
	movw	r4, #:lower16:.LANCHOR0
	movt	r4, #:upper16:.LANCHOR0
	adds	r0, r4, #4
	movs	r1, #0
	movs	r2, #64
	bl	memset
	movs	r3, #0
	str	r3, [r4]
	str	r3, [r4, #68]
	str	r3, [r4, #72]
	str	r3, [r4, #76]
	str	r3, [r4, #80]
	str	r3, [r4, #84]
	str	r3, [r4, #88]
	str	r3, [r4, #92]
	pop	{r4, pc}
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
	movw	r4, #:lower16:USB_OTG_dev
	movt	r4, #:upper16:USB_OTG_dev
	ldrb	r3, [r4, #10]	@ zero_extendqisi2
	cbz	r3, .L22
	mov	r3, #60672
	movt	r3, 57344
	ldr	r2, [r3, #16]
	bic	r2, r2, #6
	str	r2, [r3, #16]
	bl	SystemInit
	mov	r0, r4
	bl	USB_OTG_UngateClock
.L22:
	pop	{r4, lr}
	mov	r0, #262144
	b	EXTI_ClearITPendingBit
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
	.global	angle
	.global	remote_wakeup
	.section	.rodata
	.align	2
	.set	.LANCHOR1,. + 0
	.type	lookup, %object
	.size	lookup, 16
lookup:
	.byte	0
	.byte	-1
	.byte	1
	.byte	0
	.byte	1
	.byte	0
	.byte	0
	.byte	-1
	.byte	-1
	.byte	0
	.byte	0
	.byte	1
	.byte	0
	.byte	1
	.byte	-1
	.byte	0
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"HardFault_Handler\015\000"
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	abCe, %object
	.size	abCe, 4
abCe:
	.space	4
	.type	angle, %object
	.size	angle, 64
angle:
	.space	64
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
	.ident	"GCC: (crosstool-NG 1.19.0 - iwasz.pl) 4.8.2 20130603 (prerelease)"
