all: test_include

.PHONY: test_include
test_include: STEP=test_include
test_include: $(PASS)/out.log.test_include $(PASS)/err.log.test_include
$(PASS)/out.log.test_include $(PASS)/err.log.test_include: src/simple_include.c $(TGT) 
	@echo Test $(STEP): Simple test of include files
	$(TGT_E) $(EXCLUDE) src/simple_include.c --output-metrics=VAR* -- $(OUT)
	$(CHECK_E)
