ccsm
====

C Code Source Metrics - tool to gather simple metrics from C code (C++ will hopefully be 
supported in the future).

Metrics may provide insight into the size and complexity of code (both of which have 
potential impacts on the ability to effectively test & maintain it).

Some of the metrics are very straight forward (and arguably may not be
particularly useful by themselves), for example the count of the number of 'if'
conditions.  Other metrics are built on these, such as the [McCabe cyclomatic
complexity](http://en.wikipedia.org/wiki/Cyclomatic_complexity) measure.

The aim is to also include metrics in order to support the set specified by
[HIS](http://portal.automotive-his.de/images/pdf/SoftwareTest/his-sc-metriken.1.3.1_e.pdf).

Output is hierarchical - for example, the metrics relating to each function are
output and the metrics for a file will include the functions relating to the
functions within that file.  A global wrap-up provides a top-level overview of
all files.

CCSM is also able to check metrics against specified limits and output a warning
in the case that the limit is not adhered to.

* [Usage information (including command line parameters)](docs/usage.md)
* [Example output](docs/example_output.md)
* [Details of metrics](docs/metrics_details.md)
* [Building & maintaining](docs/build_and_code.md)

Comparison To Other Tools
=========================

Other tools are available which analyse C code for metrics.  It's possible that the results 
for some of the metrics will vary slightly between the tools.

Useful Links
============

1. [C99 Draft](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf)
