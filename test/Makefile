PFX     = 
TGT     = /cygdrive/C/Users/catalyst/Documents/coverage/build6/Debug/bin/ccsm.exe
LOG_OUT = out.log
LOG_OUT_T = out.log.tmp
LOG_ERR = err.log
LOG_ERR_T = err.log.tmp
PP_ERR  = sed -e 's/\s.*src/ src/;s/.*clang/ clang/;' > $(LOG_ERR)
PP_OUT  = sed -e 's/\s.*src/ src/;s/.*clang/ clang/;' > $(LOG_OUT)
OUT     = 2>$(LOG_ERR_T) 1>$(LOG_OUT_T) && cat $(LOG_ERR_T) | $(PP_ERR) && cat $(LOG_OUT_T) | $(PP_OUT)
TGT_E   = $(PFX) $(TGT)
DIFF_OPT= -c -b
PASS    = expected
CHECK_E = $(PFX) diff $(DIFF_OPT) $(PASS)/out.log.$(STEP) out.log && diff $(DIFF_OPT) $(PASS)/err.log.$(STEP) err.log 
EXCLUDE = --exclude-function=dummy,dummy2

.PHONY: all
all: test_two_include
	@echo -e \\nAll tests run OK

.PHONY: clean
clean:
	@echo Cleaning up
	$(PFX) rm -rf $(LOG_OUT) $(LOG_ERR) $(LOG_ERR_T)

.PHONY: test1
test1: STEP=1
test1: clean
	@echo Test $(STEP): Single C file
	$(TGT_E) src/1.c -- $(OUT)
	$(CHECK_E)

.PHONY: test3
test2: STEP=1
test2: test2
	@echo Test $(STEP): Single C++ file
	$(TGT_E) src/2.cpp -- $(OUT)
	$(CHECK_E)

.PHONY: test3
test3: STEP=1
test3: test2
	@echo Test $(STEP): Multiple file
	$(TGT_E) src/1.c src/2.cpp -- $(OUT)
	$(CHECK_E)

.PHONY: test_oper_array_subscript
test_oper_array_subscript: STEP=test_oper_array_subscript
test_oper_array_subscript: 
#test3
	@echo Test $(STEP): Operator Count: Array Subscript
	$(TGT_E) $(EXCLUDE) src/oper_array_subscript.c  --output-metrics=OP_ARRAY* -- $(OUT)
	$(CHECK_E)

.PHONY: test_oper_deref
test_oper_deref: STEP=test_oper_deref
test_oper_deref: test_oper_array_subscript
	@echo Test $(STEP): Operator Count: Pointer dereference
	$(TGT_E) $(EXCLUDE) src/oper_deref.c  --output-metrics=OP_DEREF* -- $(OUT)
	$(CHECK_E)

.PHONY: test_oper_reference
test_oper_reference: STEP=test_oper_reference
test_oper_reference: test_oper_deref
	@echo Test $(STEP): Operator Count: Rereference - "address of"
	$(TGT_E) $(EXCLUDE) src/oper_reference.c  --output-metrics=OP_ADDR_OF_CNT -- $(OUT)
	$(CHECK_E)

.PHONY: test_oper_member_access_direct
test_oper_member_access_direct: STEP=test_oper_member_access_direct
test_oper_member_access_direct: test_oper_reference
	@echo Test $(STEP): Operator Count: Member access direct - "something.member"
	$(TGT_E) $(EXCLUDE) src/oper_member_access_direct.c --output-metrics=OP_MEMBER_ACCESS* -- $(OUT)
	$(CHECK_E)

.PHONY: test_oper_member_access_indirect
test_oper_member_access_indirect: STEP=test_oper_member_access_indirect
test_oper_member_access_indirect: test_oper_member_access_direct
	@echo Test $(STEP): Operator Count: Member access indirect - "something->member"
	$(TGT_E) $(EXCLUDE) src/oper_member_access_indirect.c --output-metrics=OP_MEMBER_ACCESS* -- $(OUT)
	$(CHECK_E)

.PHONY: test_oper_cast
test_oper_cast: STEP=test_oper_cast
test_oper_cast: test_oper_member_access_indirect
	@echo Test $(STEP): Operator Count: cast
	$(TGT_E) $(EXCLUDE) src/oper_cast.c --output-metrics=OP_CAST* -- $(OUT)
	$(CHECK_E)

.PHONY: test_oper_sizeof
test_oper_sizeof: STEP=test_oper_sizeof
test_oper_sizeof: test_oper_cast
	@echo Test $(STEP): Operator Count: sizeof
	$(TGT_E) $(EXCLUDE) src/oper_sizeof.c --output-metrics=OP_SIZEOF* -- $(OUT)
	$(CHECK_E)

