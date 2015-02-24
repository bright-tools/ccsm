PFX     = 
TGT     = ../../../llvm-build/Debug/bin/ccsm.exe
LOG_OUT = out.log
LOG_OUT_T = out.log.tmp
LOG_ERR = err.log
LOG_ERR_T = err.log.tmp
PP_ERR  = sed -e 's/\s.*src/ src/;s/.*clang/ clang/;' > $(LOG_ERR)
PP_OUT  = sed -e 's/\s.*src/ src/;s/.*clang/ clang/;s/.*VC\\include/VC\\include/;' > $(LOG_OUT)
OUT     = 2>$(LOG_ERR_T) 1>$(LOG_OUT_T) && cat $(LOG_ERR_T) | $(PP_ERR) && cat $(LOG_OUT_T) | $(PP_OUT)
TGT_E   = $(PFX) $(TGT)
DIFF_OPT= -c -b
PASS    = expected
PASS_LOG= $(PASS)/out.log.$(STEP)
PASS_ERR= $(PASS)/err.log.$(STEP)
CHECK_E = $(PFX) diff $(DIFF_OPT) $(PASS_LOG) out.log && diff $(DIFF_OPT) $(PASS_ERR) err.log && touch $(PASS_ERR) $(PASS_LOG)
EXCLUDE = --exclude-function=dummy,dummy2

.PHONY: all
all:
	@echo -e \\nAll tests done \(use 'all_force' target to force a retest\)

.PHONY: retouch_tgt
retouch_tgt:
	$(PFX) touch $(TGT)

.PHONY: depend
depend:
	$(PFX) makedepend.exe -o.c src/*.c

.PHONY: all_force
all_force: retouch_tgt all

.PHONY: clean
clean:
	@echo Cleaning up
	$(PFX) rm -rf $(LOG_OUT) $(LOG_ERR) $(LOG_ERR_T)

test1: $(TGT) $(PASS)/out.log.$(STEP) $(PASS)/out.err.$(STEP)
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

include tests/*.mk

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

# DO NOT DELETE

src/1.c: src/1.h
src/3.c: src/1.h
src/comment.c: src/comment.h
src/comment2.c: src/comment.h src/comment_free.h src/comment_excluded.h
src/file_size.c: src/file_size.h
src/file_size2.c: src/file_size.h
src/fn_call.c: src/fn_call.h
src/fn_call2.c: src/fn_call.h src/fn_call2.h
src/kw_extern.c: src/kw_extern.h
src/oper_sizeof.c: /usr/include/stdint.h /usr/include/bits/wordsize.h
src/second_include.c: src/simple_include.h
src/simple_include.c: src/simple_include.h
