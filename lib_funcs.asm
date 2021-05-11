    strlen : 
		subu $fp, $fp, $sp
		sw $sp, -8($fp) 	 # preserve func size
		sw $ra, -4($fp) 	 # preserve return address
		move $s0, $a0	 # get value of str
		li $s1, 0	 # _tmp_1 = 0
		move $s2, $s1	 # i = _tmp_1
		sw $s0, -48($fp)
		sw $s1, -16($fp)
		sw $s2, -44($fp)
	_lib_label_3 :
		lw $s1, -44($fp)
		mul $s0, $s1, 1	 # _tmp_2 = i *int 1
		lw $s3, -48($fp)
		addu $s2, $s3, $s0	 # _tmp_3 = str +ptr _tmp_2
		lb $s4, ($s2)	 # _tmp_4 = *_tmp_3
		li $s5, 0	 # _tmp_5 = 0
		move $s6, $s4	 # _tmp_6 = char_to_int _tmp_4
		sne $s7, $s6, $s5	 # _tmp_7 = _tmp_6 !=int _tmp_5? 1 : 0
		move $a0, $s7
		sw $s0, -20($fp)
		sw $s1, -44($fp)
		sw $s2, -24($fp)
		sw $s3, -48($fp)
		sb $s4, -9($fp)
		sw $s5, -28($fp)
		sw $s6, -32($fp)
		sb $s7, -10($fp)
		bnez $a0, _lib_label_11
		j _lib_label_15
	_lib_label_11 :
		li $s0, 1	 # _tmp_8 = 1
		lw $s2, -44($fp)
		add $s1, $s2, $s0	 # _tmp_9 = i +int _tmp_8
		move $s2, $s1	 # i = _tmp_9
		sw $s0, -36($fp)
		sw $s1, -40($fp)
		sw $s2, -44($fp)
		j _lib_label_3
	_lib_label_15 :
		lw $s0, -44($fp)
		move $v0, $s0
		j _strlen_end
	_strlen_end : 
		sw $s0, -44($fp)
		lw $sp, -8($fp) 	 # load caller func size
		lw $ra, -4($fp) 	 # restore return address
		addu $fp, $fp, $sp 	 # restore frame pointer
		jr $ra 	 # Return
	isqrt : 
		subu $fp, $fp, $sp
		sw $sp, -8($fp) 	 # preserve func size
		sw $ra, -4($fp) 	 # preserve return address
		move $s0, $a0	 # get value of num
		li $s1, 0	 # _tmp_10 = 0
		move $s2, $s1	 # ret = _tmp_10
		li $s3, 1	 # _tmp_11 = 1
		li $s4, 30	 # _tmp_12 = 30
		sll $s5, $s3, $s4	 # _tmp_13 = _tmp_11 << _tmp_12
		move $s6, $s5	 # bit = _tmp_13
		li $s7, 0	 # _tmp_14 = 0
		slt $t0, $s0, $s7	 # _tmp_15 = num <int _tmp_14? 1 : 0
		move $a0, $t0
		sw $s0, -96($fp)
		sw $s1, -16($fp)
		sw $s2, -100($fp)
		sw $s3, -20($fp)
		sw $s4, -24($fp)
		sw $s5, -28($fp)
		sw $s6, -92($fp)
		sw $s7, -32($fp)
		sb $t0, -9($fp)
		bnez $a0, _lib_label_28
		j _lib_label_30
	_lib_label_28 :
		li $s0, 0	 # _tmp_16 = 0
		move $v0, $s0
		j _isqrt_end
		sw $s0, -36($fp)
	_lib_label_30 :
		lw $s0, -96($fp)
		lw $s1, -92($fp)
		slt $s2, $s0, $s1	 # _tmp_17 = num <int bit? 1 : 0
		move $a0, $s2
		sw $s0, -96($fp)
		sw $s1, -92($fp)
		sb $s2, -10($fp)
		bnez $a0, _lib_label_33
		j _lib_label_37
	_lib_label_33 :
		li $s0, 2	 # _tmp_18 = 2
		lw $s1, -92($fp)
		srl $s2, $s1, $s0	 # _tmp_19 = bit >> _tmp_18
		move $s1, $s2	 # bit = _tmp_19
		sw $s0, -40($fp)
		sw $s1, -92($fp)
		sw $s2, -44($fp)
		j _lib_label_30
	_lib_label_37 :
		li $s0, 0	 # _tmp_20 = 0
		lw $s1, -92($fp)
		sne $s2, $s1, $s0	 # _tmp_21 = bit !=int _tmp_20? 1 : 0
		move $a0, $s2
		sw $s0, -48($fp)
		sw $s1, -92($fp)
		sb $s2, -11($fp)
		bnez $a0, _lib_label_41
		j _lib_label_60
	_lib_label_41 :
		lw $s1, -100($fp)
		lw $s2, -92($fp)
		add $s0, $s1, $s2	 # _tmp_22 = ret +int bit
		lw $s3, -96($fp)
		slt $s4, $s3, $s0	 # _tmp_23 = num <int _tmp_22? 1 : 0
		move $a0, $s4
		sw $s0, -52($fp)
		sw $s1, -100($fp)
		sw $s2, -92($fp)
		sw $s3, -96($fp)
		sb $s4, -12($fp)
		bnez $a0, _lib_label_45
		j _lib_label_49
	_lib_label_45 :
		li $s0, 1	 # _tmp_24 = 1
		lw $s1, -100($fp)
		srl $s2, $s1, $s0	 # _tmp_25 = ret >> _tmp_24
		move $s1, $s2	 # ret = _tmp_25
		sw $s0, -56($fp)
		sw $s1, -100($fp)
		sw $s2, -60($fp)
		j _lib_label_56
	_lib_label_49 :
		lw $s1, -100($fp)
		lw $s2, -92($fp)
		add $s0, $s1, $s2	 # _tmp_26 = ret +int bit
		lw $s4, -96($fp)
		sub $s3, $s4, $s0	 # _tmp_27 = num -int _tmp_26
		move $s4, $s3	 # num = _tmp_27
		li $s5, 1	 # _tmp_28 = 1
		srl $s6, $s1, $s5	 # _tmp_29 = ret >> _tmp_28
		add $s7, $s6, $s2	 # _tmp_30 = _tmp_29 +int bit
		move $s1, $s7	 # ret = _tmp_30
		sw $s0, -64($fp)
		sw $s1, -100($fp)
		sw $s2, -92($fp)
		sw $s3, -68($fp)
		sw $s4, -96($fp)
		sw $s5, -72($fp)
		sw $s6, -76($fp)
		sw $s7, -80($fp)
	_lib_label_56 :
		li $s0, 2	 # _tmp_31 = 2
		lw $s1, -92($fp)
		srl $s2, $s1, $s0	 # _tmp_32 = bit >> _tmp_31
		move $s1, $s2	 # bit = _tmp_32
		sw $s0, -84($fp)
		sw $s1, -92($fp)
		sw $s2, -88($fp)
		j _lib_label_37
	_lib_label_60 :
		lw $s0, -100($fp)
		move $v0, $s0
		j _isqrt_end
	_isqrt_end : 
		sw $s0, -100($fp)
		lw $sp, -8($fp) 	 # load caller func size
		lw $ra, -4($fp) 	 # restore return address
		addu $fp, $fp, $sp 	 # restore frame pointer
		jr $ra 	 # Return
	log10 : 
		subu $fp, $fp, $sp
		sw $sp, -8($fp) 	 # preserve func size
		sw $ra, -4($fp) 	 # preserve return address
		mov.s $f10, $f12	 # get value of x
		li.s $f20, 0.000000	 # _tmp_33 = 0.000000
		mov.s $f22, $f20	 # old_sum = _tmp_33
		li $s0, 1	 # _tmp_34 = 1
		mtc1 $s0, $f24
		cvt.s.w $f24, $f24
		sub.s $f26, $f10, $f24	 # _tmp_36 = x -float _tmp_35
		li $s1, 1	 # _tmp_37 = 1
		mtc1 $s1, $f28
		cvt.s.w $f28, $f28
		add.s $f30, $f10, $f28	 # _tmp_39 = x +float _tmp_38
		div.s $f4, $f26, $f30	 # _tmp_40 = _tmp_36 /float _tmp_39
		mov.s $f6, $f4	 # xmlxpl = _tmp_40
		mul.s $f8, $f6, $f6	 # _tmp_41 = xmlxpl *float xmlxpl
		swc1 $f10, -112($fp)
		mov.s $f10, $f8	 # xmlxpl_2 = _tmp_41
		swc1 $f20, -16($fp)
		li.s $f20, 1.000000	 # _tmp_42 = 1.000000
		swc1 $f22, -100($fp)
		mov.s $f22, $f20	 # denom = _tmp_42
		swc1 $f24, -24($fp)
		mov.s $f24, $f6	 # frac = xmlxpl
		swc1 $f26, -28($fp)
		mov.s $f26, $f24	 # term = frac
		swc1 $f28, -36($fp)
		mov.s $f28, $f26	 # sum = term
		swc1 $f10, -120($fp)
		swc1 $f20, -52($fp)
		swc1 $f22, -92($fp)
		swc1 $f24, -96($fp)
		swc1 $f26, -108($fp)
		swc1 $f28, -104($fp)
		swc1 $f30, -40($fp)
		swc1 $f4, -44($fp)
		swc1 $f6, -116($fp)
		swc1 $f8, -48($fp)
		sw $s0, -20($fp)
		sw $s1, -32($fp)
	_lib_label_80 :
		lwc1 $f10, -104($fp)
		lwc1 $f20, -100($fp)
		c.eq.s $f10, $f20	 # sum !=float old_sum ?
		bc1t _fp_cond_true_0
		li $s0, 1
		b _fp_cond_end_0
	_fp_cond_true_0 : 
		li $s0, 0
	_fp_cond_end_0 : 
		move $a0, $s0
		swc1 $f10, -104($fp)
		swc1 $f20, -100($fp)
		sb $s0, -9($fp)
		bnez $a0, _lib_label_83
		j _lib_label_93
	_lib_label_83 :
		lwc1 $f20, -104($fp)
		mov.s $f10, $f20	 # old_sum = sum
		li.s $f22, 2.000000	 # _tmp_44 = 2.000000
		lwc1 $f26, -92($fp)
		add.s $f24, $f26, $f22	 # _tmp_45 = denom +float _tmp_44
		mov.s $f26, $f24	 # denom = _tmp_45
		lwc1 $f30, -96($fp)
		lwc1 $f4, -120($fp)
		mul.s $f28, $f30, $f4	 # _tmp_46 = frac *float xmlxpl_2
		mov.s $f30, $f28	 # frac = _tmp_46
		div.s $f6, $f30, $f26	 # _tmp_47 = frac /float denom
		add.s $f8, $f20, $f6	 # _tmp_48 = sum +float _tmp_47
		mov.s $f20, $f8	 # sum = _tmp_48
		swc1 $f10, -100($fp)
		swc1 $f20, -104($fp)
		swc1 $f22, -56($fp)
		swc1 $f24, -60($fp)
		swc1 $f26, -92($fp)
		swc1 $f28, -64($fp)
		swc1 $f30, -96($fp)
		swc1 $f4, -120($fp)
		swc1 $f6, -68($fp)
		swc1 $f8, -72($fp)
		j _lib_label_80
	_lib_label_93 :
		li.s $f10, 2.000000	 # _tmp_49 = 2.000000
		lwc1 $f22, -104($fp)
		mul.s $f20, $f10, $f22	 # _tmp_50 = _tmp_49 *float sum
		li.s $f24, 2.303000	 # _tmp_51 = 2.303000
		div.s $f26, $f20, $f24	 # _tmp_52 = _tmp_50 /float _tmp_51
		mov.s $f0, $f26
		j _log10_end
	_log10_end : 
		swc1 $f10, -76($fp)
		swc1 $f20, -80($fp)
		swc1 $f22, -104($fp)
		swc1 $f24, -84($fp)
		swc1 $f26, -88($fp)
		lw $sp, -8($fp) 	 # load caller func size
		lw $ra, -4($fp) 	 # restore return address
		addu $fp, $fp, $sp 	 # restore frame pointer
		jr $ra 	 # Return
	floor : 
		subu $fp, $fp, $sp
		sw $sp, -8($fp) 	 # preserve func size
		sw $ra, -4($fp) 	 # preserve return address
		mov.s $f10, $f12	 # get value of x
		li $s0, 0	 # _tmp_53 = 0
		mtc1 $s0, $f20
		cvt.s.w $f20, $f20
		c.lt.s $f10, $f20	 # x <float _tmp_54 ?
		bc1t _fp_cond_true_1
		li $s1, 0
		b _fp_cond_end_1
	_fp_cond_true_1 : 
		li $s1, 1
	_fp_cond_end_1 : 
		move $a0, $s1
		swc1 $f10, -48($fp)
		swc1 $f20, -20($fp)
		sw $s0, -16($fp)
		sb $s1, -9($fp)
		bnez $a0, _lib_label_105
		j _lib_label_110
	_lib_label_105 :
		lwc1 $f10, -48($fp)
		cvt.w.s $f0, $f10
		mfc1 $s0, $f0
		li $s1, 1	 # _tmp_57 = 1
		sub $s2, $s0, $s1	 # _tmp_58 = _tmp_56 -int _tmp_57
		mtc1 $s2, $f20
		cvt.s.w $f20, $f20
		mov.s $f0, $f20
		j _floor_end
		swc1 $f10, -48($fp)
		swc1 $f20, -36($fp)
		sw $s0, -24($fp)
		sw $s1, -28($fp)
		sw $s2, -32($fp)
	_lib_label_110 :
		lwc1 $f10, -48($fp)
		cvt.w.s $f0, $f10
		mfc1 $s0, $f0
		mtc1 $s0, $f20
		cvt.s.w $f20, $f20
		mov.s $f0, $f20
		j _floor_end
	_floor_end : 
		swc1 $f10, -48($fp)
		swc1 $f20, -44($fp)
		sw $s0, -40($fp)
		lw $sp, -8($fp) 	 # load caller func size
		lw $ra, -4($fp) 	 # restore return address
		addu $fp, $fp, $sp 	 # restore frame pointer
		jr $ra 	 # Return
	ceil : 
		subu $fp, $fp, $sp
		sw $sp, -8($fp) 	 # preserve func size
		sw $ra, -4($fp) 	 # preserve return address
		mov.s $f10, $f12	 # get value of x
		li $s0, 0	 # _tmp_62 = 0
		mtc1 $s0, $f20
		cvt.s.w $f20, $f20
		c.lt.s $f10, $f20	 # x <float _tmp_63 ?
		bc1t _fp_cond_true_2
		li $s1, 0
		b _fp_cond_end_2
	_fp_cond_true_2 : 
		li $s1, 1
	_fp_cond_end_2 : 
		move $a0, $s1
		swc1 $f10, -48($fp)
		swc1 $f20, -20($fp)
		sw $s0, -16($fp)
		sb $s1, -9($fp)
		bnez $a0, _lib_label_120
		j _lib_label_123
	_lib_label_120 :
		lwc1 $f10, -48($fp)
		cvt.w.s $f0, $f10
		mfc1 $s0, $f0
		mtc1 $s0, $f20
		cvt.s.w $f20, $f20
		mov.s $f0, $f20
		j _ceil_end
		swc1 $f10, -48($fp)
		swc1 $f20, -28($fp)
		sw $s0, -24($fp)
	_lib_label_123 :
		lwc1 $f10, -48($fp)
		cvt.w.s $f0, $f10
		mfc1 $s0, $f0
		li $s1, 1	 # _tmp_68 = 1
		add $s2, $s0, $s1	 # _tmp_69 = _tmp_67 +int _tmp_68
		mtc1 $s2, $f20
		cvt.s.w $f20, $f20
		mov.s $f0, $f20
		j _ceil_end
	_ceil_end : 
		swc1 $f10, -48($fp)
		swc1 $f20, -44($fp)
		sw $s0, -32($fp)
		sw $s1, -36($fp)
		sw $s2, -40($fp)
		lw $sp, -8($fp) 	 # load caller func size
		lw $ra, -4($fp) 	 # restore return address
		addu $fp, $fp, $sp 	 # restore frame pointer
		jr $ra 	 # Return
	power : 
		subu $fp, $fp, $sp
		sw $sp, -8($fp) 	 # preserve func size
		sw $ra, -4($fp) 	 # preserve return address
		move $s0, $a0	 # get value of x
		move $s1, $a1	 # get value of y
		li $s2, 1	 # _tmp_71 = 1
		move $s3, $s2	 # res = _tmp_71
		sw $s0, -52($fp)
		sw $s1, -56($fp)
		sw $s2, -16($fp)
		sw $s3, -48($fp)
	_lib_label_132 :
		li $s0, 0	 # _tmp_72 = 0
		lw $s1, -56($fp)
		sgt $s2, $s1, $s0	 # _tmp_73 = y >int _tmp_72? 1 : 0
		move $a0, $s2
		sw $s0, -20($fp)
		sw $s1, -56($fp)
		sb $s2, -9($fp)
		bnez $a0, _lib_label_136
		j _lib_label_148
	_lib_label_136 :
		li $s0, 1	 # _tmp_74 = 1
		lw $s1, -56($fp)
		and $s2, $s1, $s0	 # _tmp_75 = y & _tmp_74
		move $a0, $s2
		sw $s0, -24($fp)
		sw $s1, -56($fp)
		sw $s2, -28($fp)
		bnez $a0, _lib_label_140
		j _lib_label_142
	_lib_label_140 :
		lw $s1, -48($fp)
		lw $s2, -52($fp)
		mul $s0, $s1, $s2	 # _tmp_76 = res *int x
		move $s1, $s0	 # res = _tmp_76
		sw $s0, -32($fp)
		sw $s1, -48($fp)
		sw $s2, -52($fp)
	_lib_label_142 :
		li $s0, 1	 # _tmp_77 = 1
		lw $s1, -56($fp)
		srl $s2, $s1, $s0	 # _tmp_78 = y >> _tmp_77
		move $s1, $s2	 # y = _tmp_78
		lw $s4, -52($fp)
		mul $s3, $s4, $s4	 # _tmp_79 = x *int x
		move $s4, $s3	 # x = _tmp_79
		sw $s0, -36($fp)
		sw $s1, -56($fp)
		sw $s2, -40($fp)
		sw $s3, -44($fp)
		sw $s4, -52($fp)
		j _lib_label_132
	_lib_label_148 :
		lw $s0, -48($fp)
		move $v0, $s0
		j _power_end
	_power_end : 
		sw $s0, -48($fp)
		lw $sp, -8($fp) 	 # load caller func size
		lw $ra, -4($fp) 	 # restore return address
		addu $fp, $fp, $sp 	 # restore frame pointer
		jr $ra 	 # Return
	sin : 
		subu $fp, $fp, $sp
		sw $sp, -8($fp) 	 # preserve func size
		sw $ra, -4($fp) 	 # preserve return address
		mov.s $f10, $f12	 # get value of x
		li $s0, 1	 # _tmp_80 = 1
		move $s1, $s0	 # n = _tmp_80
		li.s $f20, 3.142000	 # _tmp_81 = 3.142000
		li.s $f22, 180.000000	 # _tmp_82 = 180.000000
		div.s $f24, $f20, $f22	 # _tmp_83 = _tmp_81 /float _tmp_82
		mul.s $f26, $f10, $f24	 # _tmp_84 = x *float _tmp_83
		mov.s $f10, $f26	 # x = _tmp_84
		mov.s $f28, $f10	 # term = x
		mov.s $f30, $f28	 # sinx = term
		swc1 $f10, -116($fp)
		swc1 $f20, -24($fp)
		swc1 $f22, -28($fp)
		swc1 $f24, -32($fp)
		swc1 $f26, -36($fp)
		swc1 $f28, -112($fp)
		swc1 $f30, -108($fp)
		sw $s0, -20($fp)
		sw $s1, -104($fp)
	_lib_label_160 :
		li $s0, 2	 # _tmp_85 = 2
		lw $s2, -104($fp)
		mul $s1, $s0, $s2	 # _tmp_86 = _tmp_85 *int n
		li $s3, 2	 # _tmp_87 = 2
		mul $s4, $s3, $s2	 # _tmp_88 = _tmp_87 *int n
		li $s5, 1	 # _tmp_89 = 1
		add $s6, $s4, $s5	 # _tmp_90 = _tmp_88 +int _tmp_89
		mul $s7, $s1, $s6	 # _tmp_91 = _tmp_86 *int _tmp_90
		move $t0, $s7	 # denominator = _tmp_91
		lwc1 $f20, -112($fp)
		neg.s $f10, $f20	 # _tmp_92 = -term
		lwc1 $f24, -116($fp)
		mul.s $f22, $f10, $f24	 # _tmp_93 = _tmp_92 *float x
		mul.s $f26, $f22, $f24	 # _tmp_94 = _tmp_93 *float x
		mtc1 $t0, $f28
		cvt.s.w $f28, $f28
		div.s $f30, $f26, $f28	 # _tmp_96 = _tmp_94 /float _tmp_95
		mov.s $f20, $f30	 # term = _tmp_96
		lwc1 $f6, -108($fp)
		add.s $f4, $f6, $f20	 # _tmp_97 = sinx +float term
		mov.s $f6, $f4	 # sinx = _tmp_97
		li $t1, 1	 # _tmp_98 = 1
		add $t2, $s2, $t1	 # _tmp_99 = n +int _tmp_98
		move $s2, $t2	 # n = _tmp_99
		li $t3, 10	 # _tmp_100 = 10
		slt $t4, $s2, $t3	 # _tmp_101 = n <int _tmp_100? 1 : 0
		move $a0, $t4
		swc1 $f10, -68($fp)
		swc1 $f20, -112($fp)
		swc1 $f22, -72($fp)
		swc1 $f24, -116($fp)
		swc1 $f26, -76($fp)
		swc1 $f28, -80($fp)
		swc1 $f30, -84($fp)
		swc1 $f4, -88($fp)
		swc1 $f6, -108($fp)
		sw $s0, -40($fp)
		sw $s1, -44($fp)
		sw $s2, -104($fp)
		sw $s3, -48($fp)
		sw $s4, -52($fp)
		sw $s5, -56($fp)
		sw $s6, -60($fp)
		sw $s7, -64($fp)
		sw $t0, -100($fp)
		sw $t1, -92($fp)
		sw $t2, -96($fp)
		sw $t3, -16($fp)
		sb $t4, -9($fp)
		bnez $a0, _lib_label_160
		j _lib_label_183
	_lib_label_183 :
		lwc1 $f10, -108($fp)
		mov.s $f0, $f10
		j _sin_end
	_sin_end : 
		swc1 $f10, -108($fp)
		lw $sp, -8($fp) 	 # load caller func size
		lw $ra, -4($fp) 	 # restore return address
		addu $fp, $fp, $sp 	 # restore frame pointer
		jr $ra 	 # Return
	cos : 
		subu $fp, $fp, $sp
		sw $sp, -8($fp) 	 # preserve func size
		sw $ra, -4($fp) 	 # preserve return address
		mov.s $f10, $f12	 # get value of x
		li $s0, 1	 # _tmp_102 = 1
		move $s1, $s0	 # n = _tmp_102
		li.s $f20, 3.142000	 # _tmp_103 = 3.142000
		li.s $f22, 180.000000	 # _tmp_104 = 180.000000
		div.s $f24, $f20, $f22	 # _tmp_105 = _tmp_103 /float _tmp_104
		mul.s $f26, $f10, $f24	 # _tmp_106 = x *float _tmp_105
		mov.s $f10, $f26	 # x = _tmp_106
		li $s2, 1	 # _tmp_107 = 1
		mtc1 $s2, $f28
		cvt.s.w $f28, $f28
		mov.s $f30, $f28	 # term = _tmp_108
		mov.s $f4, $f30	 # cosx = term
		swc1 $f10, -124($fp)
		swc1 $f20, -20($fp)
		swc1 $f22, -24($fp)
		swc1 $f24, -28($fp)
		swc1 $f26, -32($fp)
		swc1 $f28, -40($fp)
		swc1 $f30, -120($fp)
		swc1 $f4, -108($fp)
		sw $s0, -16($fp)
		sw $s1, -116($fp)
		sw $s2, -36($fp)
	_lib_label_197 :
		li $s0, 2	 # _tmp_109 = 2
		lw $s2, -116($fp)
		mul $s1, $s0, $s2	 # _tmp_110 = _tmp_109 *int n
		li $s3, 2	 # _tmp_111 = 2
		mul $s4, $s3, $s2	 # _tmp_112 = _tmp_111 *int n
		li $s5, 1	 # _tmp_113 = 1
		sub $s6, $s4, $s5	 # _tmp_114 = _tmp_112 -int _tmp_113
		mul $s7, $s1, $s6	 # _tmp_115 = _tmp_110 *int _tmp_114
		move $t0, $s7	 # denominator = _tmp_115
		lwc1 $f20, -120($fp)
		neg.s $f10, $f20	 # _tmp_116 = -term
		lwc1 $f24, -124($fp)
		mul.s $f22, $f10, $f24	 # _tmp_117 = _tmp_116 *float x
		mul.s $f26, $f22, $f24	 # _tmp_118 = _tmp_117 *float x
		mtc1 $t0, $f28
		cvt.s.w $f28, $f28
		div.s $f30, $f26, $f28	 # _tmp_120 = _tmp_118 /float _tmp_119
		mov.s $f20, $f30	 # term = _tmp_120
		lwc1 $f6, -108($fp)
		add.s $f4, $f6, $f20	 # _tmp_121 = cosx +float term
		mov.s $f6, $f4	 # cosx = _tmp_121
		li $t1, 1	 # _tmp_122 = 1
		add $t2, $s2, $t1	 # _tmp_123 = n +int _tmp_122
		move $s2, $t2	 # n = _tmp_123
		li $t3, 10	 # _tmp_124 = 10
		slt $t4, $s2, $t3	 # _tmp_125 = n <int _tmp_124? 1 : 0
		move $a0, $t4
		swc1 $f10, -72($fp)
		swc1 $f20, -120($fp)
		swc1 $f22, -76($fp)
		swc1 $f24, -124($fp)
		swc1 $f26, -80($fp)
		swc1 $f28, -84($fp)
		swc1 $f30, -88($fp)
		swc1 $f4, -92($fp)
		swc1 $f6, -108($fp)
		sw $s0, -44($fp)
		sw $s1, -48($fp)
		sw $s2, -116($fp)
		sw $s3, -52($fp)
		sw $s4, -56($fp)
		sw $s5, -60($fp)
		sw $s6, -64($fp)
		sw $s7, -68($fp)
		sw $t0, -112($fp)
		sw $t1, -96($fp)
		sw $t2, -100($fp)
		sw $t3, -104($fp)
		sb $t4, -9($fp)
		bnez $a0, _lib_label_197
		j _lib_label_220
	_lib_label_220 :
		lwc1 $f10, -108($fp)
		mov.s $f0, $f10
		j _cos_end
	_cos_end : 
		swc1 $f10, -108($fp)
		lw $sp, -8($fp) 	 # load caller func size
		lw $ra, -4($fp) 	 # restore return address
		addu $fp, $fp, $sp 	 # restore frame pointer
		jr $ra 	 # Return