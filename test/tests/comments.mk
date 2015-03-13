all: test_comments

.PHONY: test_comments
test_comments: STEP=test_comments
test_comments: METRICS=COMMENT_CNT,COMMENT_BYTE_COUNT,TOK_HIS_COMF,TOK_HIS_STMT
test_comments: FILES=src/comment.c src/comment2.c
test_comments: $(PASS)/out.log.test_comments $(PASS)/err.log.test_comments
$(PASS)/out.log.test_comments $(PASS)/err.log.test_comments: src/comment.c src/comment2.c $(TGT) 
	@echo Test $(STEP): Comment Count
	$(TGT_E) $(FILES) $(EXCLUDE) --exclude-file comment_excluded.h --output-metrics=$(METRICS) -- $(OUT)
	$(CHECK_E)
