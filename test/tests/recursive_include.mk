all: test_recursive_include

.PHONY: test_recursive_include
test_recursive_include: STEP=test_recursive_include
test_recursive_include: FILES=src/recursive_include.c 
test_recursive_include: $(PASS)/out.log.test_recursive_include $(PASS)/err.log.test_recursive_include
$(PASS)/out.log.test_recursive_include $(PASS)/err.log.test_recursive_include: src/recursive_include.c $(TGT)
	@echo Test $(STEP): Recursive file inclusion
	$(TGT_E) $(FILES) $(EXCLUDE) --output-format=sparsetree -- $(OUT)
	$(CHECK_E)
