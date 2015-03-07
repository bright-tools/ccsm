all: test_comments

.PHONY: test_comments
test_comments: STEP=test_comments
test_comments: $(PASS)/out.log.test_comments $(PASS)/err.log.test_comments
$(PASS)/out.log.test_comments $(PASS)/err.log.test_comments: src/comment.c src/comment2.c $(TGT) 
	@echo Test $(STEP): Comment Count
	$(TGT_E) $(EXCLUDE) --exclude-file comment_excluded.h src/comment.c src/comment2.c  --output-metrics=COMMENT_CNT,COMMENT_BYTE_COUNT,TOK_HIS_COMF,TOK_HIS_STMT -- $(OUT)
	$(CHECK_E)
