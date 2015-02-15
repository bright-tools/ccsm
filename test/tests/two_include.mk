all: test_two_include

.PHONY: test_two_include
test_two_include: STEP=test_two_include
test_two_include: $(PASS)/out.log.test_two_include $(PASS)/err.log.test_two_include
$(PASS)/out.log.test_two_include $(PASS)/err.log.test_two_include: src/simple_include.c src/second_include.c $(TGT) 
	@echo Test $(STEP): Test of same header file included in 2 source files
	$(TGT_E) $(EXCLUDE) src/simple_include.c src/second_include.c --output-metrics=VAR* -- $(OUT)
	$(CHECK_E)

