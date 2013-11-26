	.text
.globl _receive_mouse_packet
_receive_mouse_packet:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	leal	-12(%ebp), %eax
	movl	$16, 8(%esp)
	movl	%eax, 4(%esp)
	movl	$96, (%esp)
	call	_sys_in
	movl	_packet_nr, %eax
	movl	-12(%ebp), %edx
	movl	%edx, _packet(,%eax,4)
	movl	_packet_nr, %eax
	testl	%eax, %eax
	jne	L2
	movl	_packet_nr, %eax
	movl	_packet(,%eax,4), %eax
	andl	$8, %eax
	testl	%eax, %eax
	je	L7
	movl	_packet_nr, %eax
	addl	$1, %eax
	movl	%eax, _packet_nr
	jmp	L6
L2:
	movl	_packet_nr, %eax
	cmpl	$1, %eax
	jne	L5
	movl	_packet_nr, %eax
	addl	$1, %eax
	movl	%eax, _packet_nr
	jmp	L6
L5:
	movl	_packet_nr, %eax
	cmpl	$2, %eax
	jne	L6
	movl	$1, _valid_packets
	jmp	L6
L7:
	nop
L6:
	leave
	ret