.PHONY: test_var
test_var: STEP=test_var
test_var: test_oper_sizeof
	@echo Test $(STEP): Variable count
	$(TGT_E) $(EXCLUDE) src/var.c --output-metrics=VAR* -- $(OUT)
	$(CHECK_E)

.PHONY: test_inline
test_inline: STEP=test_inline
test_inline: test_var
	@echo Test $(STEP): Inline function count
	$(TGT_E) $(EXCLUDE) src/inline.c --output-metrics=FUNC_INLINE*,TOK_INLINE -- $(OUT)
	$(CHECK_E)

.PHONY: test_fn_call
test_fn_call: STEP=test_fn_call
test_fn_call: test_inline
	@echo Test $(STEP): Function call count
	$(TGT_E) $(EXCLUDE) src/fn_call.c src/fn_call2.c --output-metrics=FUNC_CALLED_BY,OP_FN_CALL* -- $(OUT)
	$(CHECK_E)

.PHONY: test_typedef
test_typedef: STEP=test_typedef
test_typedef: test_fn_call
	@echo Test $(STEP): Typedef count
	$(TGT_E) $(EXCLUDE) src/typedef.c --output-metrics=TYPEDEF*,TOK_TYPEDEF -- $(OUT)
	$(CHECK_E)

.PHONY: test_kw_auto
test_kw_auto: STEP=test_kw_auto
test_kw_auto: test_typedef
	@echo Test $(STEP): 'Auto' keyword count
	$(TGT_E) $(EXCLUDE) src/kw_auto.c --output-metrics=VAR_FN_LOC_AUTO_CNT,TOK_AUTO -- $(OUT)
	$(CHECK_E)

.PHONY: test_kw_extern
test_kw_extern: STEP=test_kw_extern
test_kw_extern: test_kw_auto
	@echo Test $(STEP): 'extern' keyword count
	$(TGT_E) $(EXCLUDE) src/kw_extern.c --output-metrics=VAR_FILE_EXT_CNT,VAR_FN_EXT_CNT,FUNC_EXTERN_IMPL_CNT,FUNC_EXTERN_EXPL_CNT,TOK_EXTERN -- $(OUT)
	$(CHECK_E)

.PHONY: test_kw_register
test_kw_register: STEP=test_kw_register
test_kw_register: test_kw_extern
	@echo Test $(STEP): 'register' keyword count
	$(TGT_E) $(EXCLUDE) src/kw_register.c --output-metrics=VAR_FN_LOC_REG_CNT,TOK_REGISTER -- $(OUT)
	$(CHECK_E)

.PHONY: test_file_size
test_file_size: STEP=test_file_size
test_file_size: test_kw_register
	@echo Test $(STEP): File size and line count metrics
	$(TGT_E) $(EXCLUDE) src/file_size.c src/file_size2.c --output-metrics=FILE_SIZE,FILE_LINE_CNT -- $(OUT)
	$(CHECK_E)

.PHONY: test_fn_size
test_fn_size: STEP=test_fn_size
test_fn_size: test_file_size
	@echo Test $(STEP): Function size
	$(TGT_E) $(EXCLUDE) src/fn_size.c --output-metrics=FUNC_LINE_CNT -- $(OUT)
	$(CHECK_E)

.PHONY: test_comments
test_comments: STEP=test_comments
test_comments: test_fn_size
	@echo Test $(STEP): Comments metric
	$(TGT_E) $(EXCLUDE) src/comment.c src/comment2.c --output-metrics=COMMENT* -- $(OUT)
	$(CHECK_E)

.PHONY: test_include
test_include: STEP=test_include
test_include: test_comments
	@echo Test $(STEP): Simple test of include files
	$(TGT_E) $(EXCLUDE) src/simple_include.c --output-metrics=VAR* -- $(OUT)
	$(CHECK_E)

.PHONY: test_two_include
test_two_include: STEP=test_two_include
test_two_include: test_include
	@echo Test $(STEP): Test of same header file included in 2 source files
	$(TGT_E) $(EXCLUDE) src/simple_include.c src/second_include.c --output-metrics=VAR* -- $(OUT)
	$(CHECK_E)

.PHONY: test_single_file
test_single_file: STEP=test_single_file
test_single_file: test_two_include
	@echo Test $(STEP): Test of FILE_CNT (single file)
	$(TGT_E) $(EXCLUDE) src/empty.c --output-metrics=FILE_CNT -- $(OUT)
	$(CHECK_E)

.PHONY: test_single_file_excluded
test_single_file_excluded: STEP=test_single_file_excluded
test_single_file_excluded: test_single_file
	@echo Test $(STEP): Test of FILE_CNT \(single file excluded\)
	$(TGT_E) $(EXCLUDE) src/empty.c --exclude-file empty.c --output-metrics=FILE_CNT -- $(OUT)
	$(CHECK_E)
